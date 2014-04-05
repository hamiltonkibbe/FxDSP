//
//  FtAudioOptocoupler.c
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 4/5/14.
//  Copyright (c) 2014 Hamilton Kibbe. All rights reserved.
//

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "FtAudioOptocoupler.h"
#include "FtAudioOnePoleFilter.h"


/* Utility Functions **********************************************************/

/* Scale a sample to the output curve given for the given optocoupler type 
 */
static inline float
scale_sample(float sample, Opto_t opto_type)
{
    
    float out = 0.0;
    
    switch (opto_type)
    {
        case OPTO_LDR:
            out = 3.0e-6 / powf(sample+FLT_MIN, 0.7);
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
static inline float
calculate_on_time(float delay, Opto_t opto_type)
{
    /* Prevent Denormals */
    float time = FLT_MIN;
    
    float delay_sq = delay*delay;
    
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
static inline float
calculate_off_time(float delay, Opto_t opto_type)
{
    /* Prevent Denormals */
    float time = FLT_MIN;
    
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





/* FTA_Opto *******************************************************************/
struct FTA_Opto
{
    Opto_t              type;           //model type
    float               sample_rate;
    float               previous;
    float               delay;
    float               on_cutoff;
    float               off_cutoff;
    char                delta_sign;     // sign of signal dv/dt
    FTA_OnePoleFilter*  lp;
};


/* FTA_OptoInit ***************************************************************/
FTA_Opto*
FTA_OptoInit(Opto_t opto_type, float delay, float sample_rate)
{
    // Create opto struct
	FTA_Opto* opto = (FTA_Opto*)malloc(sizeof(FTA_Opto));
    
    // Initialization
    opto->type = opto_type;
    opto->sample_rate = sample_rate;
    opto->previous = 0;
    opto->delta_sign = 1;
    FTA_OptoSetDelay(opto, delay);
    opto->lp = FTA_OnePoleFilterInit(opto->on_cutoff, opto->sample_rate);
    
    return opto;
}




/* FTA_OptoFree ***************************************************************/
FTA_Error_t
FTA_OptoFree(FTA_Opto* optocoupler)
{
    if (optocoupler)
    {
        if (optocoupler->lp)
        {
            free(optocoupler->lp);
        }
        free(optocoupler);
    }
     return FT_NOERR;
}


/* FTA_OptoSetDelay ***********************************************************/
FTA_Error_t
FTA_OptoSetDelay(FTA_Opto* optocoupler, float delay)
{
    optocoupler->delay = delay;
    optocoupler->on_cutoff = 1.0/calculate_on_time(delay, optocoupler->type);
    optocoupler->off_cutoff = 1.0/calculate_off_time(delay, optocoupler->type);
    return FT_NOERR;
}

/* FTA_OptoProcess ************************************************************/
FTA_Error_t
FTA_OptoProcess(FTA_Opto*       optocoupler,
                float*          out_buffer,
                const float*    in_buffer,
                unsigned        n_samples)
{
    
     return FT_NOERR;
}


/* FTA_OptoTick ***************************************************************/
float
FTA_OptoTick(FTA_Opto* opto, float in_sample)
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
            FTA_OnePoleFilterSetCutoff(opto->lp, opto->on_cutoff);
        }
        else
        {
            FTA_OnePoleFilterSetCutoff(opto->lp, opto->off_cutoff);
        }
    }
    
    /* Do Delay model */
    out = FTA_OnePoleFilterTick(opto->lp, in_sample);
    opto->previous = out;
    out = scale_sample(out, opto->type);
    
    /* spit out sample */
    return out;
}
