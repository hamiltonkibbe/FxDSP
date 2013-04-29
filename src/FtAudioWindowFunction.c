/*
 *  FtAudioWindowFunction.c
 *  c
 *
 *  Created by Hamilton Kibbe on 6/9/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioWindowFunction.h"

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

struct FtAudioWindowFunction
{
	float*		window;
	unsigned	length;
	FtWindow_t	type;
};

/* Boxcar window  */
static FtAudioError_t 
boxcar(unsigned n, float* dest)
{
	float one = 1.0;
	vDSP_vfill(&one, dest, 1, n);
	return FT_NOERR;
}

/* Hann window  */
static FtAudioError_t 
hann(unsigned n, float* dest)
{

#ifdef __APPLE__
	// Use the accelerate version if we have it
	vDSP_hann_window(dest, n, vDSP_HANN_NORM);

#else
	// Otherwise do it manually
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		*dest++ = 0.5 * (1 - cosf((2 * M_PI * buf_idx) / (n - 1)));
	}

#endif // __APPLE__
	return FT_NOERR;
}

/* Hamming window  */
static FtAudioError_t 
hamming(unsigned n, float* dest)
{

#ifdef __APPLE__
	// Use the accelerate version if we have it
	vDSP_hamm_window(dest, n);

#else
	// Otherwise do it manually
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		*dest++ = 0.54 - 0.46 * cosf((2 * M_PI * buf_idx) / (n - 1));
	}

#endif // __APPLE__
	return FT_NOERR;
}

/* Blackman window  */
static FtAudioError_t 
blackman(unsigned n, float a, float* dest)
{
#ifdef __APPLE__
	// Use the builtin version for the specific case it implements, if it is
	// available.
	if (a == 0.16)
	{
		vDSP_blkman_window(dest, n);
	}

#else
	// Otherwise do it manually
	float a0 = (1 - a) / 2;
	float a1 = 0.5;
	float a2 = a / 2;
	
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		*dest++ = a0 - a1 * cosf((2 * M_PI * buf_idx) / (n - 1)) + a2 * cosf((4 * M_PI * buf_idx) / (n - 1));
	}
#endif // __APPLE__
	return FT_NOERR;
}

/* Tukey window  */
static FtAudioError_t 
tukey(unsigned n, float a, float* dest)
{
	float term = a * (n - 1) / 2;
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		if (buf_idx <= term)
		{
			dest[buf_idx] = 0.5 * (1 + cosf(M_PI * ((2 * buf_idx) / (a * (n - 1)) - 1))); 
		}
		else if (term <= buf_idx && buf_idx <= (n - 1) * (1 - a / 2))
		{
			dest[buf_idx] = 1.0;
		}
		else
		{
			dest[buf_idx] = 0.5 * (1 + cosf(M_PI * ((2 * buf_idx) / (a * (n - 1)) - (2 / a) + 1))); 	
		}
				
	}
	return FT_NOERR;
}

/* Cosine window  */
static FtAudioError_t 
cosine(unsigned n, float* dest)
{
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		*dest++ = sinf((M_PI * buf_idx) / (n - 1));
	}
	return FT_NOERR;
}

/* Lanczos window  */
static FtAudioError_t 
lanczos(unsigned n, float* dest)
{
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		*dest++ = sinf(M_PI * ((2 * n) / (n - 1) - 1)) / (M_PI * ((2 * n) / (n - 1) - 1));
	}
	return FT_NOERR;
}

/* Bartlett window  */
static FtAudioError_t 
bartlett(unsigned n, float* dest)
{
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		if (buf_idx <= (n - 1) / 2)
		{
			*dest++ = (2 * buf_idx)/(n - 1);
		}
		else
		{
			*dest ++ = 2 - (2 * buf_idx) / (n - 1);
		}
	}
	return FT_NOERR;
}

/* Gaussian window  */
static FtAudioError_t 
gaussian(unsigned n, float sigma, float* dest)
{
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		*dest++ = expf(-0.5 * powf((buf_idx - ((n - 1) / 2)) / (sigma * ((n - 1) / 2)),2));
	}
	return FT_NOERR;
}

/* Bartlett-Hann window  */
static FtAudioError_t 
bartlett_hann(unsigned n, float* dest)
{
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{	
		*dest++ = 0.62 - 0.48 * fabs((buf_idx / (n - 1)) - 0.5) - 0.38 * cosf((2 * M_PI * buf_idx) / (n - 1));
	}
	return FT_NOERR;
}

/* Kaiser Window */
static FtAudioError_t 
kaiser(unsigned n, float a, float* dest)
{
	// Pre-calc
	float beta = M_PI * a;
	float m_2 = (float)(n-1) / 2.0;
	float denom = modZeroBessel(beta);
	
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		float val = ((buf_idx) - m_2) / m_2;
		val = 1 - (val * val);
		*dest++ = modZeroBessel(beta * sqrt(val)) / denom;
	}
	return FT_NOERR;
}


/* Nuttall window  */
static FtAudioError_t 
nuttall(unsigned n, float* dest)
{
	float term;
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		term = (2 * M_PI * buf_idx) / (n - 1);
		*dest++ = 0.355768 - 0.487396 * cosf(term)+ 0.144232 * cosf(2 * term) - 0.012604 * cosf(3 * term);
	}
	return FT_NOERR;
}


/* Blackman-Harris window  */
static FtAudioError_t 
blackman_harris(unsigned n, float* dest)
{
	float term;
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		term = (2 * M_PI * buf_idx) / (n - 1);
		*dest++ = 0.35875 - 0.48829 * cosf(term)+ 0.14128 * cosf(2 * term) - 0.01168 * cosf(3 * term);
	}
	return FT_NOERR;	
}

/* Blackman-Nuttall window  */
static FtAudioError_t 
blackman_nuttall(unsigned n, float* dest)
{
	float term;
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		term = (2 * M_PI * buf_idx) / (n - 1);
		*dest++ = 0.3635819 - 0.4891775 * cosf(term)+ 0.1365995 * cosf(2 * term) - 0.0106411 * cosf(3 * term);
	}
	return FT_NOERR;	
}

/* Flat top window */
static FtAudioError_t 
flat_top(unsigned n, float* dest)
{
	float term;
	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{
		term = (2 * M_PI * buf_idx) / (n - 1);
		*dest++ = 1 - 1.93 * cosf(term)+ 1.29 * cosf(2 * term) - 0.388 * cosf(3 * term) + 0.032 * cosf(4 * term);
	}
	return FT_NOERR;	
}

/* Poisson window */
static FtAudioError_t 
poisson(unsigned n, float D, float* dest)
{
	float term = (n - 1) / 2;
	float tau= (n / 2) * (8.69 / D);

	unsigned buf_idx;
	for (buf_idx = 0; buf_idx < n; ++buf_idx)
	{

		*dest++ = expf(-fabs(buf_idx - term) * (1 / tau));
	}
	return FT_NOERR;	
}

/* Bessel Function */
static float
modZeroBessel(float x)
{
	float x_2 = x/2;
	float num = 1;
	float fact = 1;
	float result = 1;
	
	unsigned i;
	for (i=1 ; i<20 ; i++) 
	{
		num *= x_2 * x_2;
		fact *= i;
		result += num / (fact * fact);
	}
	return result;
}



FtAudioWindowFunction*
FtAudioWindowFunctionInit(unsigned n, FtWindow_t type)
{
	FtAudioWindowFunction* window = (FtAudioWindowFunction*)malloc(sizeof(FtAudioWindowFunction));
	
	window->length = n;
	window->window = (float*)malloc(n * sizeof(float));
	window->type = type;
	
	switch (type)
	{
		case BOXCAR:
			boxcar(window->length, window->window);
			break;
		case HANN:
			hann(window->length, window->window);
			break;
		case HAMMING:
			hamming(window->length, window->window);
			break;
		case BLACKMAN:
			blackman(window->length, 0.16, window->window);
			break;
		case TUKEY:
			tukey(window->length, 0.5, window->window);
			break;
		case COSINE:
			cosine(window->length, window->window);
			break;
		case LANCZOS:
			lanczos(window->length, window->window);
			break;
		case BARTLETT:
			bartlett(window->length, window->window);
			break;
		case GAUSSIAN:
			gaussian(window->length, 0.4, window->window);
			break;
		case BARTLETT_HANN:
			bartlett_hann(window->length, window->window);
			break;
		case KAISER:
			kaiser(window->length, 3, window->window);
			break;
		case NUTTALL:
			nuttall(window->length, window->window);
			break;
		case BLACKMAN_HARRIS:
			blackman_harris(window->length, window->window);
			break;
		case BLACKMAN_NUTTALL:
			blackman_nuttall(window->length, window->window);
			break;
		case FLATTOP:
			flat_top(window->length, window->window);
			break;
		case POISSON:
			poisson(window->length, 8.69, window->window);
			
		default:
			boxcar(window->length, window->window);
			break;
	}
	
	return window;
}


FtAudioError_t
FtAudioWindowFunctionFree(FtAudioWindowFunction* window)
{
	free(window->window);
	free(window);
	return FT_NOERR;
}

FtAudioError_t
FtAudioWindowFunctionProcess(FtAudioWindowFunction* window, float* outBuffer, float* inBuffer,unsigned n_samples)
{
	vDSP_vmul(inBuffer, 1, window->window, 1, outBuffer,1, n_samples);
	return FT_NOERR;
}




