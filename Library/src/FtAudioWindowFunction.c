/*
 *  FtAudioWindowFunction.c
 *  c
 *
 *  Created by Hamilton Kibbe on 6/9/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioWindowFunction.h"
#include "FtAudioDsp.h"


#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif


/* Static Function Prototypes */
static float
modZeroBessel(float x);



/* Implementations */

struct FTA_WindowFunction
{
    float*      window;
    unsigned    length;
    FtWindow_t  type;
};

/* Boxcar window  */
FTA_Error_t 
boxcar(unsigned n, float* dest)
{
    FTA_FillBuffer(dest, n, 1.0);
    return FT_NOERR;
}

/* Hann window  */
FTA_Error_t 
hann(unsigned n, float* dest)
{

#ifdef __APPLE__
    // Use the accelerate version if we have it
    // TODO: FIX THIS!!!!!!
    vDSP_hann_window(dest, n - 1, vDSP_HANN_DENORM);

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
FTA_Error_t 
hamming(unsigned n, float* dest)
{

    // do it manually. it seems like vDSP_hamm_window is wrong.
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = 0.54 - 0.46 * cosf((2 * M_PI * buf_idx) / (n - 1));
    }


    return FT_NOERR;
}

/* Blackman window  */
FTA_Error_t 
blackman(unsigned n, float a, float* dest)
{
#ifdef __APPLE__
    // Use the builtin version for the specific case it implements, if it is
    // available.
    if (a > 0.15 && a < 0.17)
    {
        vDSP_blkman_window(dest, (n - 1), 0);
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
FTA_Error_t 
tukey(unsigned n, float a, float* dest)
{
    float term = a * (n - 1) / 2;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        if (buf_idx <= term)
        {
            dest[buf_idx] = 0.5 * (1 + cosf(M_PI * ((2 * buf_idx) /
                                                    (a * (n - 1)) - 1)));
        }
        else if (term <= buf_idx && buf_idx <= (n - 1) * (1 - a / 2))
        {
            dest[buf_idx] = 1.0;
        }
        else
        {
            dest[buf_idx] = 0.5 * (1 + cosf(M_PI *((2 * buf_idx) /
                                                   (a * (n - 1)) - (2 / a) + 1)));
        }
                
    }
    return FT_NOERR;
}

/* Cosine window  */
FTA_Error_t 
cosine(unsigned n, float* dest)
{
    float N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = sinf((M_PI * buf_idx) / N);
    }
    return FT_NOERR;
}

/* Lanczos window  */
FTA_Error_t 
lanczos(unsigned n, float* dest)
{
    float N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        float term = M_PI * ((2 * n)/ N) - 1.0;
        *dest++ = sinf(term) / term;
    }
    return FT_NOERR;
}

/* Bartlett window  */
FTA_Error_t 
bartlett(unsigned n, float* dest)
{
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        if (buf_idx <= (n - 1) / 2)
        {
            *dest++ = (float)(2 * buf_idx)/(n - 1);
        }
        else
        {
            *dest ++ = 2.0 - (float)(2 * buf_idx) / (n - 1);
        }
    }
    return FT_NOERR;
}

/* Gaussian window  */
FTA_Error_t 
gaussian(unsigned n, float sigma, float* dest)
{
    float N = n - 1;
    float L = N / 2.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {   
        *dest++ = expf(-0.5 * powf((buf_idx - L)/(sigma * L),2));
    }
    return FT_NOERR;
}

/* Bartlett-Hann window  */
FTA_Error_t 
bartlett_hann(unsigned n, float* dest)
{
    float N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        float term = ((buf_idx / N) - 0.5);
        *dest++ = 0.62 - 0.48 * fabs(term) + 0.38 * cosf(2 * M_PI * term);
    }
    return FT_NOERR;
}

/* Kaiser Window */
FTA_Error_t 
kaiser(unsigned n, float a, float* dest)
{
    // Pre-calc
    float beta = M_PI * a;
    float m_2 = (float)(n - 1.0) / 2.0;
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
FTA_Error_t 
nuttall(unsigned n, float* dest)
{
    float term;
    float N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        term = 2 * M_PI * (buf_idx / N);
        *dest++ = 0.3635819  - 0.4891775 * cosf(term) + 0.1365995 *
        cosf(2 * term) - 0.0106411 * cosf(3 * term);
    }
    return FT_NOERR;
}


/* Blackman-Harris window  */
FTA_Error_t 
blackman_harris(unsigned n, float* dest)
{
    float term;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        term = (2 * M_PI * buf_idx) / (n - 1);
        *dest++ = 0.35875 - 0.48829 * cosf(term)+ 0.14128 * cosf(2 * term) -
        0.01168 * cosf(3 * term);
    }
    return FT_NOERR;    
}

/* Blackman-Nuttall window  */
FTA_Error_t
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
FTA_Error_t
flat_top(unsigned n, float* dest)
{
    float N = n - 1.0;
    float term;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        term = (2 * M_PI * buf_idx) / N;
        *dest++ = 0.21557895 - 0.41663158 * cosf(term)+ 0.277263158 *
        cosf(2 * term) - 0.083578947 * cosf(3 * term) + 0.006947368 *
        cosf(4 * term);
    }
    return FT_NOERR;    
}

/* Poisson window */
FTA_Error_t 
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

// TODO: FIX This.
FTA_Error_t
chebyshev(unsigned n, float A, float *dest)
{
    float max=0;
    float N = n - 1.0;
    float M = N / 2;
    if(n % 2 == 0)
    {
        M = M + 0.5; /* handle even length windows */
    }
    float tg = powf(10, A / 20.0);
    float x0 = coshf((1.0 / N) * acoshf(tg));
    
    unsigned buf_idx;
    for(buf_idx=0; buf_idx<(n/2+1); ++buf_idx)
    {
        //float y = buf_idx - M;
        float sum = 0;
        for(unsigned i=1; i<=M; i++){
            sum += chebyshev_poly(N, x0 * cosf(M_PI * i / n)) *
            cosf( 2.0 * n * M_PI * i / n);
        }
        dest[buf_idx] = tg + 2 * sum;
        dest[(unsigned)N - buf_idx] = dest[buf_idx];
        if(dest[buf_idx] > max)
        {
            max = dest[buf_idx];
        }
    }
    
    for(buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        dest[buf_idx] /= max; /* normalise everything */
    }
    return FT_NOERR;
}

/* Modified Bessel function of the first kind */
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

/* Chebyshev Polynomial */
float
chebyshev_poly(int n, float x)
{
    float y;
    if (fabsf(x) <= 1)
    {
        y = cosf(n * acosf(x));
    }
    else
    {
        y = coshf(n * acoshf(x));
    }
    
    return y;
}

FTA_WindowFunction*
FTA_WindowFunctionInit(unsigned n, FtWindow_t type)
{
    FTA_WindowFunction* window = (FTA_WindowFunction*)malloc(sizeof(FTA_WindowFunction));
    
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


FTA_Error_t
FTA_WindowFunctionFree(FTA_WindowFunction* window)
{
    free(window->window);
    free(window);
    return FT_NOERR;
}

FTA_Error_t
FTA_WindowFunctionProcess(FTA_WindowFunction* window, float* outBuffer, float* inBuffer,unsigned n_samples)
{
    FTA_VectorVectorMultiply(outBuffer, inBuffer, window->window, n_samples);
    return FT_NOERR;
}




