//
//  OnePoleFilter.c
//
//  Created by Hamilton Kibbe on 5/26/13.
//
//

#include "OnePole.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


/* OnePoleFilter ********************************************************/
struct OnePole
{
    float a0;
    float b1;
    float y1;
    float cutoff;
    float sampleRate;
    Filter_t type;

};

struct OnePoleD
{
    double a0;
    double b1;
    double y1;
    double cutoff;
    double sampleRate;
    Filter_t type;
};

/* OnePoleFilterInit ***************************************************/
OnePole*
OnePoleInit(float cutoff, float sampleRate, Filter_t type)
{
    OnePole *filter = (OnePole*)malloc(sizeof(OnePole));
    if (filter)
    {
        filter->a0 = 1;
        filter->b1 = 0;
        filter->y1 = 0;
        filter->type = type;
        filter->sampleRate = sampleRate;
        OnePoleSetCutoff(filter, cutoff);
    }

    return filter;
}

OnePoleD*
OnePoleInitD(double cutoff, double sampleRate, Filter_t type)
{
    OnePoleD *filter = (OnePoleD*)malloc(sizeof(OnePoleD));
    if (filter)
    {
        filter->a0 = 1;
        filter->b1 = 0;
        filter->y1 = 0;
        filter->type = type;
        filter->sampleRate = sampleRate;
        OnePoleSetCutoffD(filter, cutoff);
    }

    return filter;
}


OnePole*
OnePoleRawInit(float beta, float alpha)
{
  OnePole *filter = (OnePole*)malloc(sizeof(OnePole));
  if (filter)
  {
    filter->a0 = alpha;
    filter->b1 = beta;
    filter->y1 = 0.0;
    filter->type = LOWPASS;
    filter->sampleRate = 0;
  }
  return filter;
}

OnePoleD*
OnePoleRawInitD(double beta, double alpha)
{
  OnePoleD *filter = (OnePoleD*)malloc(sizeof(OnePoleD));
  if (filter)
  {
    filter->a0 = alpha;
    filter->b1 = beta;
    filter->y1 = 0;
    filter->type = LOWPASS;
    filter->sampleRate = 0;
  }
  return filter;
}



Error_t
OnePoleSetType(OnePole* filter, Filter_t type)
{
    if (filter && (type == LOWPASS || type == HIGHPASS))
    {
        filter->type = type;
        OnePoleSetCutoff(filter, filter->cutoff);
        return NOERR;
    }
    else if (filter)
    {
        return VALUE_ERROR;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}

Error_t
OnePoleSetTypeD(OnePoleD* filter, Filter_t type)
{
    if (filter && (type == LOWPASS || type == HIGHPASS))
    {
        filter->type = type;
        OnePoleSetCutoffD(filter, filter->cutoff);
        return NOERR;
    }
    else if (filter)
    {
        return VALUE_ERROR;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}


/* OnePoleFilterSetCutoff **********************************************/
Error_t
OnePoleSetCutoff(OnePole* filter, float cutoff)
{
    filter->cutoff = cutoff;
    if (filter->type == LOWPASS)
    {
        filter->b1 = expf(-2.0 * M_PI * (filter->cutoff / filter->sampleRate));
        filter->a0 = 1.0 - filter->b1;
    }
    else
    {
        filter->b1 = -expf(-2.0 * M_PI * (0.5 - (filter->cutoff / filter->sampleRate)));
        filter->a0 = 1.0 + filter->b1;
    }
    return NOERR;
}


Error_t
OnePoleSetCutoffD(OnePoleD* filter, double cutoff)
{
    filter->cutoff = cutoff;
    if (filter->type == LOWPASS)
    {
        filter->b1 = exp(-2.0 * M_PI * (filter->cutoff / filter->sampleRate));
        filter->a0 = 1.0 - filter->b1;
    }
    else
    {
        filter->b1 = -exp(-2.0 * M_PI * (0.5 - (filter->cutoff / filter->sampleRate)));
        filter->a0 = 1.0 + filter->b1;
    }
    return NOERR;
}

/* OnePoleFilterSetSampleRate **********************************************/
Error_t
OnePoleSetSampleRate(OnePole* filter, float sampleRate)
{
    filter->sampleRate = sampleRate;
    OnePoleSetCutoff(filter, filter->cutoff);
    return NOERR;
}


Error_t
OnePoleSetSampleRateD(OnePoleD* filter, double sampleRate)
{
    filter->sampleRate = sampleRate;
    OnePoleSetCutoffD(filter, filter->cutoff);
    return NOERR;
}

Error_t
OnePoleSetCoefficients(OnePole* filter, float* beta, float* alpha)
{
  filter->b1 = *beta;
  filter->a0 = *alpha;
  return NOERR;
}

Error_t
OnePoleSetCoefficientsD(OnePoleD* filter, double* beta, double* alpha)
{
  filter->b1 = *beta;
  filter->a0 = *alpha;
  return NOERR;
}





/* OnePoleFilterFree ***************************************************/
Error_t
OnePoleFree(OnePole *filter)
{
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    return NOERR;
}


Error_t
OnePoleFreeD(OnePoleD *filter)
{
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    return NOERR;
}


Error_t
OnePoleFlush(OnePole *filter)
{
  if (filter)
  {
    filter->y1 = 0.0;
  }
  return NOERR;
}

Error_t
OnePoleFlushD(OnePoleD *filter)
{
  if (filter)
  {
    filter->y1 = 0.0;
  }
  return NOERR;
}

/* OnePoleFilterProcess ************************************************/
Error_t
OnePoleProcess(OnePole*     filter,
                 float*         outBuffer,
                 const float*   inBuffer,
                 unsigned       n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        outBuffer[i] = filter->y1 = inBuffer[i] * filter->a0 + filter->y1 * filter->b1;
    }
    return NOERR;
}

Error_t
OnePoleProcessD(OnePoleD*   filter,
                 double*        outBuffer,
                 const double*  inBuffer,
                 unsigned       n_samples)
{
    for (unsigned i = 0; i < n_samples; ++i)
    {
        outBuffer[i] = filter->y1 = inBuffer[i] * filter->a0 + filter->y1 * filter->b1;
    }
    return NOERR;
}


/* OnePoleFilterTick ***************************************************/
float
OnePoleTick(OnePole*    filter,
            float       inSample)
{
    return  filter->y1 = inSample * filter->a0 + filter->y1 * filter->b1;
}

double
OnePoleTickD(OnePoleD*  filter,
             double     inSample)
{
    return  filter->y1 = inSample * filter->a0 + filter->y1 * filter->b1;
}


/*******************************************************************************
 OnePoleAlpha */

float
OnePoleAlpha(OnePole* filter)
{
    return filter->a0;
}

double
OnePoleAlphaD(OnePoleD* filter)
{
    return filter->a0;
}

/*******************************************************************************
 OnePoleBeta */

float
OnePoleBeta(OnePole* filter)
{
    return filter->b1;
}

double
OnePoleBetaD(OnePoleD* filter)
{
    return filter->b1;
}