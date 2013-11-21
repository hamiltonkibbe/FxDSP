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

/** Opaque FtAudioLadderFilter structure */
typedef struct FtAudioLadderFilter FtAudioLadderFilter;



FtAudioLadderFilter* 
FtAudioLadderFilterInit(float _sample_rate);


FtAudioError_t 
FtAudioLadderFilterFree(FtAudioLadderFilter* filter);   


FtAudioError_t  
FtAudioLadderFilterFlush(FtAudioLadderFilter* filter);


FtAudioError_t
FtAudioLadderFilterProcess(FtAudioLadderFilter  *filter,
                           float                *outBuffer,
                           float                *inBuffer,
                           unsigned             n_samples);


FtAudioError_t
FtAudioLadderFilterSetCutoff(FtAudioLadderFilter *filter, float _cutoff);


FtAudioError_t
FtAudioLadderFilterSetResonance(FtAudioLadderFilter *filter, 
                                float               _resonance);


FtAudioError_t
FtAudioLadderFilterSetTemperature(FtAudioLadderFilter   *filter, 
                                  float                 tempC);


#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOLADDERFILTER_H_ */