//
//  FtAudioLinkwitzRileyFilter.h
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#ifndef FutureTechAudio_FtAudioLinkwitzRileyFilter_h
#define FutureTechAudio_FtAudioLinkwitzRileyFilter_h

#include "FtAudioError.h"
#include "FtAudioFilterTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    
    
/** Linkwitz-Riley filter type */
typedef struct FTA_LRFilter FTA_LRFilter;


/** Create a new FTA_LRFilter
 *
 * @details Allocates memory and returns an initialized FTA_LRFilter.
 *			Play nice and call FTA_LRFilterFree on the filter when you're
 *          done with it.
 *
 * @param type			The filter type
 * @param cutoff		The starting cutoff frequency to use
 * @param Q             The starting Q to use
 * @param sampleRate	The sample rate in Samp/s
 * @return 				An initialized FTA_LRFilterFilter
 */
FTA_LRFilter*
FTA_LRFilterInit(Filter_t 	type,
                 float 		cutoff,
                 float      Q,
                 long long 	sampleRate);


/** Free memory associated with a FTA_LRFilter
 *
 * @details release all memory allocated by FTA_LRFilterInit for the
 *			supplied filter.
 *
 * @param filter	FTA_LRFilter to free
 * @return			Error code, 0 on success
 */
FTA_Error_t 
FTA_LRFilterFree(FTA_LRFilter* 	filter);

    

/** Flush filter state buffers
 *
 * @param filter    FTA_LRFilter to flush.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_LRFilterFlush(FTA_LRFilter* filter);
   
    
    
/** Update FTA_LRFilter Parameters
 *
 * @details Update the filter Q and recalculate filter coefficients.
 *
 * @param filter	FTA_LRFilter to update
 * @param type		New filter type
 * @param cutoff	New filter cutoff/center frequency
 * @param Q			New filter Q
 * @return			Error code, 0 on success
 */
FTA_Error_t
FTA_LRFilterSetParams(FTA_LRFilter* filter,
                      Filter_t  type,
                      float     cutoff,
                      float     Q);



/** Filter a buffer of samples
 * @details Filter samples
 *
 * @param filter	The FTA_LRFilter to use.
 * @param outBuffer	The buffer to write the output to.
 * @param inBuffer	The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return			Error code, 0 on success
 */
FTA_Error_t
FTA_LRFilterProcess(FTA_LRFilter*   filter,
                    float*          outBuffer,
                    const float* 	inBuffer,
                    unsigned 		n_samples);



    
    
    
#ifdef __cplusplus
}
#endif
    
#endif
