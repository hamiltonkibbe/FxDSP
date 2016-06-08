/**
 * @file        LinkwitzRileyFilter.h
 * @author      Hamilton Kibbe <ham@hamiltonkib.be>
 * @copyright   2014 Hamilton Kibbe. All rights reserved.
 * @brief       Linkwitz-Riley crossover filter
 *
 * A Linkwitz-Riley crossover filter implementation. Used for splitting signals
 * into multiple frequency bands so they may be processed independently.
 *
 */

#ifndef FxDSP_LinkwitzRileyFilter_h
#define FxDSP_LinkwitzRileyFilter_h

#include "Error.h"
#include "FilterTypes.h"

#ifdef __cplusplus
extern "C" {
#endif



/** Linkwitz-Riley filter type */
typedef struct LRFilter LRFilter;
typedef struct LRFilterD LRFilterD;


/** Create a new LRFilter
 *
 * @details Allocates memory and returns an initialized LRFilter.
 *			Play nice and call LRFilterFree on the filter when you're
 *          done with it.
 *
 * @param type			The filter type
 * @param cutoff		The starting cutoff frequency to use
 * @param Q             The starting Q to use
 * @param sampleRate	The sample rate in Samp/s
 * @return 				An initialized LRFilterFilter
 */
LRFilter*
LRFilterInit(Filter_t   type,
             float 		cutoff,
             float      Q,
             float      sampleRate);

LRFilterD*
LRFilterInitD(Filter_t  type,
              double 	cutoff,
              double    Q,
              double    sampleRate);


/** Free memory associated with a LRFilter
 *
 * @details release all memory allocated by LRFilterInit for the
 *			supplied filter.
 *
 * @param filter	LRFilter to free
 * @return			Error code, 0 on success
 */
Error_t
LRFilterFree(LRFilter* filter);

Error_t
LRFilterFreeD(LRFilterD* filter);


/** Flush filter state buffers
 *
 * @param filter    LRFilter to flush.
 * @return          Error code, 0 on success
 */
Error_t
LRFilterFlush(LRFilter* filter);

Error_t
LRFilterFlushD(LRFilterD* filter);


/** Update LRFilter Parameters
 *
 * @details Update the filter Q and recalculate filter coefficients.
 *
 * @param filter	LRFilter to update
 * @param type		New filter type
 * @param cutoff	New filter cutoff/center frequency
 * @param Q			New filter Q
 * @return			Error code, 0 on success
 */
Error_t
LRFilterSetParams(LRFilter* filter,
                      Filter_t  type,
                      float     cutoff,
                      float     Q);

Error_t
LRFilterSetParamsD(LRFilterD*   filter,
                   Filter_t      type,
                   double        cutoff,
                   double        Q);


/** Filter a buffer of samples
 * @details Filter samples
 *
 * @param filter	The LRFilter to use.
 * @param outBuffer	The buffer to write the output to.
 * @param inBuffer	The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return			Error code, 0 on success
 */
Error_t
LRFilterProcess(LRFilter*   filter,
                    float*          outBuffer,
                    const float* 	inBuffer,
                    unsigned 		n_samples);

Error_t
LRFilterProcessD(LRFilterD*     filter,
                 double*        outBuffer,
                 const double*  inBuffer,
                 unsigned 		n_samples);


#ifdef __cplusplus
}
#endif

#endif
