/*
 *  WindowFunction.c
 *  c
 *
 *  Created by Hamilton Kibbe on 6/9/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "WindowFunction.h"
#include "Dsp.h"

#include <stdlib.h>
#include <stddef.h>


#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif


/* Static Function Prototypes */
static float
modZeroBessel(float x);

static double
modZeroBesselD(double x);

static float
chebyshev_poly(int n, float x);

static double
chebyshev_polyD(int n, double x);


/* Implementations */

struct WindowFunction
{
    float*      window;
    unsigned    length;
    Window_t    type;
};

struct WindowFunctionD
{
    double*     window;
    unsigned    length;
    Window_t    type;
};

/*******************************************************************************
 Boxcar */
Error_t 
boxcar(unsigned n, float* dest)
{
    FillBuffer(dest, n, 1.0);
    return NOERR;
}

Error_t
boxcarD(unsigned n, double* dest)
{
    FillBufferD(dest, n, 1.0);
    return NOERR;
}


/*******************************************************************************
 Hann */
Error_t 
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
    return NOERR;
}

Error_t
hannD(unsigned n, double* dest)
{
    
#ifdef __APPLE__
    // Use the accelerate version if we have it
    vDSP_hann_windowD(dest, n - 1, vDSP_HANN_DENORM);
    
#else
    // Otherwise do it manually
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = 0.5 * (1 - cos((2 * M_PI * buf_idx) / (n - 1)));
    }
    
#endif // __APPLE__
    return NOERR;
}



/*******************************************************************************
 Hamming */
Error_t 
hamming(unsigned n, float* dest)
{
    // do it manually. it seems like vDSP_hamm_window is wrong.
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = 0.54 - 0.46 * cosf((2 * M_PI * buf_idx) / (n - 1));
    }
    return NOERR;
}


Error_t
hammingD(unsigned n, double* dest)
{
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = 0.54 - 0.46 * cos((2 * M_PI * buf_idx) / (n - 1));
    }
    return NOERR;
}


/*******************************************************************************
 Blackman */
Error_t 
blackman(unsigned n, float a, float* dest)
{
    if (a > 0.15 && a < 0.17)
    {
        #ifdef __APPLE__
        // Use the builtin version for the specific case it implements, if it is
        // available.
        vDSP_blkman_window(dest, (n - 1), 0);
        return NOERR;
        #endif
    }

    // Otherwise do it manually
    float a0 = (1 - a) / 2;
    float a1 = 0.5;
    float a2 = a / 2;
    
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = a0 - a1 * cosf((2 * M_PI * buf_idx) / (n - 1)) + a2 * cosf((4 * M_PI * buf_idx) / (n - 1));
    }
    return NOERR;
}



Error_t
blackmanD(unsigned n, double a, double* dest)
{
    if (a > 0.15 && a < 0.17)
    {
#ifdef __APPLE__
        // Use the builtin version for the specific case it implements, if it is
        // available.
        vDSP_blkman_windowD(dest, (n - 1), 0);
        return NOERR;
#endif
    }
    
    // Otherwise do it manually
    double a0 = (1 - a) / 2;
    double a1 = 0.5;
    double a2 = a / 2;
    
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = a0 - a1 * cos((2 * M_PI * buf_idx) / (n - 1)) + a2 * cos((4 * M_PI * buf_idx) / (n - 1));
    }
    return NOERR;
}





/*******************************************************************************
 Tukey */
Error_t 
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
    return NOERR;
}


Error_t
tukeyD(unsigned n, double a, double* dest)
{
    double term = a * (n - 1) / 2;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        if (buf_idx <= term)
        {
            dest[buf_idx] = 0.5 * (1 + cos(M_PI * ((2 * buf_idx) /
                                                    (a * (n - 1)) - 1)));
        }
        else if (term <= buf_idx && buf_idx <= (n - 1) * (1 - a / 2))
        {
            dest[buf_idx] = 1.0;
        }
        else
        {
            dest[buf_idx] = 0.5 * (1 + cos(M_PI *((2 * buf_idx) /
                                                   (a * (n - 1)) - (2 / a) + 1)));
        }
        
    }
    return NOERR;
}


/*******************************************************************************
 Cosine */
Error_t 
cosine(unsigned n, float* dest)
{
    float N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = sinf((M_PI * buf_idx) / N);
    }
    return NOERR;
}


Error_t
cosineD(unsigned n, double* dest)
{
    double N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = sin((M_PI * buf_idx) / N);
    }
    return NOERR;
}

/*******************************************************************************
 Lanczos */
Error_t 
lanczos(unsigned n, float* dest)
{
    float N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        float term = M_PI * ((2 * n)/ N) - 1.0;
        *dest++ = sinf(term) / term;
    }
    return NOERR;
}

Error_t
lanczosD(unsigned n, double* dest)
{
    double N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        double term = M_PI * ((2 * n)/ N) - 1.0;
        *dest++ = sin(term) / term;
    }
    return NOERR;
}

/*******************************************************************************
 Bartlett */
Error_t 
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
    return NOERR;
}


Error_t
bartlettD(unsigned n, double* dest)
{
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        if (buf_idx <= (n - 1) / 2)
        {
            *dest++ = (double)(2 * buf_idx)/(n - 1);
        }
        else
        {
            *dest ++ = 2.0 - (double)(2 * buf_idx) / (n - 1);
        }
    }
    return NOERR;
}

/*******************************************************************************
 Gaussian */
Error_t 
gaussian(unsigned n, float sigma, float* dest)
{
    float N = n - 1;
    float L = N / 2.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {   
        *dest++ = expf(-0.5 * powf((buf_idx - L)/(sigma * L),2));
    }
    return NOERR;
}

Error_t
gaussianD(unsigned n, double sigma, double* dest)
{
    double N = n - 1;
    double L = N / 2.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        *dest++ = exp(-0.5 * pow((buf_idx - L)/(sigma * L),2));
    }
    return NOERR;
}


/*******************************************************************************
 Bartlett-Hann */
Error_t 
bartlett_hann(unsigned n, float* dest)
{
    float N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        float term = ((buf_idx / N) - 0.5);
        *dest++ = 0.62 - 0.48 * fabs(term) + 0.38 * cosf(2 * M_PI * term);
    }
    return NOERR;
}


Error_t
bartlett_hannD(unsigned n, double* dest)
{
    double N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        double term = ((buf_idx / N) - 0.5);
        *dest++ = 0.62 - 0.48 * fabs(term) + 0.38 * cos(2 * M_PI * term);
    }
    return NOERR;
}

/*******************************************************************************
 Kaiser */
Error_t 
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
    return NOERR;
}

Error_t
kaiserD(unsigned n, double a, double* dest)
{
    // Pre-calc
    double beta = M_PI * a;
    double m_2 = (float)(n - 1.0) / 2.0;
    double denom = modZeroBesselD(beta);
    
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        double val = ((buf_idx) - m_2) / m_2;
        val = 1 - (val * val);
        *dest++ = modZeroBesselD(beta * sqrt(val)) / denom;
    }
    return NOERR;
}


/*******************************************************************************
 Nuttall */
Error_t 
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
    return NOERR;
}

Error_t
nuttallD(unsigned n, double* dest)
{
    double term;
    double N = n - 1.0;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        term = 2 * M_PI * (buf_idx / N);
        *dest++ = 0.3635819  - 0.4891775 * cos(term) + 0.1365995 *
        cos(2 * term) - 0.0106411 * cos(3 * term);
    }
    return NOERR;
}


/*******************************************************************************
 Blackman-Harris */
Error_t 
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
    return NOERR;    
}


Error_t
blackman_harrisD(unsigned n, double* dest)
{
    float term;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        term = (2 * M_PI * buf_idx) / (n - 1);
        *dest++ = 0.35875 - 0.48829 * cos(term)+ 0.14128 * cos(2 * term) -
        0.01168 * cos(3 * term);
    }
    return NOERR;
}


/*******************************************************************************
 Blackman-Nuttall*/
Error_t
blackman_nuttall(unsigned n, float* dest)
{
    float term;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        term = (2 * M_PI * buf_idx) / (n - 1);
        *dest++ = 0.3635819 - 0.4891775 * cosf(term)+ 0.1365995 * cosf(2 * term) - 0.0106411 * cosf(3 * term);
    }
    return NOERR;    
}

Error_t
blackman_nuttallD(unsigned n, double* dest)
{
    double term;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        term = (2 * M_PI * buf_idx) / (n - 1);
        *dest++ = 0.3635819 - 0.4891775 * cos(term)+ 0.1365995 * cos(2 * term) - 0.0106411 * cos(3 * term);
    }
    return NOERR;
}

/*******************************************************************************
 Flat-Top */
Error_t
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
    return NOERR;    
}


Error_t
flat_topD(unsigned n, double* dest)
{
    double N = n - 1.0;
    double term;
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        term = (2 * M_PI * buf_idx) / N;
        *dest++ = 0.21557895 - 0.41663158 * cos(term)+ 0.277263158 *
        cos(2 * term) - 0.083578947 * cos(3 * term) + 0.006947368 *
        cos(4 * term);
    }
    return NOERR;
}


/*******************************************************************************
 Poisson */
Error_t 
poisson(unsigned n, float D, float* dest)
{
    float term = (n - 1) / 2;
    float tau_inv = 1. / ((n / 2) * (8.69 / D));

    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {

        *dest++ = expf(-fabs(buf_idx - term) * tau_inv);
    }
    return NOERR;    
}

Error_t
poissonD(unsigned n, double D, double* dest)
{
    double term = (n - 1) / 2;
    double tau_inv = 1. / ((n / 2) * (8.69 / D));
    
    unsigned buf_idx;
    for (buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        
        *dest++ = exp(-fabs(buf_idx - term) * tau_inv);
    }
    return NOERR;
}


/*******************************************************************************
 Chebyshev */
// TODO: FIX This.
Error_t
chebyshev(unsigned n, float A, float *dest)
{
    float max = 0;
    float N = n - 1.0;
    float M = N / 2;
    float tg = powf(10, A / 20.0);
    float x0 = coshf((1.0 / N) * acoshf(tg));
    
    for(unsigned buf_idx=0; buf_idx<(n/2+1); ++buf_idx)
    {
        float y = buf_idx - M;
        float sum = 0;
        for(unsigned i=1; i<=M; i++){
            sum += chebyshev_poly(N, x0 * cosf(M_PI * i / n)) *
            cosf( 2.0 * y * M_PI * i / n);
        }
        dest[buf_idx] = tg + 2 * sum;
        dest[(unsigned)N - buf_idx] = dest[buf_idx];
        if(dest[buf_idx] > max)
        {
            max = dest[buf_idx];
        }
    }
    
    for(unsigned buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        dest[buf_idx] /= max;
    }
    return NOERR;
}



Error_t
chebyshevD(unsigned n, double A, double *dest)
{
    double max=0;
    double N = n - 1.0;
    double M = N / 2;
    double tg = pow(10, A / 20.0);
    double x0 = cosh((1.0 / N) * acosh(tg));

    for(unsigned buf_idx=0; buf_idx<(n/2+1); ++buf_idx)
    {
        double y = buf_idx - M;
        double sum = 0;
        for(unsigned i=1; i<=M; i++){
            sum += chebyshev_polyD(N, x0 * cos(M_PI * i / n)) *
            cos( 2.0 * y * M_PI * i / n);
        }
        dest[buf_idx] = tg + 2 * sum;
        dest[(unsigned)N - buf_idx] = dest[buf_idx];
        if(dest[buf_idx] > max)
        {
            max = dest[buf_idx];
        }
    }

    for(unsigned buf_idx = 0; buf_idx < n; ++buf_idx)
    {
        dest[buf_idx] /= max; 
    }
    return NOERR;
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


static double
modZeroBesselD(double x)
{
    double x_2 = x/2;
    double num = 1;
    double fact = 1;
    double result = 1;
    
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

static float
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

static double
chebyshev_polyD(int n, double x)
{
    float y;
    if (fabs(x) <= 1)
    {
        y = cos(n * acos(x));
    }
    else
    {
        y = cosh(n * acosh(x));
    }
    
    return y;
}


/*******************************************************************************
 WindowFunctionInit */
WindowFunction*
WindowFunctionInit(unsigned n, Window_t type)
{
    WindowFunction* window = (WindowFunction*)malloc(sizeof(WindowFunction));
    
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
            kaiser(window->length, 0.5, window->window);
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

WindowFunctionD*
WindowFunctionInitD(unsigned n, Window_t type)
{
    WindowFunctionD* window = (WindowFunctionD*)malloc(sizeof(WindowFunctionD));
    
    window->length = n;
    window->window = (double*)malloc(n * sizeof(double));
    window->type = type;
    
    switch (type)
    {
        case BOXCAR:
            boxcarD(window->length, window->window);
            break;
        case HANN:
            hannD(window->length, window->window);
            break;
        case HAMMING:
            hammingD(window->length, window->window);
            break;
        case BLACKMAN:
            blackmanD(window->length, 0.16, window->window);
            break;
        case TUKEY:
            tukeyD(window->length, 0.5, window->window);
            break;
        case COSINE:
            cosineD(window->length, window->window);
            break;
        case LANCZOS:
            lanczosD(window->length, window->window);
            break;
        case BARTLETT:
            bartlettD(window->length, window->window);
            break;
        case GAUSSIAN:
            gaussianD(window->length, 0.4, window->window);
            break;
        case BARTLETT_HANN:
            bartlett_hannD(window->length, window->window);
            break;
        case KAISER:
            kaiserD(window->length, 0.5, window->window);
            break;
        case NUTTALL:
            nuttallD(window->length, window->window);
            break;
        case BLACKMAN_HARRIS:
            blackman_harrisD(window->length, window->window);
            break;
        case BLACKMAN_NUTTALL:
            blackman_nuttallD(window->length, window->window);
            break;
        case FLATTOP:
            flat_topD(window->length, window->window);
            break;
        case POISSON:
            poissonD(window->length, 8.69, window->window);
            
        default:
            boxcarD(window->length, window->window);
            break;
    }
    
    return window;
}

/*******************************************************************************
 WindowFunctionFree */
Error_t
WindowFunctionFree(WindowFunction* window)
{
    if (window)
    {
        if (window->window)
        {
            free(window->window);
            window->window = NULL;
        }

        free(window);
        window = NULL;
    }
    return NOERR;
}

Error_t
WindowFunctionFreeD(WindowFunctionD* window)
{
    if (window)
    {
        if (window->window)
        {
            free(window->window);
            window->window = NULL;
        }
        
        free(window);
        window = NULL;
    }
    return NOERR;
}

/*******************************************************************************
 WindowFunctionProcess */
Error_t
WindowFunctionProcess(WindowFunction*   window,
                      float*            outBuffer,
                      const float*      inBuffer,
                      unsigned          n_samples)
{
    VectorVectorMultiply(outBuffer, inBuffer, window->window, n_samples);
    return NOERR;
}


Error_t
WindowFunctionProcessD(WindowFunctionD* window,
                       double*          outBuffer,
                       const double*    inBuffer,
                       unsigned         n_samples)
{
    VectorVectorMultiplyD(outBuffer, inBuffer, window->window, n_samples);
    return NOERR;
}

