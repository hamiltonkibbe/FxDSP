/*
 *
 *
 *
 *
 */

#include "FtAudioWaveshaper.h"
 

struct FtAudioWaveshaper
{
	FtAudioUpsampler* upsampler;
	//FtAudioDownsampler* downsampler;
	float pre;
	float post;
	float threshold;
	float mix;
	WaveshaperFn shaper;
};


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



 FtAudioWaveshaper*
 FtAudioWaveshaperInit(Waveshaper_t type)
 {
	FtAudioWaveshaper* waveshaper = (FtAudioWaveshaper*)malloc(sizeof(FtAudioWaveshaper));
	waveshaper->upsampler = FtAudioUpsamplerInit(1);
	//waveshaper->downsampler = FtAudioDownsamplerInit(16X);
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

 FtAudioWaveshaper*
 FtAudioWaveshaperInitFn(WaveshaperFn shaper)
 {
	FtAudioWaveshaper* waveshaper = (FtAudioWaveshaper*)malloc(sizeof(FtAudioWaveshaper));
	waveshaper->upsampler = FtAudioUpsamplerInit(1);
	//waveshaper->downsampler = FtAudioDownsamplerInit(1);
	waveshaper->pre = 1.0;
	waveshaper->post = 1.0;
	waveshaper->threshold = 0.7;
	waveshaper->mix = 1.0;
	waveshaper->shaper = shaper;
	
	return waveshaper;
 }
 
 FtAudioError_t
 FtAudioWaveshaperFree(FtAudioWaveshaper* waveshaper)
 {
	FtAudioUpsamplerFree(waveshaper->upsampler);
	//FtAudioDownsamplerFree(waveshaper->downsampler);
	free(waveshaper);
	return FT_NOERR;
 }

FtAudioError_t
FtAudioWaveshaperProcess(FtAudioWaveshaper* waveshaper,
						float* outBuffer,
						const float* inBuffer,
						unsigned n_samples)
{
	/* 16x oversampling */
	float inUpsampled[16 * n_samples];
	float outUpsampled[16 * n_samples];
	FtAudioUpsamplerProcess(waveshaper->upsampler,inBuffer,(const float*)inUpsampled,n_samples);

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
	//FtAudioDownsamplerProcess(waveshaper->downsampler,outUpsampled,outBuffer,16 * n_samples);
	return FT_NOERR;
}