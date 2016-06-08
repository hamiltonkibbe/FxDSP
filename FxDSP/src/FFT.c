//
//  FFT.c
//  Test
//
//  Created by Hamilton Kibbe on 5/27/13.
//
//


#include "FFT.h"
#include "Dsp.h"
#include "Utilities.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef USE_FFTW_FFT
typedef struct {
    fftwf_plan forward_plan;
    fftwf_plan inverse_plan;
} FFT_SETUP;

typedef struct {
    fftw_plan forward_plan;
    fftw_plan inverse_plan;
} FFT_SETUP_D;

#elif defined(USE_OOURA_FFT)
typedef struct
{
    double* buffer;
    float*  fbuffer;
    int*    ip;
    double* w;
} FFT_SETUP;

typedef struct
{
    double* buffer;
    int*    ip;
    double* w;
} FFT_SETUP_D;

#elif defined(USE_APPLE_FFT)
typedef FFTSetup FFT_SETUP;
typedef FFTSetupD FFT_SETUP_D;


#endif

#pragma mark - Static Function Prototypes
/******************************************************************************
 STATIC FUNCTION PROTOTYPES */


#ifdef USE_FFTW_FFT
static inline void
interleave_complex(float*       dest,
                   const float* real,
                   const float* im,
                   unsigned     length);

static inline void
interleave_complexD(double*         dest,
                    const double*   real,
                    const double*   im,
                    unsigned        length);


static inline void
split_complex(float*        real,
              float*        im,
              const float*  data,
              unsigned      length);


static inline void
split_complexD(double*          real,
               double*          im,
               const double*    data,
               unsigned         length);

#endif
#ifdef USE_OOURA_FFT

static inline void
rdft(int n, int isgn, double *a, int *ip, double *w);

static inline void
makewt(int nw, int *ip, double *w);

static inline void
makect(int nc, int *ip, double *c);

static inline void
bitrv2(int n, int *ip, double *a);

static inline void
cftfsub(int n, double *a, double *w);

static inline void
cftbsub(int n, double *a, double *w);

static inline void
cft1st(int n, double *a, double *w);

static inline void
cftmdl(int n, int l, double *a, double *w);

static inline void
rftfsub(int n, double *a, int nc, double *c);

static inline void
rftbsub(int n, double *a, int nc, double *c);
#endif

struct FFTConfig
{
    unsigned        length;
    float           scale;
    float           log2n;
    FFTSplitComplex split;
    FFTSplitComplex split2;
    FFT_SETUP        setup;
};

struct FFTConfigD
{
    unsigned                length;
    double                  scale;
    double                  log2n;
    FFTSplitComplexD        split;
    FFTSplitComplexD        split2;
    FFT_SETUP_D             setup;
};

#pragma mark - Init/Free

FFTConfig*
FFTInit(unsigned length)
{
    FFTConfig* fft = (FFTConfig*)malloc(sizeof(FFTConfig));
    float* split_realp = (float*)malloc(length * sizeof(float));
    float* split2_realp = (float*)malloc(length * sizeof(float));

    if (fft && split_realp && split2_realp)
    {
        fft->length = length;
        fft->scale = 1.0 / (fft->length);
        fft->log2n = log2f(fft->length);

        // Store these consecutively in memory
        fft->split.realp = split_realp;
        fft->split2.realp = split2_realp;
        fft->split.imagp = fft->split.realp + (fft->length / 2);
        fft->split2.imagp = fft->split2.realp + (fft->length / 2);

#ifdef USE_FFTW_FFT
        fftwf_complex* c = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * length);
        float* r = (float*) fftwf_malloc(sizeof(float) * length);
        fft->setup.forward_plan = fftwf_plan_dft_r2c_1d(length, r, c, FFTW_MEASURE | FFTW_UNALIGNED);
        fft->setup.inverse_plan = fftwf_plan_dft_c2r_1d(length, c, r, FFTW_MEASURE | FFTW_UNALIGNED);
        fftwf_free(r);
        fftwf_free(c);
#elif defined (USE_OOURA_FFT)
        unsigned iplen = (unsigned)ceil(2 + sqrt((double)fft->length));
        unsigned wlen = (unsigned)((fft->length * 5.0 / 8.0) - 1);
        FFT_SETUP setup;
        fft->setup = setup;
        fft->scale = 2.0 / (fft->length);
        fft->setup.ip = (int*) malloc(iplen * sizeof(int));
        fft->setup.w = (double*) malloc(wlen * sizeof(double));
        fft->setup.buffer = (double*)malloc(2*fft->length * sizeof(double));
        fft->setup.fbuffer = (float*)malloc(2*fft->length * sizeof(float));
        // Initialize Buffers
        fft->setup.ip[0] = fft->setup.ip[1] = 0;
        ClearBufferD(fft->setup.w, wlen);
        ClearBufferD(fft->setup.buffer, fft->length + 1);
        ClearBuffer(fft->setup.fbuffer, fft->length + 1);

#elif defined(USE_APPLE_FFT)
        fft->setup = vDSP_create_fftsetup(fft->log2n, FFT_RADIX2);
#endif
        ClearBuffer(split_realp, fft->length);
        ClearBuffer(split2_realp, fft->length);

        return fft;
    }

    else
    {
        // Cleanup
        if (fft)
            free(fft);
        if (split_realp)
            free(split_realp);
        if (split2_realp)
            free(split2_realp);
        return NULL;
    }
}



FFTConfigD*
FFTInitD(unsigned length)
{
    FFTConfigD* fft = (FFTConfigD*)malloc(sizeof(FFTConfigD));
    double* split_realp = (double*)malloc(length * sizeof(double));
    double* split2_realp = (double*)malloc(length * sizeof(double));

    if (fft && split_realp && split2_realp)
    {
        fft->length = length;
        fft->scale = 1.0 / (fft->length);
        fft->log2n = log2f(fft->length);

        // Store these consecutively in memory
        fft->split.realp = split_realp;
        fft->split2.realp = split2_realp;
        fft->split.imagp = fft->split.realp + (fft->length / 2);
        fft->split2.imagp = fft->split2.realp + (fft->length / 2);

#ifdef USE_FFTW_FFT
        fftw_complex* c = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * length);
        double* r = (double*) fftw_malloc(sizeof(double) * length);
        fft->setup.forward_plan = fftw_plan_dft_r2c_1d(length, r, c, FFTW_MEASURE | FFTW_UNALIGNED);
        fft->setup.inverse_plan = fftw_plan_dft_c2r_1d(length, c, r, FFTW_MEASURE | FFTW_UNALIGNED);
        fftw_free(r);
        fftw_free(c);
#elif defined (USE_OOURA_FFT)
        unsigned iplen = (unsigned)ceil(2 + sqrt((double)fft->length));
        unsigned wlen = (unsigned)((fft->length * 5.0 / 8.0) - 1);
        FFT_SETUP_D setup;
        fft->scale = 2.0 / (fft->length);
        fft->setup = setup;
        fft->setup.ip = (int*) malloc(iplen * sizeof(int));
        fft->setup.w = (double*) malloc(wlen * sizeof(double));
        fft->setup.buffer = (double*)malloc(2*fft->length * sizeof(double));
        fft->setup.ip[0] = fft->setup.ip[1] = 0;
        ClearBufferD(fft->setup.w, wlen);
        ClearBufferD(fft->setup.buffer, fft->length);

#elif defined(USE_APPLE_FFT)
        fft->setup = vDSP_create_fftsetupD(fft->log2n, FFT_RADIX2);
#endif
        ClearBufferD(split_realp, fft->length);
        ClearBufferD(split2_realp, fft->length);

        return fft;
    }

    else
    {
        // Cleanup
        if (fft)
            free(fft);
        if (split_realp)
            free(split_realp);
        if (split2_realp)
            free(split2_realp);
        return NULL;
    }
}


Error_t
FFTFree(FFTConfig* fft)
{
    if (fft)
    {
        if (fft->split.realp)
        {
            free(fft->split.realp);
            fft->split.realp = NULL;
        }
        if (fft->split2.realp)
        {
            free(fft->split2.realp);
            fft->split2.realp = NULL;
        }

#ifdef USE_FFTW_FFT
        if (fft->setup.forward_plan)
            fftwf_destroy_plan(fft->setup.forward_plan);
        if (fft->setup.inverse_plan)
            fftwf_destroy_plan(fft->setup.inverse_plan);
#elif defined(USE_OOURA_FFT)
        free(fft->setup.ip);
        free(fft->setup.w);
        free(fft->setup.buffer);
        free(fft->setup.fbuffer);
#elif defined(USE_APPLE_FFT)
        if (fft->setup)
        {
            vDSP_destroy_fftsetup(fft->setup);
        }
#endif
        free(fft);
        fft = NULL;
    }
    return NOERR;
}

Error_t
FFTFreeD(FFTConfigD* fft)
{
    if (fft)
    {
        if (fft->split.realp)
        {
            free(fft->split.realp);
            fft->split.realp = NULL;
        }
        if (fft->split2.realp)
        {
            free(fft->split2.realp);
            fft->split2.realp = NULL;
        }
#ifdef USE_FFTW_FFT
        if (fft->setup.forward_plan)
            fftw_destroy_plan(fft->setup.forward_plan);
        if (fft->setup.inverse_plan)
            fftw_destroy_plan(fft->setup.inverse_plan);
#elif defined(USE_OOURA_FFT)
        free(fft->setup.ip);
        free(fft->setup.w);
        free(fft->setup.buffer);
#elif defined(USE_APPLE_FFT)
        if (fft->setup)
        {
            vDSP_destroy_fftsetupD(fft->setup);
        }
#endif
        free(fft);
        fft = NULL;
    }
    return NOERR;
}


#pragma mark - FFT

Error_t
FFT_R2C(FFTConfig*      fft,
        const float*    inBuffer,
        float*          real,
        float*          imag)
{
#ifdef USE_FFTW_FFT
    FFTComplex temp[fft->length];
    fftwf_execute_dft_r2c(fft->setup.forward_plan, (float*)inBuffer, temp);
    split_complex(real, imag, (const float*)temp, fft->length);
#elif defined(USE_OOURA_FFT)

    float* buf = fft->setup.fbuffer;
    float* end = fft->setup.fbuffer + fft->length;
    float* re = real;
    float* im = imag;

    FloatToDouble(fft->setup.buffer, inBuffer, fft->length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    DoubleToFloat(fft->setup.fbuffer, fft->setup.buffer, fft->length);

    while (buf < end)
    {
        *re++ = *buf++;
        *im++ = -(*buf++);
    }
    real[fft->length / 2 - 1] = -imag[0];
    imag[0] = 0.0;
#elif defined(USE_APPLE_FFT)
    FFTSplitComplex out = {.realp = real, .imagp = imag};

    // convert real input to split complex
    vDSP_ctoz((FFTComplex*)inBuffer, 2, &out, 1, (fft->length/2));

    // Calculate FFT of the two signals
    vDSP_fft_zrip(fft->setup, &out, 1, fft->log2n, FFT_FORWARD);
    out.realp[fft->length / 2 - 1] = out.imagp[0];
    out.imagp[0] = 0.0;
    VectorScalarMultiply(real, real, 0.5, fft->length/2);
    VectorScalarMultiply(imag, imag, 0.5, fft->length/2);

#endif
    return NOERR;
}

Error_t
FFT_R2CD(FFTConfigD*    fft,
         const double*  inBuffer,
         double*        real,
         double*        imag)
{

#ifdef USE_FFTW_FFT
    FFTComplexD temp[fft->length];
    fftw_execute_dft_r2c(fft->setup.forward_plan, (double*)inBuffer, temp);
    split_complexD(real, imag, (const double*)temp, fft->length);

#elif defined(USE_OOURA_FFT)
    double* buf = fft->setup.buffer;
    double* end = fft->setup.buffer + fft->length;
    double* re = real;
    double* im = imag;
    CopyBufferD(fft->setup.buffer, (const double*)inBuffer, fft->length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    while (buf < end)
    {
        *re++ = *buf++;
        *im++ = -(*buf++);
    }
    real[fft->length / 2 - 1] = -imag[0];
    imag[0] = 0.0;
#elif defined(USE_APPLE_FFT)
    FFTSplitComplexD out = {.realp = real, .imagp = imag};

    // convert real input to split complex
    vDSP_ctozD((FFTComplexD*)inBuffer, 2, &out, 1, (fft->length/2));

    // Calculate FFT of the two signals
    vDSP_fft_zripD(fft->setup, &out, 1, fft->log2n, FFT_FORWARD);
    out.realp[fft->length / 2 - 1] = out.imagp[0];
    out.imagp[0] = 0.0;
    VectorScalarMultiplyD(real, real, 0.5, fft->length/2);
    VectorScalarMultiplyD(imag, imag, 0.5, fft->length/2);

#endif
    return NOERR;
}


Error_t
FFT_IR_R2C(FFTConfig*       fft,
           const float*     inBuffer,
           FFTSplitComplex  out)
{
#ifdef USE_FFTW_FFT
    FFTComplex temp[fft->length];
    fftwf_execute_dft_r2c(fft->setup.forward_plan, (float*)inBuffer, temp);
    split_complex(out.realp, out.imagp, (const float*)temp, fft->length);
    out.imagp[0] = ((float*)temp)[fft->length];
#elif defined(USE_OOURA_FFT)

    float* buf = fft->setup.fbuffer;
    float* end = fft->setup.fbuffer + fft->length;
    float* re = out.realp;
    float* im = out.imagp;

    FloatToDouble(fft->setup.buffer, inBuffer, fft->length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    DoubleToFloat(fft->setup.fbuffer, fft->setup.buffer, fft->length);

    while (buf < end)
    {
        *re++ = *buf++;
        *im++ = -(*buf++);
    }
#elif defined(USE_APPLE_FFT)

    // convert real input to split complex
    vDSP_ctoz((FFTComplex*)inBuffer, 2, &out, 1, (fft->length/2));

    // Calculate FFT of the two signals
    vDSP_fft_zrip(fft->setup, &out, 1, fft->log2n, FFT_FORWARD);
    VectorScalarMultiply(out.realp, out.realp, 0.5, fft->length/2);
    VectorScalarMultiply(out.imagp, out.imagp, 0.5, fft->length/2);

#endif
    return NOERR;
}


Error_t
FFT_IR_R2CD(FFTConfigD*         fft,
            const double*       inBuffer,
            FFTSplitComplexD    out)
{

#ifdef USE_FFTW_FFT
    FFTComplexD temp[fft->length];
    fftw_execute_dft_r2c(fft->setup.forward_plan, (double*)inBuffer, temp);
    split_complexD(out.realp, out.imagp, (const double*)temp, fft->length);
    out.imagp[0] = ((double*)temp)[fft->length];
#elif defined(USE_OOURA_FFT)
    double* buf = fft->setup.buffer;
    double* end = fft->setup.buffer + fft->length;
    double* re = out.realp;
    double* im = out.imagp;
    CopyBufferD(fft->setup.buffer, (const double*)inBuffer, fft->length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    while (buf < end)
    {
        *re++ = *buf++;
        *im++ = -(*buf++);
    }
#elif defined(USE_APPLE_FFT)

    // convert real input to split complex
    vDSP_ctozD((FFTComplexD*)inBuffer, 2, &out, 1, (fft->length/2));

    // Calculate FFT of the two signals
    vDSP_fft_zripD(fft->setup, &out, 1, fft->log2n, FFT_FORWARD);
    VectorScalarMultiplyD(out.realp, out.realp, 0.5, fft->length/2);
    VectorScalarMultiplyD(out.imagp, out.imagp, 0.5, fft->length/2);

#endif
    return NOERR;
}


#pragma mark - IFFT

Error_t
IFFT_C2R(FFTConfig*    fft,
         const float*  inReal,
         const float*  inImag,
         float*        out)
{
#ifdef USE_FFTW_FFT
    FFTComplex temp[fft->length/2 + 1];
    interleave_complex((float*)temp, inReal, inImag, fft->length);
    ((float*)temp)[fft->length] = inReal[fft->length / 2 - 1];
    fftwf_execute_dft_c2r(fft->setup.inverse_plan, temp, out);
    VectorScalarMultiply(out, out, fft->scale, fft->length);
#elif defined(USE_OOURA_FFT)

    float* re = (float*)inReal;
    float* im = (float*)inImag;
    float* buf = fft->setup.fbuffer;
    float* end = fft->setup.fbuffer + fft->length;

    while (buf != end)
    {
     *buf++ = *re++;
     *buf++ = -(*im++);
    }

    FloatToDouble(fft->setup.buffer, fft->setup.fbuffer, fft->length);
    fft->setup.buffer[1] = (double)inReal[fft->length / 2 - 1];
    rdft(fft->length, -1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    DoubleToFloat(fft->setup.fbuffer, fft->setup.buffer, fft->length);
    VectorScalarMultiply(out, fft->setup.fbuffer, fft->scale, fft->length);


#elif defined(USE_APPLE_FFT)
    // Convert input to split complex format
    CopyBuffer(fft->split.realp, inReal, fft->length/2.);
    CopyBuffer(fft->split.imagp, inImag, fft->length/2.);
    fft->split.imagp[0] = inReal[fft->length/2-1];

    // Inverse Real FFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztoc(&fft->split, 1, (FFTComplex*)out, 2, fft->length/2);
    // Scale the result...
    vDSP_vsmul(out, 1, &fft->scale, out, 1, fft->length);
#endif
    return NOERR;
}

Error_t
IFFT_C2RD(FFTConfigD*   fft,
       const double*    inReal,
       const double*    inImag,
       double*          out)
{
#ifdef USE_FFTW_FFT
    FFTComplexD temp[fft->length/2 + 1];
    interleave_complexD((double*)temp, inReal, inImag, fft->length);
    ((double*)temp)[fft->length] = inReal[fft->length / 2 - 1];
    fftw_execute_dft_c2r(fft->setup.inverse_plan, temp, out);
    VectorScalarMultiplyD(out, out, fft->scale, fft->length);
#elif defined(USE_OOURA_FFT)
    double* re = (double*)inReal;
    double* im = (double*)inImag;

    double* buf = fft->setup.buffer;
    double* end = fft->setup.buffer + fft->length;
    while (buf != end)
    {
        *buf++ = *re++;
        *buf++ = -(*im++);
    }
    fft->setup.buffer[1] = inReal[fft->length / 2 - 1];
    rdft(fft->length, -1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    VectorScalarMultiplyD(out, fft->setup.buffer, fft->scale, fft->length);


#elif defined(USE_APPLE_FFT)
    // Convert input to split complex format
    CopyBufferD(fft->split.realp, inReal, fft->length/2.);
    CopyBufferD(fft->split.imagp, inImag, fft->length/2.);
    fft->split.imagp[0] = inReal[fft->length/2-1];

    // Inverse Real FFT
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztocD(&fft->split, 1, (FFTComplexD*)out, 2, fft->length/2);
    // Scale the result...
    vDSP_vsmulD(out, 1, &fft->scale, out, 1, fft->length);
#endif
    return NOERR;
}

#pragma mark - FFT Convolution

Error_t
FFTConvolve(FFTConfig*  fft,
            float       *in1,
            unsigned    in1_length,
            float       *in2,
            unsigned    in2_length,
            float       *dest)
{

#if defined(USE_FFTW_FFT)

    FFTComplex temp[fft->length];

    // Padded input buffers
    float in1_padded[fft->length];
    float in2_padded[fft->length];
    ClearBuffer(in1_padded, fft->length);
    ClearBuffer(in2_padded, fft->length);
    ClearBuffer(fft->split.realp, fft->length);
    ClearBuffer(fft->split2.realp, fft->length);

    // Zero pad the input buffers to FFT length
    CopyBuffer(in1_padded, in1, in1_length);
    CopyBuffer(in2_padded, in2, in2_length);

    fftwf_execute_dft_r2c(fft->setup.forward_plan, (float*)in1_padded, temp);
    float nyquist1 = ((float*)temp)[fft->length];
    split_complex(fft->split.realp, fft->split.imagp, (const float*)temp, fft->length);

    fftwf_execute_dft_r2c(fft->setup.forward_plan, (float*)in2_padded, temp);
    float nyquist2 = ((float*)temp)[fft->length];
    split_complex(fft->split2.realp, fft->split2.imagp, (const float*)temp, fft->length);


    float nyquist_out = nyquist1 * nyquist2;
    ComplexMultiply(fft->split.realp, fft->split.imagp, fft->split.realp,
                    fft->split.imagp, fft->split2.realp, fft->split2.imagp,
                    fft->length/2);
    interleave_complex((float*)temp, fft->split.realp, fft->split.imagp, fft->length);
    ((float*)temp)[fft->length] = nyquist_out;
    fftwf_execute_dft_c2r(fft->setup.inverse_plan, temp, dest);
    VectorScalarMultiply(dest, dest, fft->scale, fft->length);


#elif defined(USE_OOURA_FFT)

    float* buf = fft->setup.fbuffer;
    float* end = fft->setup.fbuffer + fft->length;
    float* re = fft->split.realp;
    float* im = fft->split.imagp;
    float* re2 = fft->split2.realp;
    float* im2 = fft->split2.imagp;

    ClearBufferD(fft->setup.buffer, fft->length);
    FloatToDouble(fft->setup.buffer, in1, in1_length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    DoubleToFloat(fft->setup.fbuffer, fft->setup.buffer, fft->length);

    while (buf < end)
    {
        *re++ = *buf++;
        *im++ = -(*buf++);
    }

    buf = fft->setup.fbuffer;

    ClearBufferD(fft->setup.buffer, fft->length);
    FloatToDouble(fft->setup.buffer, in2, in2_length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    DoubleToFloat(fft->setup.fbuffer, fft->setup.buffer, fft->length);

    while (buf < end)
    {
        *re2++ = *buf++;
        *im2++ = -(*buf++);
    }

    float nyquist_out = fft->split.imagp[0] * fft->split2.imagp[0];
    fft->split.imagp[0] = 0.0;
    fft->split2.imagp[0] = 0.0;
    ComplexMultiply(fft->split.realp, fft->split.imagp, fft->split.realp,
                    fft->split.imagp, fft->split2.realp, fft->split2.imagp,
                    fft->length/2);
    fft->split.imagp[0] = -nyquist_out;


    re = fft->split.realp;
    im = fft->split.imagp;
    buf = fft->setup.fbuffer;

    while (buf != end)
    {
        *buf++ = *re++;
        *buf++ = -(*im++);
    }

    FloatToDouble(fft->setup.buffer, fft->setup.fbuffer, fft->length);
    rdft(fft->length, -1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    DoubleToFloat(fft->setup.fbuffer, fft->setup.buffer, fft->length);
    VectorScalarMultiply(dest, fft->setup.fbuffer, fft->scale, fft->length);


#elif defined(USE_APPLE_FFT)

    // Padded input buffers
    float in1_padded[fft->length];
    float in2_padded[fft->length];
    ClearBuffer(in1_padded, fft->length);
    ClearBuffer(in2_padded, fft->length);
    ClearBuffer(fft->split.realp, fft->length);
    ClearBuffer(fft->split2.realp, fft->length);

    // Zero pad the input buffers to FFT length
    CopyBuffer(in1_padded, in1, in1_length);
    CopyBuffer(in2_padded, in2, in2_length);

    // convert real input to split complex
    vDSP_ctoz((FFTComplex*)in1_padded, 2, &fft->split, 1, (fft->length/2));
    vDSP_ctoz((FFTComplex*)in2_padded, 2, &fft->split2, 1, (fft->length/2));

    // Calculate FFT of the two signals
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);
    vDSP_fft_zrip(fft->setup, &fft->split2, 1, fft->log2n, FFT_FORWARD);

    // Scale FFT
    VectorScalarMultiply(fft->split.realp, fft->split.realp, 0.5, fft->length);
    VectorScalarMultiply(fft->split2.realp, fft->split2.realp, 0.5, fft->length);


    // Unpack nyquist, multiply, and re-pack for ifft
    float nyquist_out = fft->split.imagp[0] * fft->split2.imagp[0];
    fft->split.imagp[0] = 0.0;
    fft->split2.imagp[0] = 0.0;
    vDSP_zvmul(&fft->split, 1, &fft->split2, 1, &fft->split, 1, fft->length/2, 1);
    fft->split.imagp[0] = nyquist_out;

    // IFFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztoc(&fft->split, 1, (FFTComplex*)dest, 2, fft->length/2);
    vDSP_vsmul(dest, 1, &fft->scale, dest, 1, fft->length);
#endif
    // Do the inverse FFT
    //IFFT_C2R(fft, fft->split.realp, fft->split.imagp, dest);
    return NOERR;
}



Error_t
FFTConvolveD(FFTConfigD*    fft,
             const double*  in1,
             unsigned       in1_length,
             const double*  in2,
             unsigned       in2_length,
             double*        dest)
{

#if defined(USE_FFTW_FFT)

    FFTComplexD temp[fft->length];

    // Padded input buffers
    double in1_padded[fft->length];
    double in2_padded[fft->length];
    ClearBufferD(in1_padded, fft->length);
    ClearBufferD(in2_padded, fft->length);
    ClearBufferD(fft->split.realp, fft->length);
    ClearBufferD(fft->split2.realp, fft->length);

    // Zero pad the input buffers to FFT length
    CopyBufferD(in1_padded, in1, in1_length);
    CopyBufferD(in2_padded, in2, in2_length);

    fftw_execute_dft_r2c(fft->setup.forward_plan, (double*)in1_padded, temp);
    double nyquist1 = ((double*)temp)[fft->length];
    split_complexD(fft->split.realp, fft->split.imagp, (const double*)temp, fft->length);

    fftw_execute_dft_r2c(fft->setup.forward_plan, (double*)in2_padded, temp);
    double nyquist2 = ((double*)temp)[fft->length];
    split_complexD(fft->split2.realp, fft->split2.imagp, (const double*)temp, fft->length);


    double nyquist_out = nyquist1 * nyquist2;
    ComplexMultiplyD(fft->split.realp, fft->split.imagp, fft->split.realp,
                     fft->split.imagp, fft->split2.realp, fft->split2.imagp,
                     fft->length / 2);


    interleave_complexD((double*)temp, fft->split.realp, fft->split.imagp, fft->length);
    ((double*)temp)[fft->length] = nyquist_out;
    fftw_execute_dft_c2r(fft->setup.inverse_plan, temp, dest);
    VectorScalarMultiplyD(dest, dest, fft->scale, fft->length);

#elif defined(USE_OOURA_FFT)
    double* buf = fft->setup.buffer;
    double* end = fft->setup.buffer + fft->length;
    double* re = fft->split.realp;
    double* im = fft->split.imagp;
    double* re2 = fft->split2.realp;
    double* im2 = fft->split2.imagp;

    ClearBufferD(fft->setup.buffer, fft->length);
    CopyBufferD(fft->setup.buffer, (const double*)in1, in1_length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);

    while (buf < end)
    {
        *re++ = *buf++;
        *im++ = -(*buf++);
    }

    buf = fft->setup.buffer;
    ClearBufferD(fft->setup.buffer, fft->length);
    CopyBufferD(fft->setup.buffer, (const double*)in2, in2_length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);

    while (buf < end)
    {
        *re2++ = *buf++;
        *im2++ = -(*buf++);
    }


    double nyquist_out = fft->split.imagp[0] * fft->split2.imagp[0];
    fft->split.imagp[0] = 0.0;
    fft->split2.imagp[0] = 0.0;
    ComplexMultiplyD(fft->split.realp, fft->split.imagp, fft->split.realp,
                     fft->split.imagp, fft->split2.realp, fft->split2.imagp,
                     fft->length / 2);
    fft->split.imagp[0] = -nyquist_out;


    re = fft->split.realp;
    im = fft->split.imagp;
    buf = fft->setup.buffer;

    while (buf != end)
    {
        *buf++ = *re++;
        *buf++ = -(*im++);
    }

    rdft(fft->length, -1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    VectorScalarMultiplyD(dest, fft->setup.buffer, fft->scale, fft->length);


#elif defined(USE_APPLE_FFT)
    // Padded input buffers
    double in1_padded[fft->length];
    double in2_padded[fft->length];
    ClearBufferD(in1_padded, fft->length);
    ClearBufferD(in2_padded, fft->length);
    ClearBufferD(fft->split.realp, fft->length);
    ClearBufferD(fft->split2.realp, fft->length);

    // Zero pad the input buffers to FFT length
    CopyBufferD(in1_padded, in1, in1_length);
    CopyBufferD(in2_padded, in2, in2_length);

    // convert real input to split complex
    vDSP_ctozD((FFTComplexD*)in1_padded, 2, &fft->split, 1, (fft->length/2));
    vDSP_ctozD((FFTComplexD*)in2_padded, 2, &fft->split2, 1, (fft->length/2));

    // Calculate FFT of the two signals
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);
    vDSP_fft_zripD(fft->setup, &fft->split2, 1, fft->log2n, FFT_FORWARD);

    // Scale FFT
    VectorScalarMultiplyD(fft->split.realp, fft->split.realp, 0.5, fft->length);
    VectorScalarMultiplyD(fft->split2.realp, fft->split2.realp, 0.5, fft->length);


    // Unpack nyquist, multiply, and re-pack for ifft
    double nyquist_out = fft->split.imagp[0] * fft->split2.imagp[0];
    fft->split.imagp[0] = 0.0;
    fft->split2.imagp[0] = 0.0;
    vDSP_zvmulD(&fft->split, 1, &fft->split2, 1, &fft->split, 1, fft->length/2, 1);
    fft->split.imagp[0] = nyquist_out;

    // IFFT
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztocD(&fft->split, 1, (FFTComplexD*)dest, 2, fft->length/2);
    vDSP_vsmulD(dest, 1, &fft->scale, dest, 1, fft->length);

#endif
    return NOERR;
}


#pragma mark - FFT Filter Convolution

Error_t
FFTFilterConvolve(FFTConfig*        fft,
                  const float*      in,
                  unsigned          in_length,
                  FFTSplitComplex   fft_ir,
                  float*            dest)
{


#ifdef USE_FFTW_FFT

    FFTComplex temp[fft->length];

    // Padded input buffers
    float in_padded[fft->length];

    ClearBuffer(in_padded, fft->length);
    ClearBuffer(fft->split.realp, fft->length);
    CopyBuffer(in_padded, in, in_length);

    fftwf_execute_dft_r2c(fft->setup.forward_plan, (float*)in_padded, temp);
    float nyquist = ((float*)temp)[fft->length];
    split_complex(fft->split.realp, fft->split.imagp, (const float*)temp, fft->length);


    float nyquist_out = nyquist * fft_ir.imagp[0];
    ComplexMultiply(fft->split.realp, fft->split.imagp, fft->split.realp,
                    fft->split.imagp, fft_ir.realp, fft_ir.imagp,
                    fft->length/2);

    interleave_complex((float*)temp, fft->split.realp, fft->split.imagp, fft->length);
    ((float*)temp)[fft->length] = nyquist_out;
    fftwf_execute_dft_c2r(fft->setup.inverse_plan, temp, dest);
    VectorScalarMultiply(dest, dest, fft->scale, fft->length);


#elif defined(USE_OOURA_FFT)

    float* buf = fft->setup.fbuffer;
    float* end = fft->setup.fbuffer + fft->length;
    float* re = fft->split.realp;
    float* im = fft->split.imagp;

    ClearBufferD(fft->setup.buffer, fft->length);
    FloatToDouble(fft->setup.buffer, in, in_length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    DoubleToFloat(fft->setup.fbuffer, fft->setup.buffer, fft->length);

    while (buf < end)
    {
        *re++ = *buf++;
        *im++ = -(*buf++);
    }

    float nyquist_out = fft->split.imagp[0] * fft_ir.imagp[0];
    fft->split.imagp[0] = 0.0;
    ComplexMultiply(fft->split.realp, fft->split.imagp, fft->split.realp, fft->split.imagp, fft_ir.realp, fft_ir.imagp, fft->length/2);
    fft->split.imagp[0] = -nyquist_out;


    re = fft->split.realp;
    im = fft->split.imagp;
    buf = fft->setup.fbuffer;

    while (buf != end)
    {
        *buf++ = *re++;
        *buf++ = -(*im++);
    }

    FloatToDouble(fft->setup.buffer, fft->setup.fbuffer, fft->length);
    rdft(fft->length, -1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    DoubleToFloat(fft->setup.fbuffer, fft->setup.buffer, fft->length);
    VectorScalarMultiply(dest, fft->setup.fbuffer, fft->scale, fft->length);
#elif defined(USE_APPLE_FFT)

    // Padded buffer
    float in_padded[fft->length];

    // Zero pad the input to FFT length
    ClearBuffer(in_padded, fft->length);
    CopyBuffer(in_padded, in, in_length);

    // convert real input to split complex
    vDSP_ctoz((FFTComplex*)in_padded, 2, &fft->split, 1, (fft->length/2));

    // Calculate FFT of the two signals
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);

    // Scale FFT
    VectorScalarMultiply(fft->split.realp, fft->split.realp, 0.5, fft->length);

    // Unpack nyquist, multiply, and re-pack for ifft
    float nyquist_out = fft->split.imagp[0] * fft_ir.imagp[0];
    fft->split.imagp[0] = 0.0;
    vDSP_zvmul(&fft->split, 1, &fft_ir, 1, &fft->split, 1, fft->length/2, 1);
    fft->split.imagp[0] = nyquist_out;

    // IFFT
    vDSP_fft_zrip(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztoc(&fft->split, 1, (FFTComplex*)dest, 2, fft->length/2);
    vDSP_vsmul(dest, 1, &fft->scale, dest, 1, fft->length);
#endif
    return NOERR;
}


Error_t
FFTFilterConvolveD(FFTConfigD*      fft,
                   const double*    in,
                   unsigned         in_length,
                   FFTSplitComplexD fft_ir,
                   double*          dest)
{


#if defined(USE_FFTW_FFT)
    FFTComplexD temp[fft->length];

    // Padded input buffers
    double in_padded[fft->length];
    ClearBufferD(in_padded, fft->length);
    ClearBufferD(fft->split.realp, fft->length);
    CopyBufferD(in_padded, in, in_length);

    fftw_execute_dft_r2c(fft->setup.forward_plan, (double*)in_padded, temp);
    double nyquist = ((double*)temp)[fft->length];
    split_complexD(fft->split.realp, fft->split.imagp, (const double*)temp, fft->length);


    double nyquist_out = nyquist * fft_ir.imagp[0];
    ComplexMultiplyD(fft->split.realp, fft->split.imagp, fft->split.realp,
                     fft->split.imagp, fft_ir.realp, fft_ir.imagp,
                     fft->length / 2);


    interleave_complexD((double*)temp, fft->split.realp, fft->split.imagp, fft->length);
    ((double*)temp)[fft->length] = nyquist_out;
    fftw_execute_dft_c2r(fft->setup.inverse_plan, temp, dest);
    VectorScalarMultiplyD(dest, dest, fft->scale, fft->length);

#elif defined(USE_OOURA_FFT)
    double* buf = fft->setup.buffer;
    double* end = fft->setup.buffer + fft->length;
    double* re = fft->split.realp;
    double* im = fft->split.imagp;

    // Zero pad the input to FFT length
    ClearBufferD(fft->setup.buffer, fft->length);
    CopyBufferD(fft->setup.buffer, in, in_length);
    rdft(fft->length, 1, fft->setup.buffer, fft->setup.ip, fft->setup.w);

    while (buf < end)
    {
        *re++ = *buf++;
        *im++ = -(*buf++);
    }


    double nyquist_out = fft->split.imagp[0] * fft_ir.imagp[0];
    fft->split.imagp[0] = 0.0;

    ComplexMultiplyD(fft->split.realp, fft->split.imagp, fft->split.realp,
                     fft->split.imagp, fft_ir.realp, fft_ir.imagp,
                     fft->length/2);
    fft->split.imagp[0] = -nyquist_out;


    re = fft->split.realp;
    im = fft->split.imagp;
    buf = fft->setup.buffer;

    while (buf != end)
    {
        *buf++ = *re++;
        *buf++ = -(*im++);
    }

    rdft(fft->length, -1, fft->setup.buffer, fft->setup.ip, fft->setup.w);
    VectorScalarMultiplyD(dest, fft->setup.buffer, fft->scale, fft->length);
#elif defined(USE_APPLE_FFT)

    // Padded buffer
    double in_padded[fft->length];

    // Zero pad the input to FFT length
    ClearBufferD(in_padded, fft->length);
    CopyBufferD(in_padded, in, in_length);

    // convert real input to split complex
    vDSP_ctozD((FFTComplexD*)in_padded, 2, &fft->split, 1, (fft->length/2));

    // Calculate FFT of the two signals
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_FORWARD);

    // Scale FFT
    VectorScalarMultiplyD(fft->split.realp, fft->split.realp, 0.5, fft->length);

    // Unpack nyquist, multiply, and re-pack for ifft
    double nyquist_out = fft->split.imagp[0] * fft_ir.imagp[0];
    fft->split.imagp[0] = 0.0;
    vDSP_zvmulD(&fft->split, 1, &fft_ir, 1, &fft->split, 1, fft->length/2, 1);
    fft->split.imagp[0] = nyquist_out;

    // IFFT
    vDSP_fft_zripD(fft->setup, &fft->split, 1, fft->log2n, FFT_INVERSE);
    vDSP_ztocD(&fft->split, 1, (FFTComplexD*)dest, 2, fft->length/2);
    vDSP_vsmulD(dest, 1, &fft->scale, dest, 1, fft->length);
#endif
    return NOERR;
}


/******************************************************************************
 STATIC FUNCTION DEFINITIONS */
#pragma mark - Static Function Definitions
#ifdef USE_FFTW_FFT
static inline void
interleave_complex(float*dest, const float* real, const float* imag, unsigned length)
{
#if defined(__APPLE__)
    DSPSplitComplex split = {.realp = (float*)real, .imagp = (float*)imag};
    vDSP_ztoc(&split, 1, (DSPComplex*)dest, 2, length/2);
#elif defined(__HAS_BLAS__)
    cblas_scopy(length/2, real, 1, dest, 2);
    cblas_scopy(length/2, imag, 1, dest + 1, 2);
#else
    float* buf = &dest[0];
    float* end = buf + length;
    const float* re = real;
    const float* im = imag;
    while (buf != end)
    {
        *buf++ = *re++;
        *buf++ = *im++;
    }
#endif
}

static inline void
interleave_complexD(double* dest, const double* real, const double* imag, unsigned length)
{
#if defined(__APPLE__)
    DSPDoubleSplitComplex split = {.realp = (double*)real, .imagp = (double*)imag};
    vDSP_ztocD(&split, 1, (DSPDoubleComplex*)dest, 2, length/2);
#elif defined(__HAS_BLAS__)
    cblas_dcopy(length/2, real, 1, dest, 2);
    cblas_dcopy(length/2, imag, 1, dest + 1, 2);
#else
    double* buf = &dest[0];
    double* end = buf + length;
    const double* re = real;
    const double* im = imag;
    while (buf != end)
    {
        *buf++ = *re++;
        *buf++ = *im++;
    }
#endif
}


static inline void
split_complex(float* real, float* imag, const float* data, unsigned length)
{
#if defined(__APPLE__)
    DSPSplitComplex split = {.realp = real, .imagp = imag};
    vDSP_ctoz((DSPComplex*)data, 2, &split, 1, length/2);
#elif defined(__HAS_BLAS__)
    cblas_scopy(length/2, data, 2, real, 1);
    cblas_scopy(length/2, data + 1, 2, imag, 1);
#else
    float* buf = (float*)data;
    float* end = buf + length;
    float* re = real;
    float* im = imag;
    while (buf != end)
    {
        *re++ = *buf++;
        *im++ = *buf++;
    }
#endif
}


static inline void
split_complexD(double* real, double* imag, const double* data, unsigned length)
{
#if defined(__APPLE__)
    DSPDoubleSplitComplex split = {.realp = real, .imagp = imag};
    vDSP_ctozD((DSPDoubleComplex*)data, 2, &split, 1, length/2);
#elif defined(__HAS_BLAS__)
    cblas_dcopy(length/2, data, 2, real, 1);
    cblas_dcopy(length/2, data + 1, 2, imag, 1);
#else
    double* buf = (double*)data;
    double* end = buf + length;
    double* re = real;
    double* im = imag;
    while (buf != end)
    {
        *re++ = *buf++;
        *im++ = *buf++;
    }
#endif
}

#endif


#ifdef USE_OOURA_FFT

static void
rdft(int n, int isgn, double *a, int *ip, double *w)
{
    void makewt(int nw, int *ip, double *w);
    void makect(int nc, int *ip, double *c);
    void bitrv2(int n, int *ip, double *a);
    void cftfsub(int n, double *a, double *w);
    void cftbsub(int n, double *a, double *w);
    void rftfsub(int n, double *a, int nc, double *c);
    void rftbsub(int n, double *a, int nc, double *c);
    int nw, nc;
    double xi;

    nw = ip[0];
    if (n > (nw << 2)) {
        nw = n >> 2;
        makewt(nw, ip, w);
    }
    nc = ip[1];
    if (n > (nc << 2)) {
        nc = n >> 2;
        makect(nc, ip, w + nw);
    }
    if (isgn >= 0) {
        if (n > 4) {
            bitrv2(n, ip + 2, a);
            cftfsub(n, a, w);
            rftfsub(n, a, nc, w + nw);
        } else if (n == 4) {
            cftfsub(n, a, w);
        }
        xi = a[0] - a[1];
        a[0] += a[1];
        a[1] = xi;
    } else {
        a[1] = 0.5 * (a[0] - a[1]);
        a[0] -= a[1];
        if (n > 4) {
            rftbsub(n, a, nc, w + nw);
            bitrv2(n, ip + 2, a);
            cftbsub(n, a, w);
        } else if (n == 4) {
            cftfsub(n, a, w);
        }
    }
}
static void
makewt(int nw, int *ip, double *w)
{
    void bitrv2(int n, int *ip, double *a);
    int j, nwh;
    double delta, x, y;

    ip[0] = nw;
    ip[1] = 1;
    if (nw > 2) {
        nwh = nw >> 1;
        delta = atan(1.0) / nwh;
        w[0] = 1;
        w[1] = 0;
        w[nwh] = cos(delta * nwh);
        w[nwh + 1] = w[nwh];
        if (nwh > 2) {
            for (j = 2; j < nwh; j += 2) {
                x = cos(delta * j);
                y = sin(delta * j);
                w[j] = x;
                w[j + 1] = y;
                w[nw - j] = y;
                w[nw - j + 1] = x;
            }
            bitrv2(nw, ip + 2, w);
        }
    }
}


static void
makect(int nc, int *ip, double *c)
{
    int j, nch;
    double delta;

    ip[1] = nc;
    if (nc > 1) {
        nch = nc >> 1;
        delta = atan(1.0) / nch;
        c[0] = cos(delta * nch);
        c[nch] = 0.5 * c[0];
        for (j = 1; j < nch; j++) {
            c[j] = 0.5 * cos(delta * j);
            c[nc - j] = 0.5 * sin(delta * j);
        }
    }
}


/* -------- child routines -------- */


static void
bitrv2(int n, int *ip, double *a)
{
    int j, j1, k, k1, l, m, m2;
    double xr, xi, yr, yi;

    ip[0] = 0;
    l = n;
    m = 1;
    while ((m << 3) < l) {
        l >>= 1;
        for (j = 0; j < m; j++) {
            ip[m + j] = ip[j] + l;
        }
        m <<= 1;
    }
    m2 = 2 * m;
    if ((m << 3) == l) {
        for (k = 0; k < m; k++) {
            for (j = 0; j < k; j++) {
                j1 = 2 * j + ip[k];
                k1 = 2 * k + ip[j];
                xr = a[j1];
                xi = a[j1 + 1];
                yr = a[k1];
                yi = a[k1 + 1];
                a[j1] = yr;
                a[j1 + 1] = yi;
                a[k1] = xr;
                a[k1 + 1] = xi;
                j1 += m2;
                k1 += 2 * m2;
                xr = a[j1];
                xi = a[j1 + 1];
                yr = a[k1];
                yi = a[k1 + 1];
                a[j1] = yr;
                a[j1 + 1] = yi;
                a[k1] = xr;
                a[k1 + 1] = xi;
                j1 += m2;
                k1 -= m2;
                xr = a[j1];
                xi = a[j1 + 1];
                yr = a[k1];
                yi = a[k1 + 1];
                a[j1] = yr;
                a[j1 + 1] = yi;
                a[k1] = xr;
                a[k1 + 1] = xi;
                j1 += m2;
                k1 += 2 * m2;
                xr = a[j1];
                xi = a[j1 + 1];
                yr = a[k1];
                yi = a[k1 + 1];
                a[j1] = yr;
                a[j1 + 1] = yi;
                a[k1] = xr;
                a[k1 + 1] = xi;
            }
            j1 = 2 * k + m2 + ip[k];
            k1 = j1 + m2;
            xr = a[j1];
            xi = a[j1 + 1];
            yr = a[k1];
            yi = a[k1 + 1];
            a[j1] = yr;
            a[j1 + 1] = yi;
            a[k1] = xr;
            a[k1 + 1] = xi;
        }
    } else {
        for (k = 1; k < m; k++) {
            for (j = 0; j < k; j++) {
                j1 = 2 * j + ip[k];
                k1 = 2 * k + ip[j];
                xr = a[j1];
                xi = a[j1 + 1];
                yr = a[k1];
                yi = a[k1 + 1];
                a[j1] = yr;
                a[j1 + 1] = yi;
                a[k1] = xr;
                a[k1 + 1] = xi;
                j1 += m2;
                k1 += m2;
                xr = a[j1];
                xi = a[j1 + 1];
                yr = a[k1];
                yi = a[k1 + 1];
                a[j1] = yr;
                a[j1 + 1] = yi;
                a[k1] = xr;
                a[k1 + 1] = xi;
            }
        }
    }
}


static void
cftfsub(int n, double *a, double *w)
{
    void cft1st(int n, double *a, double *w);
    void cftmdl(int n, int l, double *a, double *w);
    int j, j1, j2, j3, l;
    double x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

    l = 2;
    if (n > 8) {
        cft1st(n, a, w);
        l = 8;
        while ((l << 2) < n) {
            cftmdl(n, l, a, w);
            l <<= 2;
        }
    }
    if ((l << 2) == n) {
        for (j = 0; j < l; j += 2) {
            j1 = j + l;
            j2 = j1 + l;
            j3 = j2 + l;
            x0r = a[j] + a[j1];
            x0i = a[j + 1] + a[j1 + 1];
            x1r = a[j] - a[j1];
            x1i = a[j + 1] - a[j1 + 1];
            x2r = a[j2] + a[j3];
            x2i = a[j2 + 1] + a[j3 + 1];
            x3r = a[j2] - a[j3];
            x3i = a[j2 + 1] - a[j3 + 1];
            a[j] = x0r + x2r;
            a[j + 1] = x0i + x2i;
            a[j2] = x0r - x2r;
            a[j2 + 1] = x0i - x2i;
            a[j1] = x1r - x3i;
            a[j1 + 1] = x1i + x3r;
            a[j3] = x1r + x3i;
            a[j3 + 1] = x1i - x3r;
        }
    } else {
        for (j = 0; j < l; j += 2) {
            j1 = j + l;
            x0r = a[j] - a[j1];
            x0i = a[j + 1] - a[j1 + 1];
            a[j] += a[j1];
            a[j + 1] += a[j1 + 1];
            a[j1] = x0r;
            a[j1 + 1] = x0i;
        }
    }
}


static void
cftbsub(int n, double *a, double *w)
{
    void cft1st(int n, double *a, double *w);
    void cftmdl(int n, int l, double *a, double *w);
    int j, j1, j2, j3, l;
    double x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

    l = 2;
    if (n > 8) {
        cft1st(n, a, w);
        l = 8;
        while ((l << 2) < n) {
            cftmdl(n, l, a, w);
            l <<= 2;
        }
    }
    if ((l << 2) == n) {
        for (j = 0; j < l; j += 2) {
            j1 = j + l;
            j2 = j1 + l;
            j3 = j2 + l;
            x0r = a[j] + a[j1];
            x0i = -a[j + 1] - a[j1 + 1];
            x1r = a[j] - a[j1];
            x1i = -a[j + 1] + a[j1 + 1];
            x2r = a[j2] + a[j3];
            x2i = a[j2 + 1] + a[j3 + 1];
            x3r = a[j2] - a[j3];
            x3i = a[j2 + 1] - a[j3 + 1];
            a[j] = x0r + x2r;
            a[j + 1] = x0i - x2i;
            a[j2] = x0r - x2r;
            a[j2 + 1] = x0i + x2i;
            a[j1] = x1r - x3i;
            a[j1 + 1] = x1i - x3r;
            a[j3] = x1r + x3i;
            a[j3 + 1] = x1i + x3r;
        }
    } else {
        for (j = 0; j < l; j += 2) {
            j1 = j + l;
            x0r = a[j] - a[j1];
            x0i = -a[j + 1] + a[j1 + 1];
            a[j] += a[j1];
            a[j + 1] = -a[j + 1] - a[j1 + 1];
            a[j1] = x0r;
            a[j1 + 1] = x0i;
        }
    }
}


static void
cft1st(int n, double *a, double *w)
{
    int j, k1, k2;
    double wk1r, wk1i, wk2r, wk2i, wk3r, wk3i;
    double x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

    x0r = a[0] + a[2];
    x0i = a[1] + a[3];
    x1r = a[0] - a[2];
    x1i = a[1] - a[3];
    x2r = a[4] + a[6];
    x2i = a[5] + a[7];
    x3r = a[4] - a[6];
    x3i = a[5] - a[7];
    a[0] = x0r + x2r;
    a[1] = x0i + x2i;
    a[4] = x0r - x2r;
    a[5] = x0i - x2i;
    a[2] = x1r - x3i;
    a[3] = x1i + x3r;
    a[6] = x1r + x3i;
    a[7] = x1i - x3r;
    wk1r = w[2];
    x0r = a[8] + a[10];
    x0i = a[9] + a[11];
    x1r = a[8] - a[10];
    x1i = a[9] - a[11];
    x2r = a[12] + a[14];
    x2i = a[13] + a[15];
    x3r = a[12] - a[14];
    x3i = a[13] - a[15];
    a[8] = x0r + x2r;
    a[9] = x0i + x2i;
    a[12] = x2i - x0i;
    a[13] = x0r - x2r;
    x0r = x1r - x3i;
    x0i = x1i + x3r;
    a[10] = wk1r * (x0r - x0i);
    a[11] = wk1r * (x0r + x0i);
    x0r = x3i + x1r;
    x0i = x3r - x1i;
    a[14] = wk1r * (x0i - x0r);
    a[15] = wk1r * (x0i + x0r);
    k1 = 0;
    for (j = 16; j < n; j += 16) {
        k1 += 2;
        k2 = 2 * k1;
        wk2r = w[k1];
        wk2i = w[k1 + 1];
        wk1r = w[k2];
        wk1i = w[k2 + 1];
        wk3r = wk1r - 2 * wk2i * wk1i;
        wk3i = 2 * wk2i * wk1r - wk1i;
        x0r = a[j] + a[j + 2];
        x0i = a[j + 1] + a[j + 3];
        x1r = a[j] - a[j + 2];
        x1i = a[j + 1] - a[j + 3];
        x2r = a[j + 4] + a[j + 6];
        x2i = a[j + 5] + a[j + 7];
        x3r = a[j + 4] - a[j + 6];
        x3i = a[j + 5] - a[j + 7];
        a[j] = x0r + x2r;
        a[j + 1] = x0i + x2i;
        x0r -= x2r;
        x0i -= x2i;
        a[j + 4] = wk2r * x0r - wk2i * x0i;
        a[j + 5] = wk2r * x0i + wk2i * x0r;
        x0r = x1r - x3i;
        x0i = x1i + x3r;
        a[j + 2] = wk1r * x0r - wk1i * x0i;
        a[j + 3] = wk1r * x0i + wk1i * x0r;
        x0r = x1r + x3i;
        x0i = x1i - x3r;
        a[j + 6] = wk3r * x0r - wk3i * x0i;
        a[j + 7] = wk3r * x0i + wk3i * x0r;
        wk1r = w[k2 + 2];
        wk1i = w[k2 + 3];
        wk3r = wk1r - 2 * wk2r * wk1i;
        wk3i = 2 * wk2r * wk1r - wk1i;
        x0r = a[j + 8] + a[j + 10];
        x0i = a[j + 9] + a[j + 11];
        x1r = a[j + 8] - a[j + 10];
        x1i = a[j + 9] - a[j + 11];
        x2r = a[j + 12] + a[j + 14];
        x2i = a[j + 13] + a[j + 15];
        x3r = a[j + 12] - a[j + 14];
        x3i = a[j + 13] - a[j + 15];
        a[j + 8] = x0r + x2r;
        a[j + 9] = x0i + x2i;
        x0r -= x2r;
        x0i -= x2i;
        a[j + 12] = -wk2i * x0r - wk2r * x0i;
        a[j + 13] = -wk2i * x0i + wk2r * x0r;
        x0r = x1r - x3i;
        x0i = x1i + x3r;
        a[j + 10] = wk1r * x0r - wk1i * x0i;
        a[j + 11] = wk1r * x0i + wk1i * x0r;
        x0r = x1r + x3i;
        x0i = x1i - x3r;
        a[j + 14] = wk3r * x0r - wk3i * x0i;
        a[j + 15] = wk3r * x0i + wk3i * x0r;
    }
}


static void
cftmdl(int n, int l, double *a, double *w)
{
    int j, j1, j2, j3, k, k1, k2, m, m2;
    double wk1r, wk1i, wk2r, wk2i, wk3r, wk3i;
    double x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

    m = l << 2;
    for (j = 0; j < l; j += 2) {
        j1 = j + l;
        j2 = j1 + l;
        j3 = j2 + l;
        x0r = a[j] + a[j1];
        x0i = a[j + 1] + a[j1 + 1];
        x1r = a[j] - a[j1];
        x1i = a[j + 1] - a[j1 + 1];
        x2r = a[j2] + a[j3];
        x2i = a[j2 + 1] + a[j3 + 1];
        x3r = a[j2] - a[j3];
        x3i = a[j2 + 1] - a[j3 + 1];
        a[j] = x0r + x2r;
        a[j + 1] = x0i + x2i;
        a[j2] = x0r - x2r;
        a[j2 + 1] = x0i - x2i;
        a[j1] = x1r - x3i;
        a[j1 + 1] = x1i + x3r;
        a[j3] = x1r + x3i;
        a[j3 + 1] = x1i - x3r;
    }
    wk1r = w[2];
    for (j = m; j < l + m; j += 2) {
        j1 = j + l;
        j2 = j1 + l;
        j3 = j2 + l;
        x0r = a[j] + a[j1];
        x0i = a[j + 1] + a[j1 + 1];
        x1r = a[j] - a[j1];
        x1i = a[j + 1] - a[j1 + 1];
        x2r = a[j2] + a[j3];
        x2i = a[j2 + 1] + a[j3 + 1];
        x3r = a[j2] - a[j3];
        x3i = a[j2 + 1] - a[j3 + 1];
        a[j] = x0r + x2r;
        a[j + 1] = x0i + x2i;
        a[j2] = x2i - x0i;
        a[j2 + 1] = x0r - x2r;
        x0r = x1r - x3i;
        x0i = x1i + x3r;
        a[j1] = wk1r * (x0r - x0i);
        a[j1 + 1] = wk1r * (x0r + x0i);
        x0r = x3i + x1r;
        x0i = x3r - x1i;
        a[j3] = wk1r * (x0i - x0r);
        a[j3 + 1] = wk1r * (x0i + x0r);
    }
    k1 = 0;
    m2 = 2 * m;
    for (k = m2; k < n; k += m2) {
        k1 += 2;
        k2 = 2 * k1;
        wk2r = w[k1];
        wk2i = w[k1 + 1];
        wk1r = w[k2];
        wk1i = w[k2 + 1];
        wk3r = wk1r - 2 * wk2i * wk1i;
        wk3i = 2 * wk2i * wk1r - wk1i;
        for (j = k; j < l + k; j += 2) {
            j1 = j + l;
            j2 = j1 + l;
            j3 = j2 + l;
            x0r = a[j] + a[j1];
            x0i = a[j + 1] + a[j1 + 1];
            x1r = a[j] - a[j1];
            x1i = a[j + 1] - a[j1 + 1];
            x2r = a[j2] + a[j3];
            x2i = a[j2 + 1] + a[j3 + 1];
            x3r = a[j2] - a[j3];
            x3i = a[j2 + 1] - a[j3 + 1];
            a[j] = x0r + x2r;
            a[j + 1] = x0i + x2i;
            x0r -= x2r;
            x0i -= x2i;
            a[j2] = wk2r * x0r - wk2i * x0i;
            a[j2 + 1] = wk2r * x0i + wk2i * x0r;
            x0r = x1r - x3i;
            x0i = x1i + x3r;
            a[j1] = wk1r * x0r - wk1i * x0i;
            a[j1 + 1] = wk1r * x0i + wk1i * x0r;
            x0r = x1r + x3i;
            x0i = x1i - x3r;
            a[j3] = wk3r * x0r - wk3i * x0i;
            a[j3 + 1] = wk3r * x0i + wk3i * x0r;
        }
        wk1r = w[k2 + 2];
        wk1i = w[k2 + 3];
        wk3r = wk1r - 2 * wk2r * wk1i;
        wk3i = 2 * wk2r * wk1r - wk1i;
        for (j = k + m; j < l + (k + m); j += 2) {
            j1 = j + l;
            j2 = j1 + l;
            j3 = j2 + l;
            x0r = a[j] + a[j1];
            x0i = a[j + 1] + a[j1 + 1];
            x1r = a[j] - a[j1];
            x1i = a[j + 1] - a[j1 + 1];
            x2r = a[j2] + a[j3];
            x2i = a[j2 + 1] + a[j3 + 1];
            x3r = a[j2] - a[j3];
            x3i = a[j2 + 1] - a[j3 + 1];
            a[j] = x0r + x2r;
            a[j + 1] = x0i + x2i;
            x0r -= x2r;
            x0i -= x2i;
            a[j2] = -wk2i * x0r - wk2r * x0i;
            a[j2 + 1] = -wk2i * x0i + wk2r * x0r;
            x0r = x1r - x3i;
            x0i = x1i + x3r;
            a[j1] = wk1r * x0r - wk1i * x0i;
            a[j1 + 1] = wk1r * x0i + wk1i * x0r;
            x0r = x1r + x3i;
            x0i = x1i - x3r;
            a[j3] = wk3r * x0r - wk3i * x0i;
            a[j3 + 1] = wk3r * x0i + wk3i * x0r;
        }
    }
}


static void
rftfsub(int n, double *a, int nc, double *c)
{
    int j, k, kk, ks, m;
    double wkr, wki, xr, xi, yr, yi;

    m = n >> 1;
    ks = 2 * nc / m;
    kk = 0;
    for (j = 2; j < m; j += 2) {
        k = n - j;
        kk += ks;
        wkr = 0.5 - c[nc - kk];
        wki = c[kk];
        xr = a[j] - a[k];
        xi = a[j + 1] + a[k + 1];
        yr = wkr * xr - wki * xi;
        yi = wkr * xi + wki * xr;
        a[j] -= yr;
        a[j + 1] -= yi;
        a[k] += yr;
        a[k + 1] -= yi;
    }
}


static void
rftbsub(int n, double *a, int nc, double *c)
{
    int j, k, kk, ks, m;
    double wkr, wki, xr, xi, yr, yi;

    a[1] = -a[1];
    m = n >> 1;
    ks = 2 * nc / m;
    kk = 0;
    for (j = 2; j < m; j += 2) {
        k = n - j;
        kk += ks;
        wkr = 0.5 - c[nc - kk];
        wki = c[kk];
        xr = a[j] - a[k];
        xi = a[j + 1] + a[k + 1];
        yr = wkr * xr + wki * xi;
        yi = wkr * xi - wki * xr;
        a[j] -= yr;
        a[j + 1] = yi - a[j + 1];
        a[k] += yr;
        a[k + 1] = yi - a[k + 1];
    }
    a[m + 1] = -a[m + 1];
}

#endif