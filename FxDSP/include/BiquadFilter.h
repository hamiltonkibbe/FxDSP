/**
 * @file        BiquadFilter.h
 * @author      Hamilton Kibbe <ham@hamiltonkib.be>
 * @copyright   2014 Hamilton Kibbe. All rights reserved.
 * @brief       Biquadratic Filter
 *
 * Biquadratic filter implementation.
 *
 */

#ifndef BIQUADFILTER_H_
#define BIQUADFILTER_H_

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif


/** BiquadFilter type */
typedef struct BiquadFilter BiquadFilter;
typedef struct BiquadFilterD BiquadFilterD;


/** Create a new BiquadFilter
 *
 * @details Allocates memory and returns an initialized BiquadFilter.
 *          Play nice and call BiquadFilterFree on the filter when
 *          you're done with it.
 *
 * @param bCoeff    Numerator coefficients [b0, b1, b2]
 * @param aCoeff    Denominator coefficients [a1, a2]
 * @return          An initialized BiquadFilter
 */
BiquadFilter*
BiquadFilterInit(const float* bCoeff, const float* aCoeff);

BiquadFilterD*
BiquadFilterInitD(const double *bCoeff, const double *aCoeff);


/** Free memory associated with a BiquadFilter
 *
 * @details release all memory allocated by BiquadFilterInit for the
 *          supplied filter.
 * @param filter    BiquadFilter to free.
 * @return          Error code, 0 on success
 */
Error_t
BiquadFilterFree(BiquadFilter* filter);

Error_t
BiquadFilterFreeD(BiquadFilterD* filter);


/** Flush filter state buffers
 *
 * @param filter    BiquadFilter to flush.
 * @return          Error code, 0 on success
 */
Error_t
BiquadFilterFlush(BiquadFilter* filter);

Error_t
BiquadFilterFlushD(BiquadFilterD* filter);


/** Filter a buffer of samples
 * @details Uses a DF-II biquad implementation to filter input samples
 *
 * @param filter    The BiquadFilter to use.
 * @param outBuffer The buffer to write the output to.
 * @param inBuffer  The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return          Error code, 0 on success
 */
Error_t
BiquadFilterProcess(BiquadFilter*   filter,
                    float*          outBuffer,
                    const float*    inBuffer,
                    unsigned        n_samples);

Error_t
BiquadFilterProcessD(BiquadFilterD  *filter,
                     double         *outBuffer,
                     const double   *inBuffer,
                     unsigned       n_samples);

/** Filter a single samples
 * @details Uses a DF-II biquad implementation to filter input sample
 *
 * @param filter    The BiquadFilter to use.
 * @param in_sample The sample to process.
 * @return          Filtered sample.
 */
float
BiquadFilterTick(BiquadFilter* filter, float in_sample);

double
BiquadFilterTickD(BiquadFilterD* filter, double in_sample);


/** Update the filter kernel for a given filter
 *
 * @param filter    The filter to update
 * @param bCoeff    Numerator coefficients [b0, b1, b2]
 * @param aCoeff    Denominator coefficients [a1, a2]
 */
Error_t
BiquadFilterUpdateKernel(BiquadFilter*  filter,
                         const float*   bCoeff,
                         const float*   aCoeff);

Error_t
BiquadFilterUpdateKernelD(BiquadFilterD *filter,
                          const double  *bCoeff,
                          const double  *aCoeff);

#ifdef __cplusplus
}
#endif

#endif /* BIQUADFILTER_H_ */