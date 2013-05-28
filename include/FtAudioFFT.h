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

typedef struct FtAudioFFTConfig FtAudioFFTConfig;

    
/** Create a new FtAudioFFTConfig
 *
 * @details Allocates memory and returns an initialized FtAudioFFTConfig,
 *			which is used to store the FFT Configuration. Play nice and call 
 *          FtAudioFFTFree on it when you're done.
 *
 * @param length        length of the FFT. should be a power of 2.
 * @return 				An initialized FtAudioFFTConfig.
 */
FtAudioFFTConfig*
FtAudioFFTInit(unsigned length);

    
/** Free memory associated with a FtAudioFFTConfig
 *
 * @details release all memory allocated by FtAudioFFTInit for the supplied
 *			fft configuration.
 *
 * @param fft       pointer to the FtAudioFFTConfig to free.
 * @return			Error code, 0 on success
 */
FtAudioError_t
FtAudioFFTFree(FtAudioFFTConfig* fft);

    
/** Calculate Real Forward FFT
 *
 * @details Calculates the magnitude and phase of the real forward FFT of the
 *          data in inBuffer.
 *
 * @param fft       Pointer to the FFT configuration.
 * @param inBuffer  Input data. should be the same size as the fft.
 * @param outMag    Allocated buffer where the magnitude will be written. length
 *                  should be (fft length / 2).
 * @param outPhase  Allocated buffer where the phase will be written. length 
 *                  should be (fft length / 2).
 * @return          Error code, 0 on success.
 */
FtAudioError_t
FtAudioFFTForward(FtAudioFFTConfig*     fft,
                  const float*          inBuffer,
                  float*                outMag,
                  float*                outPhase);
    
/** Calculate Real Inverse FFT
 *
 * @details Calculates the real inverse FFT of the data in inMag and inPhase.        
 *
 * @param fft       Pointer to the FFT configuration.
 * @param inMag     FFT magnitude data. length should be (fft length / 2).
 * @param inPhase   FFT phase data. length should be (fft length / 2).
 * @param outBuffer Allocated buffer where the ifft will be written. length
 *                  should be the same as the fft size.
 * @return          Error code, 0 on success.
 */
FtAudioError_t
FtAudioFFTInverse(FtAudioFFTConfig*     fft,
                  const float*          inMag,
                  const float*          inPhase,
                  float*                outBuffer);

    
    
#ifdef __cplusplus
}
#endif

#endif  // FTAUDIOFFT_H
