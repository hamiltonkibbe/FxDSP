/*
 *
 *
 *
 *
 */

#include "Waveshaper.h"
#include <stdlib.h>
#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

/* Waveshaper **********************************************************/
struct Waveshaper
{
	Upsampler* upsampler;
	//Downsampler* downsampler;
	float pre;
	float post;
	float threshold;
	float mix;
	WaveshaperFn shaper;
};


/* identity *******************************************************************/
static void
identity(float* outBuffer, 
		const float* inBuffer, 
		float threshold,  
		unsigned n_samples)
{
	float *inptr = (float*)inBuffer;
	float *outptr = outBuffer;
	while(inptr < inBuffer + n_samples)
	{
		*outptr++ = *inptr++;
	}
}

/* hard_clip ******************************************************************/
static void
hard_clip(float* outBuffer,
		const float* inBuffer, 
		float threshold, 
		unsigned n_samples)
{
#ifdef __APPLE__
	float high = 1.0 * threshold;
	float low = -1.0 * high;
	
		vDSP_vclip((float*)inBuffer,1,&low,&high,outBuffer,1,n_samples);
	
#else
	float *inptr = (float*)inBuffer;
	float *outptr = outBuffer;
	while(inptr < inBuffer + n_samples)
	{
		*outptr++ = *inptr < -threshold ? -threshold : (*inptr > threshold ? threshold : *inptr);
		++inptr;
	}
#endif
	
}

/* hard_clip ******************************************************************/
static void
soft_clip(float* outBuffer,
          const float* inBuffer,
          float threshold,
          unsigned n_samples)
{
    //TODO
}

/* arctan *********************************************************************/
static void
arctan(float* outBuffer,
		const float* inBuffer,
		float threshold, 
		unsigned n_samples)
{
#ifdef __APPLE__
	float four_over_pi = 4/M_PI;
	vvatanf(outBuffer, inBuffer,(const int*)&n_samples);
	vDSP_vsmul(outBuffer, 1, (const float*)&four_over_pi, outBuffer, 1, n_samples);
#else
	float *inptr = (float*)(float*)(float*)(float*)(float*)(float*)(float*)(float*)inBuffer;
	float *outptr = outBuffer;
	while(inptr < inBuffer + n_samples)
	{
		*outptr++ = atanf(*inptr++);
	}
#endif

}


/* WaveshaperInit ******************************************************/
 Waveshaper*
 WaveshaperInit(Waveshaper_t type)
 {
	Waveshaper* waveshaper = (Waveshaper*)malloc(sizeof(Waveshaper));
	waveshaper->upsampler = UpsamplerInit(1);
	//waveshaper->downsampler = DownsamplerInit(16X);
	waveshaper->pre = 1.0;
	waveshaper->post = 1.0;
	waveshaper->threshold = 0.7;
	waveshaper->mix = 1.0;

    waveshaper->shaper = shapers[type];
	 
    return waveshaper;

 }


/* WaveshaperInitFn ****************************************************/
 Waveshaper*
 WaveshaperInitFn(WaveshaperFn shaper)
 {
	Waveshaper* waveshaper = (Waveshaper*)malloc(sizeof(Waveshaper));
	waveshaper->upsampler = UpsamplerInit(1);
	//waveshaper->downsampler = DownsamplerInit(1);
	waveshaper->pre = 1.0;
	waveshaper->post = 1.0;
	waveshaper->threshold = 0.7;
	waveshaper->mix = 1.0;
	waveshaper->shaper = shaper;
	
	return waveshaper;
 }
 

/* WaveshaperFree ******************************************************/
 Error_t
 WaveshaperFree(Waveshaper* waveshaper)
 {
	UpsamplerFree(waveshaper->upsampler);
	//DownsamplerFree(waveshaper->downsampler);
    if (waveshaper)
    {
        free(waveshaper);
        waveshaper = NULL;
    }
	return NOERR;
 }


/* WaveshaperProcess ***************************************************/
Error_t
WaveshaperProcess(Waveshaper	*waveshaper,
                  float         *outBuffer,
                  const float   *inBuffer,
                  unsigned      n_samples)
{
	/* 16x oversampling */
	float inUpsampled[16 * n_samples];
	float outUpsampled[16 * n_samples];
	UpsamplerProcess(waveshaper->upsampler,(float*)inBuffer,(const float*)inUpsampled,n_samples);

/* Apple Accelerate Framework implementation */
#ifdef __APPLE__
	/* pre-gain */
	vDSP_vsmul(inUpsampled, 1, (const float*)&waveshaper->pre, outUpsampled, 1, 16 * n_samples);

	/* waveshaper */
	(*waveshaper->shaper)(outUpsampled,(const float*)outUpsampled, 0.7, 16 * n_samples);

	/* post-gain */
	vDSP_vsmul(outUpsampled, 1, (const float*)&waveshaper->post, outUpsampled, 1, 16 * n_samples);

/* canonical implementation */
#else

	/* TODO... */

#endif

	/* downsample to original Fs */
	//DownsamplerProcess(waveshaper->downsampler,outUpsampled,outBuffer,16 * n_samples);
	return NOERR;
}

