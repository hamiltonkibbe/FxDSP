/*
 *
 *
 *
 *
 */

#include "FtAudioWaveshaper.h"
#include <stdlib.h>
#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

/* FTA_Waveshaper **********************************************************/
struct FTA_Waveshaper
{
	FTA_Upsampler* upsampler;
	//FTA_Downsampler* downsampler;
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
	float *inptr = inBuffer;
	float *outptr = outBuffer;
	while(inptr < inbuffer + n_samples)
	{
		*outptr++ = *inptr < -threshold ? -threshold : (*inptr > threshold ? threshold : *inptr);
		++inptr;
	}
#endif
	
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
	float *inptr = inBuffer;
	float *outptr = outBuffer;
	while(inptr < inBuffer + n_samples)
	{
		*outptr++ = atanf(*inptr++);
	}
#endif

}


/* FTA_WaveshaperInit ******************************************************/
 FTA_Waveshaper*
 FTA_WaveshaperInit(Waveshaper_t type)
 {
	FTA_Waveshaper* waveshaper = (FTA_Waveshaper*)malloc(sizeof(FTA_Waveshaper));
	waveshaper->upsampler = FTA_UpsamplerInit(1);
	//waveshaper->downsampler = FTA_DownsamplerInit(16X);
	waveshaper->pre = 1.0;
	waveshaper->post = 1.0;
	waveshaper->threshold = 0.7;
	waveshaper->mix = 1.0;

	switch (type)
	{
	case HARD_CLIP:
		waveshaper->shaper = &hard_clip;
		break;
	case ARCTAN:
		waveshaper->shaper = &arctan;
		break;
	default:
		waveshaper->shaper = &identity;
		break;
	}
	 
	 return waveshaper;

 }


/* FTA_WaveshaperInitFn ****************************************************/
 FTA_Waveshaper*
 FTA_WaveshaperInitFn(WaveshaperFn shaper)
 {
	FTA_Waveshaper* waveshaper = (FTA_Waveshaper*)malloc(sizeof(FTA_Waveshaper));
	waveshaper->upsampler = FTA_UpsamplerInit(1);
	//waveshaper->downsampler = FTA_DownsamplerInit(1);
	waveshaper->pre = 1.0;
	waveshaper->post = 1.0;
	waveshaper->threshold = 0.7;
	waveshaper->mix = 1.0;
	waveshaper->shaper = shaper;
	
	return waveshaper;
 }
 

/* FTA_WaveshaperFree ******************************************************/
 FTA_Error_t
 FTA_WaveshaperFree(FTA_Waveshaper* waveshaper)
 {
	FTA_UpsamplerFree(waveshaper->upsampler);
	//FTA_DownsamplerFree(waveshaper->downsampler);
	free(waveshaper);
	return FT_NOERR;
 }


/* FTA_WaveshaperProcess ***************************************************/
FTA_Error_t
FTA_WaveshaperProcess(FTA_Waveshaper	*waveshaper,
						float				*outBuffer,
						const float			*inBuffer,
						unsigned			n_samples)
{
	/* 16x oversampling */
	float inUpsampled[16 * n_samples];
	float outUpsampled[16 * n_samples];
	FTA_UpsamplerProcess(waveshaper->upsampler,(float*)inBuffer,(const float*)inUpsampled,n_samples);

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
	//FTA_DownsamplerProcess(waveshaper->downsampler,outUpsampled,outBuffer,16 * n_samples);
	return FT_NOERR;
}

