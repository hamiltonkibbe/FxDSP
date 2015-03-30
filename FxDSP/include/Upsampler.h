/** 
 * @file Upsampler.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef UPSAMPLER_H_
#define UPSAMPLER_H_

#include "Error.h"
#include "FIRFilter.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Opaque Upsampler object */
typedef struct Upsampler Upsampler;

/** Upsampling Factor constants */
typedef enum factors
{
    /** 4x upsampling */
    UP_4X,
    
    /** 8x upsampling */
    UP_8X,
    
    /** 16x upsampling */
    UP_16X,
    
    /** number of upsampling factors */
    N_FACTORS
}UpsampleFactor_t;


/** Create a new Upsampler
 *
 * @details Allocates memory and returns an initialized Upsampler with
 *          a given upsampling factor. Play nice and call UpsamplerFree
 *          on the filter whenyou're done with it.
 *
 * @param factor    Upsampling factor
 * @return          An initialized Upsampler
 */
Upsampler*
UpsamplerInit(UpsampleFactor_t factor);


/** Free memory associated with a Upsampler
 *  
 * @details release all memory allocated by Upsampler for the
 *          supplied filter.
 * @param upsampler Upsampler to free.
 * @return          Error code, 0 on success
 */
Error_t
UpsamplerFree(Upsampler* upsampler);


/** Flush upsampler state buffers
 *  
 * @param upsampler Upsampler to flush.
 * @return          Error code, 0 on success
 */
Error_t
UpsamplerFlush(Upsampler* upsampler);


/** Upsample a buffer of samples
 *
 * @details Upsamples given buffer using sinc interpolation
 *
 * @param upsampler The Upsampler to use
 * @param outBuffer The buffer to write the output to
 * @param inBuffer  The buffer to filter
 * @param n_samples The number of samples to upsample
 * @return          Error code, 0 on success
 */
Error_t
UpsamplerProcess(Upsampler*     upsampler,
                 float*         outBuffer,
                 const float*   inBuffer,
                 unsigned       n_samples);
                        
#ifdef __cplusplus
}
#endif

#endif /* UPSAMPLER_H_ */
