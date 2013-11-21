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
    
/** Opaque FtAudioOnePoleFilter structure */
typedef struct FtAudioOnePoleFilter FtAudioOnePoleFilter;
   

FtAudioOnePoleFilter*
FtAudioOnePoleFilterInit(float cutoff, float sampleRate);

FtAudioError_t
FtAudioOnePoleFilterFree(FtAudioOnePoleFilter *filter);
    
FtAudioError_t
FtAudioOnePoleFilterSetCutoff(FtAudioOnePoleFilter* filter, float cutoff);

FtAudioError_t
FtAudioOnePoleFilterProcess(FtAudioOnePoleFilter* filter,
                            float*               outBuffer,
                            const float*         inBuffer,
                            unsigned             n_samples);

#ifdef __cplusplus
}
#endif


#endif  //FTAUDIOONEPOLEFILTER_H
