//
//  FtAudioOnePoleFilter.h
//  Test
//
//  Created by Hamilton Kibbe on 5/26/13.
//
//

#ifndef FTAUDIOONEPOLEFILTER_H
#define FTAUDIOONEPOLEFILTER_H

#include "FtAudioError.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/** Opaque FTA_OnePoleFilter structure */
typedef struct FTA_OnePoleFilter FTA_OnePoleFilter;
   

FTA_OnePoleFilter*
FTA_OnePoleFilterInit(float cutoff, float sampleRate);

FTA_Error_t
FTA_OnePoleFilterFree(FTA_OnePoleFilter *filter);
    
FTA_Error_t
FTA_OnePoleFilterSetCutoff(FTA_OnePoleFilter* filter, float cutoff);

FTA_Error_t
FTA_OnePoleFilterProcess(FTA_OnePoleFilter* filter,
                            float*               outBuffer,
                            const float*         inBuffer,
                            unsigned             n_samples);
float
FTA_OnePoleFilterTick(FTA_OnePoleFilter*  filter,
                         float                  inSample);

#ifdef __cplusplus
}
#endif


#endif  //FTAUDIOONEPOLEFILTER_H
