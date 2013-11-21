/*
 *  FtAudioWaveshaper.h
 *  c
 *
 *  Created by Hamilton Kibbe on 6/24/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#ifndef FTAUDIOWAVESHAPER_H_
#define FTAUDIOWAVESHAPER_H_


#include <math.h>

#include "FtAudioError.h"
#include "FtAudioFIRFilter.h"
#include "FtAudioUpsampler.h"

#ifdef __cplusplus
extern "C" {
#endif

	
typedef void(*WaveshaperFn)(float*, const float*, float, unsigned);

/** Waveshaper Function */
typedef enum Waveshaper_t
{
	/** Hard clipping */
	HARD_CLIP,
	
	/** Soft clipping */
	SOFT_CLIP,
	
	/** Arctan */
	ARCTAN,

	/** Number of waveshaping functions */
	NUMBER_OF_WAVESHAPING_FUNCTIONS

} Waveshaper_t;

	
	
static void
identity(float* outBuffer, 
		 const float* inBuffer, 
		 float threshold,  
		 unsigned n_samples);


static void
hard_clip(float* outBuffer,
		  const float* inBuffer, 
		  float threshold, 
		  unsigned n_samples);

static void
arctan(float* outBuffer,
	   const float* inBuffer,
	   float threshold, 
	   unsigned n_samples);
	
	
	
	
/** Opaque FtAudioWaveshaper structure */
typedef struct FtAudioWaveshaper FtAudioWaveshaper;


/** Create a new FtAudioFIRFilter
 *
 * @details Allocates memory and returns an initialized FtAudioWaveshaper.
 *			Play nice and call FtAudioWaveshaperFree on it when you're
 *          done with it.
 * @param type			The waveshaping function to use
 */
FtAudioWaveshaper* 
FtAudioWaveshaperInit(Waveshaper_t type);
	
	
FtAudioWaveshaper*
FtAudioWaveshaperInitFn(WaveshaperFn shape);
	

/** Free memory associated with a FtAudioWaveshaper
 *  
 * @details release all memory allocated by FtAudioWaveshaperInit for the
 *			supplied waveshaper.
 * @param waveshaper	FtAudioWaveshaper to free.
 * @return				Error code, 0 on success
 */
FtAudioError_t
FtAudioWaveshaperFree(FtAudioWaveshaper* waveshaper);



FtAudioError_t
FtAudioWaveshaperProcess(FtAudioWaveshaper* waveshaper,
						 float* outBuffer,
						 const float* inBuffer,
						 unsigned n_samples);





#ifdef __cplusplus
}		/* extern "C" */
#endif

#endif /* FTAUDIOWAVESHAPER_H_ */