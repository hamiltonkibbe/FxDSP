//
//  Optocoupler.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/5/14.
//  Copyright (c) 2014 Hamilton Kibbe. All rights reserved.
//


#include "Optocoupler.h"
#include "OnePole.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>

/* Utility Functions **********************************************************/

/* Scale a sample to the output curve given for the given optocoupler type
 */
static inline double
scale_sample(double sample, Opto_t opto_type)
{

    double out = 0.0;

    switch (opto_type)
    {
        case OPTO_LDR:
            out = 3.0e-6 / pow(sample+DBL_MIN, 0.7);
            out = out > 1.0? 1.0 : out;
            break;
        case OPTO_PHOTOTRANSISTOR:
            out = sample;
            break;
        default:
            break;
    }
    return out;
}


/* Calculate the turn-on times [in seconds] for the given optocoupler type with
 the specified delay value
 */
static inline double
calculate_on_time(double delay, Opto_t opto_type)
{
    /* Prevent Denormals */
    double time = DBL_MIN;

    double delay_sq = delay*delay;

    switch (opto_type)
    {
        case OPTO_LDR:
            time = 0.01595 * delay_sq + 0.02795 * delay + 1e-5;
            break;

        case OPTO_PHOTOTRANSISTOR:
            time = 0.01595 * delay_sq + 0.02795 * delay + 1e-5;
            break;

        default:
            break;
    }
    return time;
}


/* Calculate the turn-off times [in seconds] for the given optocoupler type with
 the specified delay value
 */
static inline double
calculate_off_time(double delay, Opto_t opto_type)
{
    /* Prevent Denormals */
    double time = DBL_MIN;

    switch (opto_type)
    {
        case OPTO_LDR:
            time = 1.5*powf(delay+FLT_MIN,3.5);
            break;

        case OPTO_PHOTOTRANSISTOR:
            time = 1.5*powf(delay+FLT_MIN,3.5);
            break;
        default:
            break;
    }
    return time;
}


/* Opto *******************************************************************/
struct Opto
{
    Opto_t      type;           //model type
    float       sample_rate;
    float       previous;
    float       delay;
    float       on_cutoff;
    float       off_cutoff;
    char        delta_sign;     // sign of signal dv/dt
    OnePole*  lp;
};



struct OptoD
{
    Opto_t      type;           //model type
    double      sample_rate;
    double      previous;
    double      delay;
    double      on_cutoff;
    double      off_cutoff;
    char        delta_sign;     // sign of signal dv/dt
    OnePoleD* lp;
};

/* OptoInit ***************************************************************/
Opto*
OptoInit(Opto_t opto_type, float delay, float sample_rate)
{
    // Create opto struct
	Opto* opto = (Opto*)malloc(sizeof(Opto));
    if (opto)
    {
        // Initialization
        opto->type = opto_type;
        opto->sample_rate = sample_rate;
        opto->previous = 0;
        opto->delta_sign = 1;
        OptoSetDelay(opto, delay);
        opto->lp = OnePoleInit(opto->on_cutoff, opto->sample_rate, LOWPASS);
    }
    return opto;
}


OptoD*
OptoInitD(Opto_t opto_type, double delay, double sample_rate)
{
    // Create opto struct
    OptoD* opto = (OptoD*)malloc(sizeof(OptoD));
    if (opto)
    {
        // Initialization
        opto->type = opto_type;
        opto->sample_rate = sample_rate;
        opto->previous = 0;
        opto->delta_sign = 1;
        OptoSetDelayD(opto, delay);
        opto->lp = OnePoleInitD(opto->on_cutoff, opto->sample_rate, LOWPASS);
    }
    return opto;
}

/* OptoFree ***************************************************************/
Error_t
OptoFree(Opto* optocoupler)
{
    if (optocoupler)
    {
        if (optocoupler->lp)
        {
            OnePoleFree(optocoupler->lp);
        }
        free(optocoupler);
    }
     return NOERR;
}


Error_t
OptoFreeD(OptoD* optocoupler)
{
    if (optocoupler)
    {
        if (optocoupler->lp)
        {
            OnePoleFreeD(optocoupler->lp);
        }
        free(optocoupler);
    }
    return NOERR;
}


/* OptoSetDelay ***********************************************************/
Error_t
OptoSetDelay(Opto* optocoupler, float delay)
{
    optocoupler->delay = delay;
    optocoupler->on_cutoff = 1.0/(float)calculate_on_time((double)delay, optocoupler->type);
    optocoupler->off_cutoff = 1.0/(float)calculate_off_time((double)delay, optocoupler->type);
    return NOERR;
}

Error_t
OptoSetDelayD(OptoD* optocoupler, double delay)
{
    optocoupler->delay = delay;
    optocoupler->on_cutoff = 1.0/calculate_on_time(delay, optocoupler->type);
    optocoupler->off_cutoff = 1.0/calculate_off_time(delay, optocoupler->type);
    return NOERR;
}

/* OptoProcess ************************************************************/
Error_t
OptoProcess(Opto*           optocoupler,
            float*          out_buffer,
            const float*    in_buffer,
            unsigned        n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = OptoTick(optocoupler, in_buffer[i]);
    }
    return NOERR;
}

Error_t
OptoProcessD(OptoD*         optocoupler,
             double*        out_buffer,
             const double*  in_buffer,
             unsigned       n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        out_buffer[i] = OptoTickD(optocoupler, in_buffer[i]);
    }
    return NOERR;
}


/* OptoTick ***************************************************************/
float
OptoTick(Opto* opto, float in_sample)
{
    float out;
    char prev_delta;

    /* Check sign of dv/dt */
    prev_delta = opto->delta_sign;
    opto->delta_sign = (in_sample - opto->previous) >= 0 ? 1 : -1;

    /* Update lopwass if sign changed */
    if (opto->delta_sign != prev_delta)
    {
        if (opto->delta_sign == 1)
        {
            OnePoleSetCutoff(opto->lp, opto->on_cutoff);
        }
        else
        {
            OnePoleSetCutoff(opto->lp, opto->off_cutoff);
        }
    }

    /* Do Delay model */
    out = OnePoleTick(opto->lp, in_sample);
    opto->previous = out;
    out = (float)scale_sample((double)out, opto->type);

    /* spit out sample */
    return out;
}


double
OptoTickD(OptoD* opto, double in_sample)
{
    double out;
    char prev_delta;

    /* Check sign of dv/dt */
    prev_delta = opto->delta_sign;
    opto->delta_sign = (in_sample - opto->previous) >= 0 ? 1 : -1;

    /* Update lopwass if sign changed */
    if (opto->delta_sign != prev_delta)
    {
        if (opto->delta_sign == 1)
        {
            OnePoleSetCutoffD(opto->lp, opto->on_cutoff);
        }
        else
        {
            OnePoleSetCutoffD(opto->lp, opto->off_cutoff);
        }
    }

    /* Do Delay model */
    out = OnePoleTickD(opto->lp, in_sample);
    opto->previous = out;
    out = scale_sample(out, opto->type);

    /* spit out sample */
    return out;
}



