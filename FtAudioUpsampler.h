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
typedef struct FtAudioUpsampler FtAudioUpsampler;

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
}FtAudioUpsampleFactor_t;


/** Create a new FtAudioUpsampler
 *
 * @details Allocates memory and returns an initialized FtAudioUpsampler with
 *			a given upsampling factor. Play nice and call FtAudioUpsamplerFree
 *			on the filter whenyou're done with it.
 *
 * @param factor	Upsampling factor
 * @return			An initialized FtAudioUpsampler
 */
FtAudioUpsampler*
FtAudioUpsamplerInit(FtAudioUpsampleFactor_t factor);


/** Free memory associated with a FtAudioUpsampler
 *  
 * @details release all memory allocated by FtAudioUpsampler for the
 *			supplied filter.
 * @param upsampler	FtAudioUpsampler to free.
 * @return			Error code, 0 on success
 */
FtAudioError_t
FtAudioUpsamplerFree(FtAudioUpsampler* upsampler);


/** Flush upsampler state buffers
 *  
 * @param upsampler	FtAudioUpsampler to flush.
 * @return			Error code, 0 on success
 */
FtAudioError_t
FtAudioUpsamplerFlush(FtAudioUpsampler* upsampler);


/** Upsample a buffer of samples
 *
 * @details Upsamples given buffer using sinc interpolation
 *
 * @param upsampler	The FtAudioUpsampler to use
 * @param outBuffer	The buffer to write the output to
 * @param inBuffer	The buffer to filter
 * @param n_samples The number of samples to upsample
 * @return			Error code, 0 on success
 */
FtAudioError_t
FtAudioUpsamplerProcess(FtAudioUpsampler* upsampler, 
						float* outBuffer, 
						const float* inBuffer, 
						unsigned n_samples);
						
#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOUPSAMPLER_H_ */
