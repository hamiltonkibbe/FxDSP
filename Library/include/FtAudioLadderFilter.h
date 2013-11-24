/*
 *  FtAudioLadderFilter.h
 *  
 *
 *  Created by Hamilton Kibbe on 5/10/13.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */

#ifndef FTAUDIOLADDERFILTER_H_
#define FTAUDIOLADDERFILTER_H_

#include <string.h>
#include "FtAudioError.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Boltzman's constant */
static const float BOLTZMANS_CONSTANT = 1.38e-23;

/** Magnitude of electron charge */
static const float Q = 1.609e-19;

/** Opaque FTA_LadderFilter structure */
typedef struct FTA_LadderFilter FTA_LadderFilter;


/** Initialize a Ladder Filter 
 *
 *  @details allocates memory on the heap for the filter. Make sure to call 
 *  FTA_LadderFilterFree when you're finished.
 *
 */
FTA_LadderFilter* 
FTA_LadderFilterInit(float _sample_rate);


FTA_Error_t 
FTA_LadderFilterFree(FTA_LadderFilter* filter);   


FTA_Error_t  
FTA_LadderFilterFlush(FTA_LadderFilter* filter);


FTA_Error_t
FTA_LadderFilterProcess(FTA_LadderFilter  *filter,
                           float                *outBuffer,
                           float                *inBuffer,
                           unsigned             n_samples);


FTA_Error_t
FTA_LadderFilterSetCutoff(FTA_LadderFilter *filter, float _cutoff);


FTA_Error_t
FTA_LadderFilterSetResonance(FTA_LadderFilter *filter, 
                                float               _resonance);


FTA_Error_t
FTA_LadderFilterSetTemperature(FTA_LadderFilter   *filter, 
                                  float                 tempC);


#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOLADDERFILTER_H_ */