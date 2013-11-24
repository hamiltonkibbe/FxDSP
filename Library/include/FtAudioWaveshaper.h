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
	
	
	
	
/** Opaque FTA_Waveshaper structure */
typedef struct FTA_Waveshaper FTA_Waveshaper;


/** Create a new FTA_FIRFilter
 *
 * @details Allocates memory and returns an initialized FTA_Waveshaper.
 *			Play nice and call FTA_WaveshaperFree on it when you're
 *          done with it.
 * @param type			The waveshaping function to use
 */
FTA_Waveshaper* 
FTA_WaveshaperInit(Waveshaper_t type);
	
	
FTA_Waveshaper*
FTA_WaveshaperInitFn(WaveshaperFn shape);
	

/** Free memory associated with a FTA_Waveshaper
 *  
 * @details release all memory allocated by FTA_WaveshaperInit for the
 *			supplied waveshaper.
 * @param waveshaper	FTA_Waveshaper to free.
 * @return				Error code, 0 on success
 */
FTA_Error_t
FTA_WaveshaperFree(FTA_Waveshaper* waveshaper);



FTA_Error_t
FTA_WaveshaperProcess(FTA_Waveshaper* waveshaper,
						 float* outBuffer,
						 const float* inBuffer,
						 unsigned n_samples);





#ifdef __cplusplus
}		/* extern "C" */
#endif

#endif /* FTAUDIOWAVESHAPER_H_ */