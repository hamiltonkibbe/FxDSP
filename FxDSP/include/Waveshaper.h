/*
 *  Waveshaper.h
 *  c
 *
 *  Created by Hamilton Kibbe on 6/24/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#ifndef WAVESHAPER_H_
#define WAVESHAPER_H_


#include <math.h>

#include "Error.h"
#include "FIRFilter.h"
#include "Upsampler.h"

#ifdef __cplusplus
extern "C" {
#endif

	
typedef void(*WaveshaperFn)(float*, const float*, float, unsigned);

/** Waveshaper Function */
typedef enum Waveshaper_t
{
    /** Identity Function */
    IDENTITY,
    
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
soft_clip(float*        outBuffer,
          const float*  inBuffer,
          float         threshold,
          unsigned      n_samples);
    
static void
arctan(float* outBuffer,
	   const float* inBuffer,
	   float threshold, 
	   unsigned n_samples);
	


WaveshaperFn shapers[NUMBER_OF_WAVESHAPING_FUNCTIONS] =
{
    &identity,
    &hard_clip,
    &soft_clip,
    &arctan
};
	
	
/** Opaque Waveshaper structure */
typedef struct Waveshaper Waveshaper;


/** Create a new FIRFilter
 *
 * @details Allocates memory and returns an initialized Waveshaper.
 *			Play nice and call WaveshaperFree on it when you're
 *          done with it.
 * @param type			The waveshaping function to use
 */
Waveshaper* 
WaveshaperInit(Waveshaper_t type);
	
	
Waveshaper*
WaveshaperInitFn(WaveshaperFn shape);
	

/** Free memory associated with a Waveshaper
 *  
 * @details release all memory allocated by WaveshaperInit for the
 *			supplied waveshaper.
 * @param waveshaper	Waveshaper to free.
 * @return				Error code, 0 on success
 */
Error_t
WaveshaperFree(Waveshaper* waveshaper);



Error_t
WaveshaperProcess(Waveshaper* waveshaper,
						 float* outBuffer,
						 const float* inBuffer,
						 unsigned n_samples);





#ifdef __cplusplus
}		/* extern "C" */
#endif

#endif /* WAVESHAPER_H_ */