/**
 * @file Dsp.h
 * @author Hamilton Kibbe
 * @copyright 2013 Hamilton Kibbe
 * @brief Vector DSP routines
 *
 * This module contains DSP routines which operate on single- and double-
 * precision arrays. The implementation will use vectorized code if available
 * for the target architecture, and will fall back to canonical implementations
 * otherwise.
 */

#ifndef DSP_H
#define DSP_H

#include "Error.h"

#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif


#pragma mark - Sample Type Conversion
/** Convert an array of float samples to 16-bit signed
* @details Converts array of floating point samples in [-1, 1] to signed 16-bit
 * samples
*
* @param dest       Signed samples array
* @param src        Floating point samples to convert
* @param length     Number of samples.
* @return           Error code.
*/
Error_t
FloatBufferToInt16(signed short* dest, const float* src, unsigned length);

/** Convert an array of double samples to 16-bit signed
 * @details Converts array of double-precision samples in [-1, 1] to signed 16-
 * bit samples
 *
 * @param dest       Signed samples array
 * @param src        double-precision samples to convert
 * @param length     Number of samples.
 * @return           Error code.
 */
Error_t
DoubleBufferToInt16(signed short* dest, const double* src, unsigned length);


/** Convert an array of 16-bit signed samples to floats
* @details Converts array of 16-bit integer samples to float samples in [-1,1]
*
* @param dest       floating point samples array
* @param src        integer samples to convert
* @param length     Number of samples.
* @return           Error code.
*/
Error_t
Int16BufferToFloat(float* dest, const signed short* src, unsigned length);

/** Convert an array of 16-bit signed samples to double
 * @details Converts array of 16-bit integer samples to double samples in [-1,1]
 *
 * @param dest       double-precisionsamples array
 * @param src        integer samples to convert
 * @param length     Number of samples.
 * @return           Error code.
 */
Error_t
Int16BufferToDouble(double* dest, const signed short* src, unsigned length);


/** Convert double-precision samples to floats
 * @details Convert array of double-precision samples to float samples in [-1,1]
 *
 * @param dest       single-precision sample destination array
 * @param src        double-precision samples to convert
 * @param length     Number of samples.
 * @return           Error code.
 */
Error_t
DoubleToFloat(float* dest, const double* src, unsigned length);


/** Convert single-precision samples to doubles
 * @details Convert array of single-precision samples to doubles in [-1,1]
 *
 * @param dest       double-precision sample destination array
 * @param src        single-precision samples to convert
 * @param length     Number of samples.
 * @return           Error code.
 */
Error_t
FloatToDouble(double* dest, const float* src, unsigned length);



#pragma mark - Fill Buffer
/** Fill an array with a given value
 * @details Fill the passed array with the value passed in as value. Uses
 * a vectorized implementation if available.
 *
 * @param dest          Array to fill
 * @param length        Size of array in samples
 * @param value         Value to use.
 * @return              Error code.
 */
Error_t
FillBuffer(float *dest, unsigned length, float value);

Error_t
FillBufferD(double *dest, unsigned length, double value);



#pragma mark - Clear Buffer
/** Set array to zero
 * @details Fill the passed array with zeros.
 *
 * @param dest          Array to fill
 * @param length        Size of array in samples
 * @return              Error code.
 */
Error_t
ClearBuffer(float *dest, unsigned length);

Error_t
ClearBufferD(double *dest, unsigned length);



#pragma mark - Copy Buffer
/** Copy an array
 * @details copy an array from src to dest
 *
 * @param dest          Array to fill
 * @param src           source buffer
 * @param length        Size of array in samples
 * @return              Error code.
 */
Error_t
CopyBuffer(float* dest, const float* src, unsigned length);

Error_t
CopyBufferD(double* dest, const double* src, unsigned length);


/** Copy an array with given source and destination strides
 * @details copy an array from src to dest
 *
 * @param dest          Array to fill
 * @param dest_stride   Destination stride
 * @param src           source buffer
 * @param src_stride    Sounrce stride
 * @param length        Size of array in samples
 * @return              Error code.
 */
Error_t
CopyBufferStride(float*         dest,
                 unsigned       dest_stride,
                 const float*   src,
                 unsigned       src_stride,
                 unsigned       length);

Error_t
CopyBufferStrideD(double*       dest,
                  unsigned      dest_stride,
                  const double* src,
                  unsigned      src_stride,
                  unsigned      length);



#pragma mark - Split <-> Interleaved conversion
/** Split To Interleaved Complex Vector conversion
 *
 * @details Convert `length` complex values from split to interleaved-complex
 *  format
 * @code
 *      dest[i] = real[i], dest[i+1] = imag[i]| i = [0 : 2 : length]
 * @endcode
 *
 * @param dest          Interleaved complex destination buffer
 * @param real          Buffer of real values
 * @param imag          Buffer of imaginary values
 * @param length        Number of complex values to convert
 * @return              Error code.
 */
Error_t
SplitToInterleaved(float*       dest,
                   const float* real,
                   const float* imag,
                   unsigned     length);


Error_t
SplitToInterleavedD(double*         dest,
                    const double*   real,
                    const double*   imag,
                    unsigned        length);


/** Interleaved to split Complex Vector conversion
 *
 * @details Convert `length` complex values from interleaved to split-complex
 *  format
 * @code
 *      real[i] = input[2*i], imag[i] = input[2*i + 1] | i = [0 - length]
 * @endcode
 *
 * @param real          Destination buffer for real values
 * @param imag          Destination buffer for imaginary values
 * @param input         Interleaved complex input buffer
 * @param length        Number of complex values to convert
 * @return              Error code.
 */
Error_t
InterleavedToSplit(float*       real,
                   float*       imag,
                   const float* input,
                   unsigned     length);

Error_t
InterleavedToSplitD(double*         real,
                    double*         imag,
                    const double*   input,
                    unsigned        length);


#pragma mark - Vector Absolute Value
/** Vector Absolute Value
 * @details Calculate absolute value of elements in in1 and write the
 * results to dest:
 * @code
 *      dest[i] = |in[i]| | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write
 * @param in            Input buffer
 * @param length        Number of samples to negate
 * @return              Error code.
 */
Error_t
VectorAbs(float *dest, const float *in, unsigned length);

Error_t
VectorAbsD(double *dest, const double *in, unsigned length);


#pragma mark - Vector Negation
/** Negate a vector
 * @details Negate every element in in and write the
 * results to dest:
 * @code
 *      dest[i] = -in[i] | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write
 * @param in            Input buffer
 * @param length        Number of samples to negate
 * @return              Error code.
 */
Error_t
VectorNegate(float          *dest,
             const float    *in,
             unsigned       length);

Error_t
VectorNegateD(double        *dest,
              const double  *in,
              unsigned      length);


#pragma mark - Vector Sum
/** Sum all values in an array
 *
 * @param src       Data to sum
 * @param length    Number of samples to sum
 * @return          Sum of all values in src
 */
float
VectorSum(const float* src, unsigned length);

double
VectorSumD(const double* src, unsigned length);


#pragma mark - Vector Min/Max
/** Find the Maximum value in a vector
 *
 * @param src       Vector to search
 * @param length    Vector length in samples
 * @return          Maximum value in vector
 */
float
VectorMax(const float* src, unsigned length);

double
VectorMaxD(const double* src, unsigned length);


/** Find the index and value of the Maximum value in a vector
 *
 * @param value     Maximum value in vector
 * @param index     Index of maximum value
 * @param src       Vector to search
 * @param length    Vector length in samples
 */
Error_t
VectorMaxVI(float* value, unsigned* index, const float* src, unsigned length);

Error_t
VectorMaxVID(double* value, unsigned* index, const double* src, unsigned length);


/** Find the Minimum value in a vector
 *
 * @param src       Vector to search
 * @param length    Vector length in samples
 * @return          Miminum value in vector
 */
float
VectorMin(const float* src, unsigned length);

double
VectorMinD(const double* src, unsigned length);


/** Find the index and value of the Maximum value in a vector
 *
 * @param value     Minimum value in vector
 * @param index     Index of minimum value
 * @param src       Vector to search
 * @param length    Vector length in samples
 */
Error_t
VectorMinVI(float* value, unsigned* index, const float* src, unsigned length);

Error_t
VectorMinVID(double* value, unsigned* index, const double* src, unsigned length);


#pragma mark - Vector Addition
/** Add two buffers
 * @details Add values in in1  to values in in2 element-by-element and write
 * results to dest:
 * @code
 *      dest[i] = in1[i] + in2[i] | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write
 * @param in1           First input buffer
 * @param in2           Second input buffer
 * @param length        Number of samples to add
 * @return              Error code.
 */
Error_t
VectorVectorAdd(float         *dest,
                const float   *in1,
                const float   *in2,
                unsigned      length);


Error_t
VectorVectorAddD(double         *dest,
                 const double   *in1,
                 const double   *in2,
                 unsigned       length);

#pragma mark - Vector Subtraction
/** Subtract two buffers
 * @details Subtract values in in1 from values in in2 element-by-element and write
 * results to dest:
 * @code
 *      dest[i] = in2[i] - in1[i] | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write
 * @param in1           First input buffer
 * @param in2           Second input buffer
 * @param length        Number of samples to add
 * @return              Error code.
 */
Error_t
VectorVectorSub(float         *dest,
                const float   *in1,
                const float   *in2,
                unsigned      length);


Error_t
VectorVectorSubD(double         *dest,
                 const double   *in1,
                 const double   *in2,
                 unsigned       length);

#pragma mark - Vector Scalar Addition
/** Add scalar to a vector
 * @details add scalar to every element in in1 and write results to dest:
 * @code
 *      dest[i] = in1[i] + scalar | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write.
 * @param in1           Input buffer.
 * @param scalar        Scalar value.
 * @param length        Number of samples to add.
 * @return              Error code.
 */
Error_t
VectorScalarAdd(float       *dest,
                const float *in1,
                const float scalar,
                unsigned    length);


Error_t
VectorScalarAddD(double         *dest,
                 const double   *in1,
                 const double   scalar,
                 unsigned       length);


#pragma mark - Vector-Vector Multiplication
/** Multiply two buffers
 * @details Multiply values in in1 by values in in2 element by element and write
 * results to dest:
 * @code
 *      dest[i] = in1[i] * in2[i] | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write
 * @param in1           First input buffer
 * @param in2           Second input buffer
 * @param length        Number of samples to multiply
 * @return              Error code.
 */
Error_t
VectorVectorMultiply(float          *dest,
                     const float    *in1,
                     const float    *in2,
                     unsigned       length);


Error_t
VectorVectorMultiplyD(double        *dest,
                      const double  *in1,
                      const double  *in2,
                      unsigned      length);


#pragma mark - Vector-Scalar Multiplication
/** Multiply buffer by a scalar
 * @details Multiply values in in1 by scalar and write results to dest:
 * @code
 *      dest[i] = in1[i] * scalar | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write.
 * @param in1           Input buffer.
 * @param scalar        Scalar value.
 * @param length        Number of samples to multiply.
 * @return              Error code.
 */
Error_t
VectorScalarMultiply(float          *dest,
                     const float    *in1,
                     const float    scalar,
                     unsigned       length);


Error_t
VectorScalarMultiplyD(double        *dest,
                      const double  *in1,
                      const double  scalar,
                      unsigned      length);


/** Multiply two buffers by a scalar and sum
 * @details Multiply inputs by scalars and write sum to dest:
 * @code
 *      dest[i] = (in1[i] * scalar1) + (in2[i] * scalar2) | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write.
 * @param in1           Input buffer.
 * @param scalar1       Scalar value.
 * @param in2           Input buffer.
 * @param scalar2       Scalar value.
 * @param length        Number of samples to multiply.
 * @return              Error code.
 */
Error_t
VectorVectorMix(float        *dest,
                const float  *in1,
                const float  *scalar1,
                const float  *in2,
                const float  *scalar2,
                unsigned     length);

Error_t
VectorVectorMixD(double        *dest,
                 const double  *in1,
                 const double  *scalar1,
                 const double  *in2,
                 const double  *scalar2,
                 unsigned     length);


/** Sum two vectors and multiply result by a scalar
 * @details Sum input vectors and multiply by a scalar, leaves results in dest:
 * @code
 *      dest[i] = (in1[i] + in2[i]) * scalar | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write.
 * @param in1           Input buffer.
 * @param in2           Input buffer.
 * @param scalar        Scalar value.
 * @param length        Number of samples to multiply.
 * @return              Error code.
 */
Error_t
VectorVectorSumScale(float        *dest,
                     const float  *in1,
                     const float  *in2,
                     const float  *scalar,
                     unsigned     length);

Error_t
VectorVectorSumScaleD(double        *dest,
                      const double  *in1,
                      const double  *in2,
                      const double  *scalar,
                      unsigned     length);

#pragma mark - Vector Power
/** Raise elements of vector to a power
 * @details Raise values in in to power of 'power' and write results to dest:
 * @code
 *      dest[i] = in1[i]^power | i = [0, length)
 * @endcode
 *
 * @param dest          Output array to write.
 * @param in1           Input buffer.
 * @param power         Power to raise input by.
 * @param length        Number of samples to process.
 * @return              Error code.
 */
Error_t
VectorPower(float* dest, const float* in, float power, unsigned length);

Error_t
VectorPowerD(double* dest, const double* in, double power, unsigned length);


#pragma mark - Convolution
/** Perform Convolution *
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
Convolve(float       *in1,
         unsigned    in1_length,
         float       *in2,
         unsigned    in2_length,
         float       *dest);

Error_t
ConvolveD(double    *in1,
          unsigned  in1_length,
          double    *in2,
          unsigned  in2_length,
          double    *dest);


#pragma mark - Vector Amplitude-dB Conversion
/** Convert amplitude values to dB
 * @details Convert an array of amplitude values to their dB equivalent.
 * @param dest      dB output array
 * @param src       Array of amplitude values
 * @param length    Number of samples to process.
 * @return          Error Code
 */
Error_t
VectorDbConvert(float*          dest,
                const float*    src,
                unsigned        length);

Error_t
VectorDbConvertD(double*        dest,
                 const double*  src,
                 unsigned       length);


#pragma mark - Vector Complex Multiplication
/** Multiply two complex vectors
 * @details Complex elementwise multiplication of two vectors.
 * @param re        destination for real part of result.
 * @param im        destination for imaginary part of result.
 * @param re1       real part of first source array.
 * @param im1       imaginary part of first source array.
 * @param re2       real part of second source array.
 * @param im2       imaginary part of second source array.
 * @param length    Number of samples to process.
 * @return          Error Code
 */
Error_t
ComplexMultiply(float*          re,
                float*          im,
                const float*    re1,
                const float*    im1,
                const float*    re2,
                const float*    im2,
                unsigned        length);

Error_t
ComplexMultiplyD(double*        re,
                 double*        im,
                 const double*  re1,
                 const double*  im1,
                 const double*  re2,
                 const double*  im2,
                 unsigned       length);


#pragma mark - Vector Rectangular to Polar conversion
/** Convert vector from rectangular to polar representation
 * @details Convert vector of real-imaginary to vector of magnitude-phase
 * @param magnitude Magnitude destination array
 * @param im        Phase destination array
 * @param real      Real part of source array.
 * @param imaginary Imaginary part of source array.
 * @param length    Number of samples to process.
 * @return          Error Code
 */
Error_t
VectorRectToPolar(float*        magnitude,
                  float*        phase,
                  const float*  real,
                  const float*  imaginary,
                  unsigned      length);

Error_t
VectorRectToPolarD(double*          magnitude,
                   double*          phase,
                   const double*    real,
                   const double*    imaginary,
                   unsigned         length);


#pragma mark - Vector Mean Square calculation
/** Calculate the Mean Square of a vector
 * @details calculates the sum over i of (data[i]^2)/length

 * @param data      Data to process
 * @param length    Number of samples to sum
 * @return          Mean Square of data
 */
float
MeanSquare(const float* data, unsigned length);

double
MeanSquareD(const double* data, unsigned length);

#ifdef __cplusplus
}
#endif


#endif // DSP_H
