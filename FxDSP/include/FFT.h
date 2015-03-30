//
//  .h
//  Test
//
//  Created by Hamilton Kibbe on 5/27/13.
//
//

#ifndef FFT_H
#define FFT_H

#include "Error.h"



#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
    typedef DSPComplex              FFTComplex;
    typedef DSPSplitComplex         FFTSplitComplex;
    typedef DSPDoubleComplex        FFTComplexD;
    typedef DSPDoubleSplitComplex   FFTSplitComplexD;
#elif defined(__USE_FFTW__)
    typedef struct FFTComplex FFTComplex;
    typedef struct FFTSplitComplex FFTSplitComplex;
    typedef struct FFTComplexD FFTComplexD;
    typedef struct FFTSplitComplexD FFTSplitComplexD;
#endif


/** FFTConfig type */
typedef struct FFTConfig FFTConfig;
typedef struct FFTConfigD FFTConfigD;

    
/** Create a new FFTConfig
 *
 * @details Allocates memory and returns an initialized FFTConfig,
 *			which is used to store the FFT Configuration. Play nice and call 
 *          FFTFree on it when you're done.
 *
 * @param length        length of the FFT. should be a power of 2.
 * @return 				An initialized FFTConfig.
 */
FFTConfig*
FFTInit(unsigned length);

FFTConfigD*
FFTInitD(unsigned length);
    
/** Free memory associated with a FFTConfig
 *
 * @details release all memory allocated by FFTInit for the supplied
 *			fft configuration.
 *
 * @param fft       pointer to the FFTConfig to free.
 * @return			Error code, 0 on success
 */
Error_t
FFTFree(FFTConfig* fft);

Error_t
FFTFreeD(FFTConfigD* fft);

    
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
Error_t
FFTForward(FFTConfig*       fft,
           const float*     inBuffer,
           float*           outMag,
           float*           outPhase);


                  
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
Error_t
FFTForwardInterleaved(FFTConfig*    fft,
                      FFTComplex*   in_buffer,
                      FFTComplex*   out);
                  
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
Error_t
FFTForwardSplit(FFTConfig*          fft,
                FFTComplex*         in_buffer,
                FFTSplitComplex*    out);
    
             
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
Error_t
FFTInverse(FFTConfig*       fft,
           const float*     inMag,
           const float*     inPhase,
           float*           outBuffer);


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
Error_t
FFTInverseInterleaved(FFTConfig*     fft,
                      const float*   inBuffer,
                      float*         outBuffer);

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
Error_t
FFTInverseSplit(FFTConfig*          fft,
                FFTSplitComplex*    in_buffer,
                FFTComplex*         out_buffer);
    
    
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
Error_t
FFTConvolve(FFTConfig* fft,
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
Error_t
FFTFilterConvolve(FFTConfig*        fft,
                  float*            in1,
                  unsigned          in1_length,
                  FFTSplitComplex   fft_ir,
                  float*            dest);

    
    
/** Just prints the complex output
 *
 */
Error_t
FFTdemo(FFTConfig* fft, float* buffer);


    
#ifdef __cplusplus
}
#endif

#endif  // FFT_H
