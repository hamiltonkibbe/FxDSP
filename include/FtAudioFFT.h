//
//  FtAudioFFT.h
//  Test
//
//  Created by Hamilton Kibbe on 5/27/13.
//
//

#ifndef FTAUDIOFFT_H
#define FTAUDIOFFT_H

#include "FtAudioError.h"
#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct FtAudioFFT FtAudioFFT;
    
FtAudioFFT*
FtAudioFFTInit(unsigned length);

FtAudioError_t
FtAudioFFTFree(FtAudioFFT* fft);
    
FtAudioError_t
FtAudioFFTForward(FtAudioFFT*   fft,
                  const float*  inBuffer,
                  float*        outMag,
                  float*        outPhase);

FtAudioError_t
FtAudioFFTInverse(FtAudioFFT*   fft,
                  const float*  inMag,
                  const float*  inPhase,
                  float*        outBuffer);
    
#ifdef __cplusplus
}
#endif

#endif  // FTAUDIOFFT_H
