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


#ifdef __cplusplus
extern "C" {
#endif

#pragma mark Sample Type Conversion
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

Error_t
Int16BufferToDouble(double* dest, const signed short* src, unsigned length);

#pragma mark Fill Buffer
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
    
    
#pragma mark Clear Buffer
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

    
#pragma mark Copy Buffer
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

#pragma mark Vector Absolute Value
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


#pragma mark Vector Negation
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


#pragma mark Vector Addition
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

    
#pragma mark Vector Multiplication
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


#pragma mark Vector Power
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
    
    
    
#pragma mark Convolution
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


Error_t
VectorDbConvert(float* dest,
                const float* in1,
                unsigned amplitude_flag,
                unsigned length);


#ifdef __cplusplus
}
#endif


#endif // DSP_H