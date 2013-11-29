/** 
 * @file FtAudioDsp.h
 * @author Hamilton Kibbe
 * @copyright 2013 Hamilton Kibbe
 * @details This module contains DSP routines which operate on float arrays. The
 * implementation will use vectorized code if available for the target 
 * architecture, and will fall back to canonical implementations otherwise.
 */

#ifndef FTAUDIODSP_H
#define FTAUDIODSP_H
#include "FtAudioError.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Convert an array of float samples to 16-bit signed
* @details Converts array of floating point samples in [-1, 1] to signed 16-bit 
 * samples
*
* @param dest       Signed samples array
* @param src        Floating point samples to convert
* @param length     Number of samples.
* @return           Error code.
*/
FTA_Error_t
FTA_FloatBufferToInt16(short* dest, const float* src, unsigned length);
 
    
/** Convert an array of 16-bit signed samples to floats
* @details Converts array of 16-bit integer samples to float samples in [-1,1]
*
* @param dest       floating point samples array
* @param src        integer samples to convert
* @param length     Number of samples.
* @return           Error code.
*/
FTA_Error_t
FTA_Int16BufferToFloat(float* dest, const signed short* src, unsigned length);
 
    
/** Fill an array with a given value
 * @details Fill the passed array with the value passed in as value. Uses
 * a vectorized implementation if available.
 *
 * @param dest          Array to fill
 * @param length        Size of array in samples
 * @param value         Value to use.
 * @return              Error code.
 */
FTA_Error_t
FTA_FillBuffer(float* dest, unsigned length, float value);

    
/** Copy an array
 * @details copy an array from src to dest
 *
 * @param dest          Array to fill
 * @param src           source buffer
 * @param length        Size of array in samples
 * @return              Error code.
 */
FTA_Error_t
FTA_CopyBuffer(float* dest, const float* src, unsigned length);
    
    
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
FTA_Error_t
FTA_BufferAdd(float *dest, float *in1, float *in2, unsigned length);


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
FTA_Error_t
FTA_VectorVectorMultiply(float *dest, float *in1, float *in2, unsigned length);


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
FTA_Error_t
FTA_VectorScalarMultiply(float *dest, float *in1, float scalar, unsigned length);


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
FTA_Error_t
FTA_Convolve(float       *in1, 
                unsigned    in1_length, 
                float       *in2, 
                unsigned    in2_length, 
                float       *dest);
    


#ifdef __cplusplus
}
#endif


#endif // FTAUDIODSP_H