/** 
 * @file FtAudioBiquadFilter.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */


#ifndef FTAUDIOBIQUADFILTER_H_
#define FTAUDIOBIQUADFILTER_H_

#include <string.h>
#include "FtAudioError.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Opaque FtAudioBiquadFilter structure */
typedef struct FtAudioBiquadFilter FtAudioBiquadFilter;


/** Create a new FtAudioBiquadFilter
 *
 * @details Allocates memory and returns an initialized FtAudioBiquadFilter.
 *			Play nice and call FtAudioBiquadFilterFree on the filter when 
 *			you're done with it.
 *
 * @param bCoeff	Numerator coefficients.
 * @param aCoeff	Denominator coefficients.
 * @return			An initialized FtAudioBiquadFilter.
 */
FtAudioBiquadFilter* 
FtAudioBiquadFilterInit(const float* bCoeff, 
						const float* aCoeff);


/** Free memory associated with a FtAudioBiquadFilter
 *  
 * @details release all memory allocated by FtAudioBiquadFilterInit for the
 *			supplied filter.
 * @param filter	FtAudioBiquadFilter to free.
 * @return			Error code, 0 on success
 */
FtAudioError_t
FtAudioBiquadFilterFree(FtAudioBiquadFilter* filter);


/** Flush filter state buffers
 *  
 * @param filter	FtAudioBiquadFilter to flush.
 * @return			Error code, 0 on success
 */
FtAudioError_t
FtAudioBiquadFilterFlush(FtAudioBiquadFilter* filter);



/** Filter a buffer of samples
 * @details Uses a DF-I biquad implementation to filter input samples
 *
 * @param filter	The FtAudioBiquadFilter to use.
 * @param outBuffer	The buffer to write the output to.
 * @param inBuffer	The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return			Error code, 0 on success
 */
FtAudioError_t
FtAudioBiquadFilterProcess(FtAudioBiquadFilter*	filter, 
						   float*				outBuffer, 
						   const float*			inBuffer, 
						   unsigned				n_samples);


/** Update the filter kernel for a given filter
 *
 * @param filter	The filter to update
 * @param bCoeff	Numerator coefficients
 * @param aCoeff	Denominator coefficients
 */
FtAudioError_t
FtAudioBiquadFilterUpdateKernel(FtAudioBiquadFilter*	filter,
							    const float*			bCoeff,
							    const float*			aCoeff);


#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOBIQUADFILTER_H_ */