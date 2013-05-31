/** 
 * @file FtAudioFIRFilter.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef FTAUDIOFIRFILTER_H_
#define FTAUDIOFIRFILTER_H_

#include <string.h>
#include "FtAudioError.h"

#ifdef __cplusplus
extern "C" {
#endif

/** The kernel length at which to use FFT convolution vs direct */
#define USE_FFT_CONVOLUTION_LENGTH (128)


/** Opaque FtAudioFIRFilter structure */
typedef struct FtAudioFIRFilter FtAudioFIRFilter;


/** Create a new FtAudioFIRFilter
 *
 * @details Allocates memory and returns an initialized FtAudioFIRFilter.
 *			Play nice and call FtAudioFIRFilterFree on the filter when you're
 *          done with it.
 *
 * @param filter_kernel	The filter coefficients. These are copied to the
 *                      filter so there is no need to keep them around.
 * @param length		The number of coefficients in filter_kernel.
 * @return 				An initialized FtAudioFIRFilter	
 */
FtAudioFIRFilter* 
FtAudioFIRFilterInit(const float*	filter_kernel,
					 unsigned		length);


/** Free memory associated with a FtAudioFIRFilter
 *  
 * @details release all memory allocated by FtAudioFIRFilterInit for the
 *			supplied filter.
 *
 * @param filter	FtAudioFIRFilter to free
 * @return			Error code, 0 on success
 */ 
FtAudioError_t 
FtAudioFIRFilterFree(FtAudioFIRFilter* filter);


/** Flush filter state buffer
 *  
 * @param filter	FtAudioFIRFilter to flush
 * @return			Error code, 0 on success
 */
FtAudioError_t  
FtAudioFIRFilterFlush(FtAudioFIRFilter* filter);



/** Filter a buffer of samples
 *
 * @details Uses direct form convolution to filter the samples. This is not the
 *			best method to use for filters with kernels longer than oh, say, 
 *			128 coefficients or so
 * @TODO:	Add FFT convolution implementation for longer filters
 *
 * @param filter	The FtAudioFIRFilter to use
 * @param outBuffer	The buffer to write the output to
 * @param inBuffer	The buffer to filter
 * @param n_samples The number of samples to filter
 * @return			Error code, 0 on success
 */
FtAudioError_t 
FtAudioFIRFilterProcess(FtAudioFIRFilter*	filter, 
						float*				outBuffer, 
						const float*		inBuffer, 
						unsigned			n_samples);


/** Update the filter kernel for a given filter
 *  
 * @details New kernel must be the same length as the old one!
 *
 * @param filter		The FtAudioFIRFilter to use
 * @param filter_kernel	The new filter kernel to use
 * @return			Error code, 0 on success
 */
FtAudioError_t  
FtAudioFIRFilterUpdateKernel(FtAudioFIRFilter*	filter,
							 const float*		filter_kernel);
									

#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOFIRFILTER_H_ */