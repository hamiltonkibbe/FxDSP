/** 
 * @file FtAudioUtilities.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef FTAUDIOUTILITIES_H
#define FTAUDIOUTILITIES_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
    
/* Scalar for converting int to float samples */
static const float INT16_TO_FLOAT_SCALAR = (1/32768.0);

/** Convert signed sample to float
 *
 * @details convert a signed 16 bit sample to a 32 bit float sample in the range
 * [-1.0, 1,0]
 *
 * @param sample    The sample to convert.
 * @return          The sample as a float.
 */
float 
int16ToFloat(signed short sample);


/** Convert a float sample to signed
 *
 * @details convert a 32 bit float sample in the range [-1.0, 1,0] to a signed
 * 16 bit sample.
 *
 * @param sample    The sample to convert.
 * @return          The sample as a 16-bit signed int.
 */
signed short
floatToInt16(float sample);
    
/** Convert a buffer of 16 bit audio to float
 * 
 * @param inBuffer  The buffer of 16-bit samples to convert
 * @param outBuffer The buffer to write the samples to.
 * @param nSamples  The number of samples to convert.
 */
void
int_buffer_to_float(const signed short* inBuffer, float* outBuffer, unsigned nSamples);
    
/** Convert a buffer floating point samples to 16-bit
* 
* @param inBuffer   The buffer of floating point samples to convert
* @param outBuffer  The buffer to write the samples to.
* @param nSamples   The number of samples to convert.
*/
void
float_buffer_to_int(const float* inBuffer, signed short* outBuffer, unsigned nSamples); 
    
/** Convert a ratio to dB
 *
 * @details convert a voltage ratio to dB.
 *
 * @param ratio     The ratio to convert.
 * @return          Ratio value in dB.  
 */
float
to_dB(float ratio);

/** Convert a value in dB to a ratio
 *
 * @details convert a dBFS value to a voltage ratio.
 *
 * @param dB        The value in dB to convert.
 * @return          dB value as a voltage ratio.
 */
float
to_ratio(float dB);

/** Convert from Rad/s to Hz
 *
 * @details Convert a frequency in Rad/s to Hz
 *
 * @param radians       Frequency in Radians/s
 * @param sampleRate    System sampling frequency (in Samples/s)
 * @return              Frequency value in Hz   
 */
float
radians_to_Hz(float radians, long long sampleRate);

/** Convert from Gz to Rad/s
 *
 * @details Convert a frequency in Hz to Rad/s
 *
 * @param Hz            Frequency in Hz
 * @param sampleRate    System sampling frequency (in Samples/s)
 * @return              Frequency value in Rad/s    
 */
float
Hz_to_radians(float Hz, long long sampleRate);

/** Fill an array with a given value
 *
 * @details Fill the passed array with the value passed in as value. Uses
 * a vectorized implementation if available.
 *
 * @param dest          array to fill
 * @param length        size of array in samples
 * @param value         value to use.
 */
void
FtAudioFillBuffer(float* dest, unsigned length, float value);


#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOUTILITIES_H_ */
