/** 
 * @file FtAudioBiquadFilter.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */


#ifndef FTAUDIOBIQUADFILTER_H_
#define FTAUDIOBIQUADFILTER_H_

#include "FtAudioError.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Opaque FTA_BiquadFilter structure */
typedef struct FTA_BiquadFilter FTA_BiquadFilter;


/** Create a new FTA_BiquadFilter
 *
 * @details Allocates memory and returns an initialized FTA_BiquadFilter.
 *          Play nice and call FTA_BiquadFilterFree on the filter when 
 *          you're done with it.
 *
 * @param bCoeff    Numerator coefficients (b0, b1, b2).
 * @param aCoeff    Denominator coefficients (a1, a2).
 * @return          An initialized FTA_BiquadFilter.
 */
FTA_BiquadFilter* 
FTA_BiquadFilterInit(const float* bCoeff, 
                        const float* aCoeff);


/** Free memory associated with a FTA_BiquadFilter
 *  
 * @details release all memory allocated by FTA_BiquadFilterInit for the
 *          supplied filter.
 * @param filter    FTA_BiquadFilter to free.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_BiquadFilterFree(FTA_BiquadFilter* filter);


/** Flush filter state buffers
 *  
 * @param filter    FTA_BiquadFilter to flush.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_BiquadFilterFlush(FTA_BiquadFilter* filter);



/** Filter a buffer of samples
 * @details Uses a DF-I biquad implementation to filter input samples
 *
 * @param filter    The FTA_BiquadFilter to use.
 * @param outBuffer The buffer to write the output to.
 * @param inBuffer  The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_BiquadFilterProcess(FTA_BiquadFilter* filter, 
                           float*               outBuffer, 
                           const float*         inBuffer, 
                           unsigned             n_samples);


/** Update the filter kernel for a given filter
 *
 * @param filter    The filter to update
 * @param bCoeff    Numerator coefficients
 * @param aCoeff    Denominator coefficients
 */
FTA_Error_t
FTA_BiquadFilterUpdateKernel(FTA_BiquadFilter*    filter,
                                const float*            bCoeff,
                                const float*            aCoeff);


#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOBIQUADFILTER_H_ */