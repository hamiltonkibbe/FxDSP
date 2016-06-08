/**
 * @file RBJFilter.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef RBJFILTER_H_
#define RBJFILTER_H_

#include "Error.h"
#include "FilterTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Opaque RBJFilter structure */
typedef struct RBJFilter RBJFilter;
typedef struct RBJFilterD RBJFilterD;



/** Create a new RBJFilter
 *
 * @details Allocates memory and returns an initialized RBJFilter.
 *			Play nice and call RBJFilterFree on the filter when you're
 *          done with it.
 *
 * @param type			The filter type
 * @param cutoff		The starting cutoff frequency to use
 * @param sampleRate	The sample rate in Samp/s
 * @return 				An initialized RBJFilter
 */
RBJFilter*
RBJFilterInit(Filter_t type, float cutoff, float sampleRate);

RBJFilterD*
RBJFilterInitD(Filter_t type, double cutoff,double sampleRate);


/** Free memory associated with a RBJFilter
 *
 * @details release all memory allocated by RBJFilterInit for the
 *			supplied filter.
 *
 * @param filter	RBJFilter to free
 * @return			Error code, 0 on success
 */
Error_t
RBJFilterFree(RBJFilter* filter);

Error_t
RBJFilterFreeD(RBJFilterD* filter);


/** Update RBJFilter type
 *
 * @details Update the filter type and recalculate filter coefficients.
 *
 * @param filter	RBJFilter to update
 * @param type		New filter type
 * @return			Error code, 0 on success
 */
Error_t
RBJFilterSetType(RBJFilter* filter, Filter_t type);

Error_t
RBJFilterSetTypeD(RBJFilterD* filter, Filter_t type);


/** Update RBJFilter Cutoff
 *
 * @details Update the filter cutoff/center frequency and recalculate filter
 *			coefficients.
 *
 * @param filter	RBJFilter to update
 * @param cutoff	New filter cutoff/center frequency
 * @return			Error code, 0 on success
 */
Error_t
RBJFilterSetCutoff(RBJFilter* filter, float cutoff);

Error_t
RBJFilterSetCutoffD(RBJFilterD* filterD, double cutoff);

/** Update RBJFilter Q
 *
 * @details Update the filter Q and recalculate filter coefficients.
 *
 * @param filter	RBJFilter to update
 * @param Q			New filter Q
 * @return			Error code, 0 on success
 */
Error_t
RBJFilterSetQ(RBJFilter* filter, float Q);

Error_t
RBJFilterSetQD(RBJFilterD* filter, double Q);



/** Update RBJFilter Parameters
 *
 * @details Update the filter Q and recalculate filter coefficients.
 *
 * @param filter	RBJFilter to update
 * @param type		New filter type
 * @param cutoff	New filter cutoff/center frequency
 * @param Q			New filter Q
 * @return			Error code, 0 on success
 */
Error_t
RBJFilterSetParams(RBJFilter*   filter,
                   Filter_t     type,
                   float        cutoff,
                   float        Q);

Error_t
RBJFilterSetParamsD(RBJFilterD* filter,
                   Filter_t     type,
                   double       cutoff,
                   double       Q);



/** Filter a buffer of samples
 * @details Uses an RBJ-style filter to filter input samples
 *
 * @param filter	The RBJFilter to use.
 * @param outBuffer	The buffer to write the output to.
 * @param inBuffer	The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return			Error code, 0 on success
 */
Error_t
RBJFilterProcess(RBJFilter*     filter,
                float*          outBuffer,
                const float*    inBuffer,
                unsigned        n_samples);

Error_t
RBJFilterProcessD(RBJFilterD*   filter,
                  double*       outBuffer,
                  const double* inBuffer,
                  unsigned      n_samples);


/** Flush filter state buffers
*
* @param filter    RBJFilter to flush.
* @return          Error code, 0 on success
*/
Error_t
RBJFilterFlush(RBJFilter* filter);

Error_t
RBJFilterFlushD(RBJFilterD* filter);


#ifdef __cplusplus
}
#endif

#endif /* RBJFILTER_H_ */
