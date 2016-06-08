//
//  OnePoleFilter.h
//  Test
//
//  Created by Hamilton Kibbe on 5/26/13.
//
//

#ifndef ONEPOLEFILTER_H
#define ONEPOLEFILTER_H

#include "Error.h"
#include "FilterTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/** OnePoleLP type */
typedef struct OnePole OnePole;
typedef struct OnePoleD OnePoleD;

OnePole*
OnePoleInit(float cutoff, float sampleRate, Filter_t type);
    
OnePoleD*
OnePoleInitD(double cutoff, double sampleRate, Filter_t type);
    
OnePole*
OnePoleRawInit(float beta, float alpha);

OnePoleD*
OnePoleRawInitD(double beta, double alpha);

Error_t
OnePoleFree(OnePole *filter);
    
Error_t
OnePoleFreeD(OnePoleD *filter);

Error_t
OnePoleFlush(OnePole *filter);

Error_t
OnePoleFlushD(OnePoleD *filter);

Error_t
OnePoleSetType(OnePole* filter, Filter_t type);

Error_t
OnePoleSetTypeD(OnePoleD* filter, Filter_t type);

    
Error_t
OnePoleSetCutoff(OnePole* filter, float cutoff);
    
Error_t
OnePoleSetCutoffD(OnePoleD* filter, double cutoff);

    
Error_t
OnePoleSetSampleRate(OnePole* filter, float sampleRate);
    
Error_t
OnePoleSetSampleRateD(OnePoleD* filter, double sampleRate);
    
Error_t
OnePoleSetCoefficients(OnePole* filter, float* beta, float* alpha);

Error_t
OnePoleSetCoefficientsD(OnePoleD* filter, double* beta, double* alpha);
  
  Error_t
OnePoleProcess(OnePole*         filter,
                 float*         outBuffer,
                 const float*   inBuffer,
                 unsigned       n_samples);

Error_t
OnePoleProcessD(OnePoleD*   filter,
                 double*        outBuffer,
                 const double*  inBuffer,
                 unsigned       n_samples);
    
    
float
OnePoleTick(OnePole*    filter,
              float         inSample);
    
double
OnePoleTickD(OnePoleD*  filter,
               double       inSample);


    
float
OnePoleAlpha(OnePole* filter);

double
OnePoleAlphaD(OnePoleD* filter);
 
float
OnePoleBeta(OnePole* filter);

double
OnePoleBetaD(OnePoleD* filter);
    
#ifdef __cplusplus
}
#endif


#endif  //ONEPOLEFILTER_H
