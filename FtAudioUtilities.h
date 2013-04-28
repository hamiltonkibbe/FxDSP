/** 
 * @file FtAudioUtilities.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef FTAUDIOUTILITIES_H_
#define FTAUDIOUTILITIES_H_

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	
const float INT16_TO_FLOAT_SCALAR = (1/32768.0);

inline float int16ToFloat(int sample)
{
    return (float)(sample * INT16_TO_FLOAT_SCALAR;
}
	
	
/** Convert a ratio to dB
 *
 * @details convert a voltage ratio to dB.
 *
 * @param ratio		The ratio to convert.
 * @return 			Ratio value in dB.	
 */
float to_dB(float ratio);

/** Convert a value in dB to a ratio
 *
 * @details convert a dBFS value to a voltage ratio.
 *
 * @param dB		The value in dB to convert.
 * @return 			dB value as a voltage ratio.
 */
float to_ratio(float dB);

/** Convert from Rad/s to Hz
 *
 * @details Convert a frequency in Rad/s to Hz
 *
 * @param radians		Frequency in Radians/s
 * @param sampleRate	System sampling frequency (in Samples/s)
 * @return 				Frequency value in Hz	
 */
float radians_to_Hz(float radians, long long sampleRate);

/** Convert from Gz to Rad/s
 *
 * @details Convert a frequency in Hz to Rad/s
 *
 * @param Hz			Frequency in Hz
 * @param sampleRate	System sampling frequency (in Samples/s)
 * @return 				Frequency value in Rad/s	
 */
float Hz_to_radians(float Hz, long long sampleRate);



#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOUTILITIES_H_ */
