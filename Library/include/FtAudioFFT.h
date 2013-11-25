//
//  FtAudio.h
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

typedef struct FTA_FFTConfig FTA_FFTConfig;

    
/** Create a new FTA_FFTConfig
 *
 * @details Allocates memory and returns an initialized FTA_FFTConfig,
 *			which is used to store the FFT Configuration. Play nice and call 
 *          FTA_FFTFree on it when you're done.
 *
 * @param length        length of the FFT. should be a power of 2.
 * @return 				An initialized FTA_FFTConfig.
 */
FTA_FFTConfig*
FTA_FFTInit(unsigned length);

    
/** Free memory associated with a FTA_FFTConfig
 *
 * @details release all memory allocated by FTA_FFTInit for the supplied
 *			fft configuration.
 *
 * @param fft       pointer to the FTA_FFTConfig to free.
 * @return			Error code, 0 on success
 */
FTA_Error_t
FTA_FFTFree(FTA_FFTConfig* fft);

    
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
FTA_Error_t
FTA_FFTForward(FTA_FFTConfig*     fft,
                  const float*          inBuffer,
                  float*                outMag,
                  float*                outPhase);

                  
/** Calculate Real Forward FFT
 *
 * @details Calculates the magnitude and phase of the real forward FFT of the
 *          data in inBuffer.
 *
 * @param fft       Pointer to the FFT configuration.
 * @param inBuffer  Input data. should be the same size as the fft.
 * @param out       Allocated buffer where the transform will be written. length
 *                  should be the same as the fft length
 * @return          Error code, 0 on success.
 */
FTA_Error_t
FTA_FFTForwardInterleaved(FTA_FFTConfig*  fft,
                             DSPComplex*        in_buffer,
                             DSPComplex*        out);
                  
/** Calculate Real Forward FFT
 *
 * @details Calculates the magnitude and phase of the real forward FFT of the
 *          data in inBuffer. 
 *
 * @param fft       Pointer to the FFT configuration.
 * @param inBuffer  Input data. should be the same size as the fft.
 * @param out       Allocated DSPSPlitComplex
 * @return          Error code, 0 on success.
 */
FTA_Error_t
FTA_FFTForwardSplit(FTA_FFTConfig*    fft,
                       DSPComplex*          in_buffer,
                       DSPSplitComplex*     out);
    
             
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
FTA_Error_t
FTA_FFTInverse(FTA_FFTConfig*     fft,
                  const float*          inMag,
                  const float*          inPhase,
                  float*                outBuffer);


/** Calculate Real Inverse FFT
 *
 * @details Calculates the real inverse FFT of the data in inBuffer
 *
 * @param fft       Pointer to the FFT configuration.
 * @param inBuffer  interleaved real/imaginary input buffer. length same as fft.
 * @param outBuffer Allocated buffer where the ifft will be written. length
 *                  should be the same as the fft size.
 * @return          Error code, 0 on success.
 */
FTA_Error_t
FTA_FFTInverseInterleaved(FTA_FFTConfig*     fft,
                             const float*          inBuffer,
                              float*                outBuffer);

/** Calculate Real Inverse FFT
*
* @details Calculates the real inverse FFT of the split-complex data in in_buffer
*
* @param fft       Pointer to the FFT configuration.
* @param in_buffer  interleaved real/imaginary input buffer. length same as fft.
* @param out_buffer Allocated buffer where the ifft will be written. length
*                  should be the same as the fft size.
* @return          Error code, 0 on success.
*/
FTA_Error_t
FTA_FFTInverseSplit(FTA_FFTConfig*     fft,
                       DSPSplitComplex*      in_buffer,
                       DSPComplex*           out_buffer);
    
    
/** Perform Convolution using FFT*
 * @details convolve in1 with in2 and write results to dest
 * @param in1           First input to convolve.
 * @param in1_length    Length [samples] of in1.
 * @param in2           Second input to convolve.
 * @param in2_length    Length[samples] of second input.
 * @param dest          Output buffer. needs to be of length 
 *                      in1_length + in2_length - 1
 * @return              Error code.
 */
FTA_Error_t
FTA_FFTConvolve(FTA_FFTConfig* fft,
                    float       *in1, 
                   unsigned    in1_length, 
                   float       *in2, 
                   unsigned    in2_length, 
                   float       *dest);

                   
/** Perform Convolution using FFT*
 * @details Convolve in1 with IFFT(fft_ir) and write results to dest.
 *          This takes an already transformed kernel as the second argument, to
 *          be used in an LTI filter, where the FFT of the kernel can be pre-
 *          calculated.
 * @param in1           First input to convolve.
 * @param in1_length    Length [samples] of in1.
 * @param fft_ir        Second input to convolve (Already FFT'ed).
 * @param dest          Output buffer. needs to be of length 
 *                      in1_length + in2_length - 1
 * @return              Error code.
 */
FTA_Error_t
FTA_FFTFilterConvolve(FTA_FFTConfig*  fft,
                         float*             in1,
                         unsigned           in1_length, 
                         DSPSplitComplex    fft_ir,
                         float*             dest);

    
    
/** Just prints the complex output
 *
 */
FTA_Error_t
FTA_FFTdemo(FTA_FFTConfig *         fft,
               float*               buffer);


    
#ifdef __cplusplus
}
#endif

#endif  // FTAUDIOFFT_H
