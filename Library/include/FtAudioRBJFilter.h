/** 
 * @file FtAudioRBJFilter.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */
 
#ifndef FTAUDIORBJFILTER_H_
#define FTAUDIORBJFILTER_H_

#include "FtAudioError.h"
#include "FtAudioFilterTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Opaque FTA_RBJFilter structure */
typedef struct FTA_RBJFilter FTA_RBJFilter;



/** Create a new FTA_RBJFilter
 *
 * @details Allocates memory and returns an initialized FTA_RBJFilter.
 *			Play nice and call FTA_RBJFilterFree on the filter when you're
 *          done with it.
 *
 * @param type			The filter type
 * @param cutoff		The starting cutoff frequency to use
 * @param sampleRate	The sample rate in Samp/s
 * @return 				An initialized FTA_RBJFilter	
 */
FTA_RBJFilter* 
FTA_RBJFilterInit(Filter_t 	type, 
                float 			cutoff,
                long long 		sampleRate);


/** Free memory associated with a FTA_RBJFilter
 *  
 * @details release all memory allocated by FTA_RBJFilterInit for the
 *			supplied filter.
 *
 * @param filter	FTA_RBJFilter to free
 * @return			Error code, 0 on success
 */
FTA_Error_t 
FTA_RBJFilterFree(FTA_RBJFilter* 	filter);

/** Update FTA_RBJFilter type
 *  
 * @details Update the filter type and recalculate filter coefficients.
 *
 * @param filter	FTA_RBJFilter to update
 * @param type		New filter type
 * @return			Error code, 0 on success
 */
FTA_Error_t 
FTA_RBJFilterSetType(FTA_RBJFilter* 	filter, 
						Filter_t 		type);

/** Update FTA_RBJFilter Cutoff
 *  
 * @details Update the filter cutoff/center frequency and recalculate filter
 *			coefficients.
 *
 * @param filter	FTA_RBJFilter to update
 * @param cutoff	New filter cutoff/center frequency
 * @return			Error code, 0 on success
 */
FTA_Error_t 
FTA_RBJFilterSetCutoff(FTA_RBJFilter* filter,
						  float 			cutoff);

/** Update FTA_RBJFilter Q
 *  
 * @details Update the filter Q and recalculate filter coefficients.
 *
 * @param filter	FTA_RBJFilter to update
 * @param Q			New filter Q
 * @return			Error code, 0 on success
 */
FTA_Error_t 
FTA_RBJFilterSetQ(FTA_RBJFilter* 	filter, 
					 float 				Q);



/** Update FTA_RBJFilter Parameters
 *
 * @details Update the filter Q and recalculate filter coefficients.
 *
 * @param filter	FTA_RBJFilter to update
 * @param type		New filter type
 * @param cutoff	New filter cutoff/center frequency
 * @param Q			New filter Q
 * @return			Error code, 0 on success
 */
FTA_Error_t
FTA_RBJFilterSetParams(FTA_RBJFilter* filter,
                          Filter_t       type,
                          float             cutoff,
                          float             Q);
    
    
    
/** Filter a buffer of samples
 * @details Uses an RBJ-style filter to filter input samples
 *
 * @param filter	The FTA_RBJFilter to use.
 * @param outBuffer	The buffer to write the output to.
 * @param inBuffer	The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return			Error code, 0 on success
 */
FTA_Error_t
FTA_RBJFilterProcess(FTA_RBJFilter* 	filter,
						float* 				outBuffer,
						const float* 		inBuffer, 
						unsigned 			n_samples);

    
/** Flush filter state buffers
*
* @param filter    FTA_RBJFilter to flush.
* @return          Error code, 0 on success
*/
FTA_Error_t
FTA_RBJFilterFlush(FTA_RBJFilter* filter);

#ifdef __cplusplus
}
#endif

#endif /* FTAUDIORBJFILTER_H_ */
