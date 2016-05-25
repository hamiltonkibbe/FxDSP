/**
 * @file FIRFilter.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef FIRFILTER_H_
#define FIRFILTER_H_

#include <string.h>
#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif

/** The kernel length at which to use FFT convolution vs direct */
/* So this is a pretty good value for now */
#define USE_FFT_CONVOLUTION_LENGTH (128)


/** Convolution Algorithm to use */
typedef enum _ConvolutionMode
{
    /** Choose the best algorithm based on filter size */
    BEST    = 0,

    /** Use direct convolution */
    DIRECT  = 1,

    /** Use FFT Convolution (Better for longer filter kernels */
    FFT     = 2

} ConvolutionMode_t;


/** FIRFilter type */
typedef struct FIRFilter FIRFilter;
typedef struct FIRFilterD FIRFilterD;


/** Create a new FIRFilter
 *
 * @details Allocates memory and returns an initialized FIRFilter.
 *			Play nice and call FIRFilterFree on the filter when you're
 *          done with it.
 *
 * @param filter_kernel     The filter coefficients. These are copied to the
 *                          filter so there is no need to keep them around.
 * @param length            The number of coefficients in filter_kernel.
 * @param convolution_mode  Convolution algorithm. Either BEST, FFT, or DIRECT.
 * @return                  An initialized FIRFilter
 */
FIRFilter*
FIRFilterInit(const float*      filter_kernel,
              unsigned          length,
              ConvolutionMode_t convolution_mode);
FIRFilterD*
FIRFilterInitD(const double*        filter_kernel,
               unsigned             length,
               ConvolutionMode_t    convolution_mode);


/** Free memory associated with a FIRFilter
 *
 * @details release all memory allocated by FIRFilterInit for the
 *			supplied filter.
 *
 * @param filter	FIRFilter to free
 * @return			Error code, 0 on success
 */
Error_t
FIRFilterFree(FIRFilter* filter);

Error_t
FIRFilterFreeD(FIRFilterD* filter);


/** Flush filter state buffer
 *
 * @param filter	FIRFilter to flush
 * @return			Error code, 0 on success
 */
Error_t
FIRFilterFlush(FIRFilter* filter);

Error_t
FIRFilterFlushD(FIRFilterD* filter);


/** Filter a buffer of samples
 *
 * @details Uses either FFT or direct-form convolution to filter the samples.
 *
 * @param filter	The FIRFilter to use
 * @param outBuffer	The buffer to write the output to
 * @param inBuffer	The buffer to filter
 * @param n_samples The number of samples to filter
 * @return			Error code, 0 on success
 */
Error_t
FIRFilterProcess(FIRFilter*     filter,
                 float*         outBuffer,
                 const float*   inBuffer,
				 unsigned       n_samples);


Error_t
FIRFilterProcessD(FIRFilterD*   filter,
                  double*       outBuffer,
                  const double* inBuffer,
                  unsigned      n_samples);


/** Update the filter kernel for a given filter
 *
 * @details New kernel must be the same length as the old one!
 *
 * @param filter		The FIRFilter to use
 * @param filter_kernel	The new filter kernel to use
 * @return			Error code, 0 on success
 */
Error_t
FIRFilterUpdateKernel(FIRFilter*    filter,
					  const float*  filter_kernel);

Error_t
FIRFilterUpdateKernelD(FIRFilterD*    filter,
                       const double*  filter_kernel);


#ifdef __cplusplus
}
#endif

#endif /* FIRFILTER_H_ */
