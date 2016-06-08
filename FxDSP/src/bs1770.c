//
//  bs1770.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/27/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "bs1770.h"
#include "BiquadFilter.h"
#include "CircularBuffer.h"
#include "Dsp.h"
#include "Upsampler.h"
#include "Utilities.h"
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#define PREFILTER_FC    (1500.12162162162167078350)
#define PREFILTER_GAIN  (3.99976976976976983380)
#define PREFILTER_Q     (sqrt(2.0)/2.0) // Close enough....
#define RLBFILTER_FC    (40.2802802803)
#define RLBFILTER_Q     (0.92792792793)
#define GATE_LENGTH_S   (0.4)
#define GATE_OVERLAP    (0.75)

/* Calculate BS.1770 prefilter coefficients for a given sample rate */
static void
calc_prefilter(float* b, float* a, float sample_rate);

static void
calc_prefilterD(double* b, double* a, double sample_rate);

/* Calculate BS.1770 rlb filter coefficients for a given sample rate */
static void
calc_rlbfilter(float* b, float* a, float sample_rate);

static void
calc_rlbfilterD(double* b, double* a, double sample_rate);


/* Channel id numbers */
enum
{
    LEFT = 0,
    RIGHT,
    CENTER,
    LEFT_SURROUND,
    RIGHT_SURROUND,
    N_CHANNELS
};


double CHANNEL_GAIN[N_CHANNELS] =
{
    1.0,    /* LEFT */
    1.0,    /* RIGHT */
    1.0,    /* CENTER */
    1.41,   /* LEFT_SURROUND */
    1.41    /* RIGHT_SURROUND */
};


struct KWeightingFilter
{
    BiquadFilter*   pre_filter;
    BiquadFilter*   rlb_filter;
};


struct KWeightingFilterD
{
    BiquadFilterD*  pre_filter;
    BiquadFilterD*  rlb_filter;
};


struct BS1770Meter
{
    KWeightingFilter**  filters;
    Upsampler**         upsamplers;
    CircularBuffer**    buffers;
    unsigned            n_channels;
    unsigned            sample_count;
    unsigned            gate_len;
    unsigned            overlap_len;
};



struct BS1770MeterD
{
    KWeightingFilterD** filters;
    UpsamplerD**        upsamplers;
    CircularBufferD**   buffers;
    unsigned            n_channels;
    unsigned            sample_count;
    unsigned            gate_len;
    unsigned            overlap_len;
};



KWeightingFilter*
KWeightingFilterInit(float sample_rate)
{
    float b[3] = {0.};
    float a[2] = {0.};

    KWeightingFilter* filter = (KWeightingFilter*)malloc(sizeof(KWeightingFilter));

    if (filter)
    {
        calc_prefilter(b, a, sample_rate);
        filter->pre_filter = BiquadFilterInit(b, a);
        calc_rlbfilter(b, a, sample_rate);
        filter->rlb_filter = BiquadFilterInit(b, a);
    }

    return filter;
}

KWeightingFilterD*
KWeightingFilterInitD(double sample_rate)
{
    double b[3] = {0.};
    double a[2] = {0.};

    KWeightingFilterD* filter = (KWeightingFilterD*)malloc(sizeof(KWeightingFilterD));

    if (filter)
    {
        calc_prefilterD(b, a, sample_rate);
        filter->pre_filter = BiquadFilterInitD(b, a);
        calc_rlbfilterD(b, a, sample_rate);
        filter->rlb_filter = BiquadFilterInitD(b, a);
    }

    return filter;
}


Error_t
KWeightingFilterProcess(KWeightingFilter*   filter,
                        float*              dest,
                        const float*        src,
                        unsigned            length)
{
    float scratch[length];
    BiquadFilterProcess(filter->pre_filter, scratch, src, length);
    BiquadFilterProcess(filter->rlb_filter, dest, (const float*)scratch, length);
    return NOERR;
}


Error_t
KWeightingFilterProcessD(KWeightingFilterD* filter,
                         double*            dest,
                         const double*      src,
                         unsigned           length)
{
    double scratch[length];
    BiquadFilterProcessD(filter->pre_filter, scratch, src, length);
    BiquadFilterProcessD(filter->rlb_filter, dest, (const double*)scratch, length);
    return NOERR;
}

Error_t
KWeightingFilterFlush(KWeightingFilter* filter)
{
    if (filter)
    {
        BiquadFilterFlush(filter->pre_filter);
        BiquadFilterFlush(filter->rlb_filter);
        return NOERR;
    }
    return NULL_PTR_ERROR;
}

Error_t
KWeightingFilterFlushD(KWeightingFilterD* filter)
{
    if (filter)
    {
        BiquadFilterFlushD(filter->pre_filter);
        BiquadFilterFlushD(filter->rlb_filter);
        return NOERR;
    }
    return NULL_PTR_ERROR;
}


Error_t
KWeightingFilterFree(KWeightingFilter* filter)
{
    if (filter)
    {
        BiquadFilterFree(filter->pre_filter);
        BiquadFilterFree(filter->rlb_filter);
        free(filter);
        filter = NULL;
    }
    return NOERR;
}

Error_t
KWeightingFilterFreeD(KWeightingFilterD* filter)
{
    if (filter)
    {
        BiquadFilterFreeD(filter->pre_filter);
        BiquadFilterFreeD(filter->rlb_filter);
        free(filter);
        filter = NULL;
    }
    return NOERR;
}


BS1770Meter*
BS1770MeterInit(unsigned n_channels, float sample_rate)
{
    BS1770Meter* meter = (BS1770Meter*)malloc(sizeof(BS1770Meter));
    KWeightingFilter** filters = (KWeightingFilter**)malloc(n_channels * sizeof(KWeightingFilter*));
    Upsampler** upsamplers = (Upsampler**)malloc(n_channels * sizeof(Upsampler*));
    CircularBuffer** buffers = (CircularBuffer**)malloc(n_channels * sizeof(CircularBuffer*));
    if (meter && filters && upsamplers && buffers)
    {
        for (unsigned i = 0; i < n_channels; ++i)
        {
            filters[i] = KWeightingFilterInit(sample_rate);
            upsamplers[i] = UpsamplerInit(X4);
            buffers[i] = CircularBufferInit((unsigned)(2 * GATE_LENGTH_S * sample_rate));
        }

        meter->sample_count = 0;
        meter->n_channels = n_channels;
        meter->gate_len = (unsigned)(GATE_LENGTH_S * sample_rate);
        meter->overlap_len = (unsigned)(GATE_OVERLAP * meter->gate_len);
        meter->filters= filters;
        meter->upsamplers = upsamplers;
        meter->buffers = buffers;
    }
    else
    {
        if (meter)
        {
            free(meter);
        }

        if (filters)
        {
            free(filters);
        }

        if (upsamplers)
        {
            free(upsamplers);
        }
        if (buffers)
        {
            free(buffers);
        }
        return NULL;
    }
    return meter;
}

BS1770MeterD*
BS1770MeterInitD(unsigned n_channels, double sample_rate)
{
    BS1770MeterD* meter = (BS1770MeterD*)malloc(sizeof(BS1770MeterD));
    KWeightingFilterD** filters = (KWeightingFilterD**)malloc(n_channels * sizeof(KWeightingFilterD*));
    UpsamplerD** upsamplers = (UpsamplerD**)malloc(n_channels * sizeof(UpsamplerD*));
    CircularBufferD** buffers = (CircularBufferD**)malloc(n_channels * sizeof(CircularBufferD*));

    if (meter && filters && upsamplers && buffers)
    {
        for (unsigned i = 0; i < n_channels; ++i)
        {
            filters[i] = KWeightingFilterInitD(sample_rate);
            upsamplers[i] = UpsamplerInitD(X4);
            buffers[i] = CircularBufferInitD((unsigned)(2 * GATE_LENGTH_S * sample_rate));
        }

        meter->sample_count = 0;
        meter->n_channels = n_channels;
        meter->gate_len = (unsigned)(GATE_LENGTH_S * sample_rate);
        meter->overlap_len = (unsigned)(GATE_OVERLAP * meter->gate_len);
        meter->filters= filters;
        meter->upsamplers = upsamplers;
        meter->buffers = buffers;

    }
    else
    {
        if (meter)
        {
            free(meter);
        }

        if (filters)
        {
            free(filters);
        }

        if (upsamplers)
        {
            free(upsamplers);
        }
        if (buffers)
        {
            free(buffers);
        }
        return NULL;
    }
    return meter;
}



Error_t
BS1770MeterProcess(BS1770Meter*     meter,
                   float*           loudness,
                   float**          peaks,
                   const float**    samples,
                   unsigned         n_samples)
{
    unsigned os_length = 4 * n_samples;
    float filtered[n_samples];
    float os_sig[os_length];
    float gate[meter->gate_len];
    float sum = 0.0;

    if (meter)
    {
        *loudness = 0.0;

        for (unsigned i = 0; i < meter->n_channels; ++i)
        {
            // Calculate peak for each channel
            UpsamplerProcess(meter->upsamplers[i], os_sig, samples[i], n_samples);
            VectorAbs(os_sig, (const float*)os_sig, os_length);
            *peaks[i] = AmpToDb(VectorMax(os_sig, os_length));

            KWeightingFilterProcess(meter->filters[i], filtered, samples[i], n_samples);
            CircularBufferWrite(meter->buffers[i], (const float*)filtered, n_samples);

            if (CircularBufferCount(meter->buffers[i]) >= meter->gate_len)
            {
                CircularBufferRead(meter->buffers[i], gate, meter->gate_len);
                CircularBufferRewind(meter->buffers[i], meter->overlap_len);
                sum += CHANNEL_GAIN[i] * MeanSquare(gate, meter->gate_len);
            }
        }

        *loudness = -0.691 + 10 * log10f(sum);
        return NOERR;
    }
    return NULL_PTR_ERROR;
}

Error_t
BS1770MeterProcessD(BS1770MeterD*   meter,
                    double*         loudness,
                    double**        peaks,
                    const double**  samples,
                    unsigned        n_samples)
{
    unsigned os_length = 4 * n_samples;
    double filtered[n_samples];
    double os_sig[os_length];
    double gate[meter->gate_len];
    double sum = 0.0;

    if (meter)
    {
        *loudness = 0.0;

        for (unsigned i = 0; i < meter->n_channels; ++i)
        {
            // Calculate peak for each channel
            UpsamplerProcessD(meter->upsamplers[i], os_sig, samples[i], n_samples);
            VectorAbsD(os_sig, (const double*)os_sig, os_length);
            *peaks[i] = AmpToDbD(VectorMaxD(os_sig, os_length));

            KWeightingFilterProcessD(meter->filters[i], filtered, samples[i], n_samples);
            CircularBufferWriteD(meter->buffers[i], (const double*)filtered, n_samples);

            if (CircularBufferCountD(meter->buffers[i]) >= meter->gate_len)
            {
                CircularBufferReadD(meter->buffers[i], gate, meter->gate_len);
                CircularBufferRewindD(meter->buffers[i], meter->overlap_len);
                sum += CHANNEL_GAIN[i] * MeanSquareD(gate, meter->gate_len);
            }
        }

        *loudness = -0.691 + 10 * log10(sum);
        return NOERR;
    }
    return NULL_PTR_ERROR;
}


Error_t
BS1770MeterFree(BS1770Meter* meter)
{
    if (meter)
    {
        for (unsigned ch = 0; ch < meter->n_channels; ++ch)
        {
            if (meter->filters)
            {
                KWeightingFilterFree(meter->filters[ch]);
            }
            if (meter->buffers)
            {
                CircularBufferFree(meter->buffers[ch]);
            }
            if (meter->upsamplers)
            {
                UpsamplerFree(meter->upsamplers[ch]);
            }
        }
        free(meter);
        return NOERR;
    }
    return NULL_PTR_ERROR;
}

Error_t
BS1770MeterFreeD(BS1770MeterD* meter)
{
    if (meter)
    {
        for (unsigned ch = 0; ch < meter->n_channels; ++ch)
        {
            if (meter->filters)
            {
                KWeightingFilterFreeD(meter->filters[ch]);
            }
            if (meter->buffers)
            {
                CircularBufferFreeD(meter->buffers[ch]);
            }
            if (meter->upsamplers)
            {
                UpsamplerFreeD(meter->upsamplers[ch]);
            }
        }
        free(meter);
        return NOERR;
    }
    return NULL_PTR_ERROR;
}


static void
calc_prefilter(float* b, float* a, float sample_rate)
{

    if (sample_rate == 48000)
    {
        // Use exact coefficients from BS.1770
        b[0] = 1.53512485958697;
        b[1] = -2.69169618940638;
        b[2] = 1.19839281085285;

        a[0] = -1.69065929318241;
        a[1] = 0.73248077421585;
    }

    else
    {

        // Calculate prefilter as a high shelf using RBJ formula with the
        // following params:
        //    Fc = 1650
        //    Q = 0.807348173842115   (gives close approximation at Fs=48000)
        //    G = 4 (dB)
        float A = sqrtf(powf(10., PREFILTER_GAIN/20.));
        float wc = 2 * M_PI * PREFILTER_FC / sample_rate;
        float wS = sinf(wc);
        float wC = cosf(wc);
        float beta = sqrtf(A) / PREFILTER_Q;

        // Normalize filter by a[0]
        float norm = (A+1) - ((A - 1) * wC) + (beta * wS);

        a[0] = (2 * ((A - 1) - ((A + 1) * wC))) / norm;
        a[1] = ((A+1) - ((A - 1) * wC) - (beta * wS)) / norm;
        b[0] = (A * ((A + 1) + ((A - 1) * wC) + (beta * wS))) / norm;
        b[1] = (-2 * A * ((A - 1) + ((A + 1) * wC))) / norm;
        b[2] = (A * ((A + 1) + ((A - 1) * wC) - (beta * wS))) / norm;

    }

}

static void
calc_prefilterD(double* b, double* a, double sample_rate)
{
    if (sample_rate == 48000)
    {
        // Use exact coefficients from BS.1770
        b[0] = 1.53512485958697;
        b[1] = -2.69169618940638;
        b[2] = 1.19839281085285;

        a[0] = -1.69065929318241;
        a[1] = 0.73248077421585;
    }

    else
    {
        // Calculate prefilter as a high shelf using RBJ formula with the
        // following params:
        //    Fc = 1650
        //    Q = 0.807348173842115   (gives close approximation at Fs=4800)
        //    G = 4 (dB)
        double A = sqrt(pow(10., PREFILTER_GAIN / 20.));
        double wc = 2 * M_PI * PREFILTER_FC / sample_rate;
        double wS = sin(wc);
        double wC = cos(wc);
        double beta = sqrt(A) / PREFILTER_Q;

        // Normalize filter by a[0]
        double norm = (A+1) - ((A - 1) * wC) + (beta * wS);

        a[0] = (2 * ((A - 1) - ((A + 1) * wC))) / norm;
        a[1] = ((A+1) - ((A - 1) * wC) - (beta * wS)) / norm;
        b[0] = (A * ((A + 1) + ((A - 1) * wC) + (beta * wS))) / norm;
        b[1] = (-2 * A * ((A - 1) + ((A + 1) * wC))) / norm;
        b[2] = (A * ((A + 1) + ((A - 1) * wC) - (beta * wS))) / norm;
    }
}

static void
calc_rlbfilter(float* b, float* a, float sample_rate)
{

    if (sample_rate == 48000)
    {
        // Use exact coefficients from BS.1770
        b[0] = 1.0;
        b[1] = -2.0;
        b[2] = 1.0;

        a[0] = -1.99004745483398;
        a[1] = 0.99007225036621;
    }

    else
    {

        // Calculate as highpass using RLB formula
        float wc = 2 * M_PI * RLBFILTER_FC / sample_rate;
        float wS = sinf(wc);
        float wC = cosf(wc);
        float alpha = wS / (2.0 * RLBFILTER_Q);

        float norm = 1 + alpha;

        b[0] = ((1 + wC) / 2.0) / norm;
        b[1] = (-(1 + wC)) / norm;
        b[2] = ((1 + wC) / 2.0) / norm;

        a[0] = (-2 * wC) / norm;
        a[1] = (1 - alpha) / norm;

    }

}

static void
calc_rlbfilterD(double* b, double* a, double sample_rate)
{
    if (sample_rate == 48000)
    {
        // Use exact coefficients from BS.1770
        b[0] = 1.0;
        b[1] = -2.0;
        b[2] = 1.0;

        a[0] = -1.99004745483398;
        a[1] = 0.99007225036621;
    }

    else
    {
        // Calculate as highpass using RLB formula
        double wc = 2 * M_PI * RLBFILTER_FC / sample_rate;
        double wS = sin(wc);
        double wC = cos(wc);
        double alpha = wS / (2.0 * RLBFILTER_Q);

        double norm = 1 + alpha;

        b[0] = ((1 + wC) / 2.0) / norm;
        b[1] = (-(1 + wC)) / norm;
        b[2] = ((1 + wC) / 2.0) / norm;

        a[0] = (-2 * wC) / norm;
        a[1] = (1 - alpha) / norm;
    }
}
