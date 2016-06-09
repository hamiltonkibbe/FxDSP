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

#ifdef USE_FFTW_FFT
#include <fftw3.h>
#endif

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Prefer Accelerate
#if defined(__APPLE__)
#define USE_APPLE_FFT
#endif

// Prefer FFTW over OOURA
#if defined(USE_FFTW_FFT)
#undef USE_APPLE_FFT
#undef USE_OOURA_FFT
#endif

// Fallback to OOURA
#if !defined(USE_FFTW_FFT) && !defined(__APPLE__) && !defined(USE_OOURA_FFT)
#define USE_OOURA_FFT
#undef USE_APPLE_FFT
#endif


#ifdef USE_FFTW_FFT
    typedef fftwf_complex    FFTComplex;
    typedef struct { float* realp; float* imagp;}  FFTSplitComplex;
    typedef fftw_complex     FFTComplexD;
    typedef struct { double* realp; double* imagp;} FFTSplitComplexD;
#elif defined(USE_OOURA_FFT)
    typedef struct { float real; float imag;} FFTComplex;
    typedef struct { float* realp; float* imagp;} FFTSplitComplex;
    typedef struct { double real; double imag;} FFTComplexD;
    typedef struct { double* realp; double* imagp;} FFTSplitComplexD;
#elif defined(USE_APPLE_FFT)
    typedef DSPComplex              FFTComplex;
    typedef DSPSplitComplex         FFTSplitComplex;
    typedef DSPDoubleComplex        FFTComplexD;
    typedef DSPDoubleSplitComplex   FFTSplitComplexD;
#endif


/** FFTConfig type */
typedef struct FFTConfig FFTConfig;
typedef struct FFTConfigD FFTConfigD;


/** Create a new FFTConfig
 *
 * @details Allocates memory and returns an initialized FFTConfig,
 *      which is used to store the FFT Configuration. Play nice and call
 *          FFTFree on it when you're done.
 *
 * @param length        length of the FFT. should be a power of 2.
 * @return        An initialized FFTConfig.
 */
FFTConfig*
FFTInit(unsigned length);

FFTConfigD*
FFTInitD(unsigned length);

/** Free memory associated with a FFTConfig
 *
 * @details release all memory allocated by FFTInit for the supplied
 *      fft configuration.
 *
 * @param fft       pointer to the FFTConfig to free.
 * @return      Error code, 0 on success
 */
Error_t
FFTFree(FFTConfig* fft);

Error_t
FFTFreeD(FFTConfigD* fft);


/** Calculate Real to Complex Forward FFT
 *
 * @details Calculates the magnitude of the real forward FFT of the data in
 *          inBuffer.
 *
 * @param fft       Pointer to the FFT configuration.
 * @param inBuffer  Input data. should be the same size as the fft.
 * @param real      Allocated buffer where the real part will be written. length
 *                  should be (fft->length/2).
 * @param imag      Allocated buffer where the imaginary part will be written. l
 *                  length should be (fft->length/2).
 * @return          Error code, 0 on success.
 */
Error_t
FFT_R2C(FFTConfig*      fft,
        const float*    inBuffer,
        float*          real,
        float*          imag);

Error_t
FFT_R2CD(FFTConfigD*    fft,
         const double*  inBuffer,
         double*        real,
         double*        imag);

Error_t
FFT_IR_R2C(FFTConfig*       fft,
           const float*     inBuffer,
           FFTSplitComplex  out);

Error_t
FFT_IR_R2CD(FFTConfigD*         fft,
            const double*       inBuffer,
            FFTSplitComplexD    out);


/** Calculate Complex to Real Inverse FFT
 *
 * @details Calculates the inverse FFT of the data in inBuffer.
 *
 * @param fft       Pointer to the FFT configuration.
 * @param inReal    Input real part. Length fft->length/2
 * @param inImag    Input imaginary part. Length fft->length/2
 * @param out       Allocated buffer where the signal will be written. length
 *                  should be fft->length.
 * @return          Error code, 0 on success.
 */
Error_t
IFFT_C2R(FFTConfig*    fft,
         const float*  inReal,
         const float*  inImag,
         float*        out);

Error_t
IFFT_C2RD(FFTConfigD*   fft,
          const double* inreal,
          const double* inImag,
          double*       out);


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

Error_t
FFTConvolveD(FFTConfigD*    fft,
             const double*  in1,
             unsigned       in1_length,
             const double*  in2,
             unsigned       in2_length,
             double*        dest);


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
                  const float*      in,
                  unsigned          in_length,
                  FFTSplitComplex   fft_ir,
                  float*            dest);

Error_t
FFTFilterConvolveD(FFTConfigD*      fft,
                   const double*    in,
                   unsigned         in_length,
                   FFTSplitComplexD fft_ir,
                   double*          dest);

/** Just prints the complex output
 *
 */
Error_t
FFTdemo(FFTConfig* fft, float* buffer);



#ifdef __cplusplus
}
#endif

#endif  // FFT_H
