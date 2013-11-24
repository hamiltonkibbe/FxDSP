/** 
 * @file FtAudioUpsampler.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef FTAUDIOUPSAMPLER_H_
#define FTAUDIOUPSAMPLER_H_

#include "FtAudioError.h"
#include "FtAudioFIRFilter.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Opaque Upsampler object */
typedef struct FTA_Upsampler FTA_Upsampler;

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
}FTA_UpsampleFactor_t;


/** Create a new FTA_Upsampler
 *
 * @details Allocates memory and returns an initialized FTA_Upsampler with
 *          a given upsampling factor. Play nice and call FTA_UpsamplerFree
 *          on the filter whenyou're done with it.
 *
 * @param factor    Upsampling factor
 * @return          An initialized FTA_Upsampler
 */
FTA_Upsampler*
FTA_UpsamplerInit(FTA_UpsampleFactor_t factor);


/** Free memory associated with a FTA_Upsampler
 *  
 * @details release all memory allocated by FTA_Upsampler for the
 *          supplied filter.
 * @param upsampler FTA_Upsampler to free.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_UpsamplerFree(FTA_Upsampler* upsampler);


/** Flush upsampler state buffers
 *  
 * @param upsampler FTA_Upsampler to flush.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_UpsamplerFlush(FTA_Upsampler* upsampler);


/** Upsample a buffer of samples
 *
 * @details Upsamples given buffer using sinc interpolation
 *
 * @param upsampler The FTA_Upsampler to use
 * @param outBuffer The buffer to write the output to
 * @param inBuffer  The buffer to filter
 * @param n_samples The number of samples to upsample
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_UpsamplerProcess(FTA_Upsampler* upsampler, 
                        float* outBuffer, 
                        const float* inBuffer, 
                        unsigned n_samples);
                        
#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOUPSAMPLER_H_ */
