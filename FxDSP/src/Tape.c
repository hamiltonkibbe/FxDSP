//
//  TapeSaturator.c
//  FxDSP
//
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "Tape.h"
#include "PolySaturator.h"
#include "Dsp.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// The higher this value is, the more the saturation amount is affected by tape
// speed
#define SPEED_SATURATION_COEFF (0.0)

#define N_FLUTTER_COMPONENTS (11)
// Base frequency components for flutter sources of a Studer A820 (@ 3.75IPS)
// REF: http://www.manquen.net/audio/docs/Flutter%20database%2002-8-28.htm
static const float flutterRateBase[N_FLUTTER_COMPONENTS] =
{
    1.01,   // Outer Tension Sense
    2.52,   // Inner Tension Sense
    0.80,   // Pre-stabilizer
    1.01,   // Left lifter
    3.11,   // Record scrape idler
    3.33,   // Capstan
    0.81,   // Pinch roller
    1.01,   // Right lifter
    0.80,   // Motion Sensor
    2.52,   // Inner tension sense
    1.01    // Outer tension sense
};


static inline float
calculate_n(float saturation, TapeSpeed speed)
{
    // Tape speed dependent saturation.
    float n = ((50 * (1-SPEED_SATURATION_COEFF)) + \
            ((unsigned)speed * 50 * SPEED_SATURATION_COEFF)) \
    * powf((1.0075 - saturation), 2.);
    printf("N: %1.5f\n", n);
    return n;
}

/*******************************************************************************
 TapeSaturator */
struct Tape
{
    PolySaturator*  polysat;
    TapeSpeed       speed;
    float           sample_rate;
    float           saturation;
    float           hysteresis;
    float           flutter;
    float           pos_peak;
    float           neg_peak;
    float*          flutter_mod;
    unsigned        flutter_mod_length;
};



/*******************************************************************************
TapeInit */
Tape*
TapeInit(TapeSpeed speed, float saturation, float hysteresis, float flutter, float sample_rate)
{
    // Create TapeSaturator Struct
    Tape* tape = (Tape*)malloc(sizeof(Tape));
    PolySaturator* saturator = PolySaturatorInit(1);
    // Allocate for longest period...
    unsigned mod_length = (unsigned)(sample_rate / 0.80);
    float* mod = (float*)malloc(mod_length * sizeof(float));
    if (tape && saturator && mod)
    {
        // Initialization
        tape->polysat = saturator;
        tape->sample_rate = sample_rate;
        tape->pos_peak = 0.0;
        tape->neg_peak = 0.0;
        tape->flutter_mod = mod;
        tape->flutter_mod_length = mod_length;
        
        // Need these initialized here.
        tape->speed = speed;
        tape->saturation = saturation;
        
        // Set up
        TapeSetFlutter(tape, flutter);
        TapeSetSaturation(tape, saturation);
        TapeSetSpeed(tape, speed);
        TapeSetHysteresis(tape, hysteresis);
        return tape;
    }
    else
    {
        free(mod);
        free(tape);
        free(saturator);
        return NULL;
    }
}


/*******************************************************************************
 Tape Free */
Error_t
TapeFree(Tape* tape)
{
    if(tape)
        free(tape);
    tape = NULL;
    return NOERR;
}


/*******************************************************************************
 Set Speed */
Error_t
TapeSetSpeed(Tape* tape, TapeSpeed speed)
{
    
    if (tape)
    {
        // Set speed
        tape->speed = speed;
        
        // Update saturation curve
        PolySaturatorSetN(tape->polysat, calculate_n(tape->saturation, speed));
        
        // Clear old flutter/wow modulation waveform
        ClearBuffer(tape->flutter_mod, tape->flutter_mod_length); // Yes, clear the old length...
        
        // Calculate new modulation waveform length...
        tape->flutter_mod_length = (unsigned)(tape->sample_rate / \
                                              (0.80 * powf(2.0, (float)speed)));
        
        // Generate flutter/wow modulation waveform
        float temp_buffer[tape->flutter_mod_length];
        for (unsigned comp = 0; comp < N_FLUTTER_COMPONENTS; ++comp)
        {
            float phase_step = (2.0 * M_PI * comp * powf(2.0, (float)speed)) / tape->sample_rate;
            ClearBuffer(temp_buffer, tape->flutter_mod_length);
            for (unsigned i = 0; i < tape->flutter_mod_length; ++i)
            {
                temp_buffer[i] = sinf(i * phase_step) / N_FLUTTER_COMPONENTS;
            }
            VectorVectorAdd(tape->flutter_mod, tape->flutter_mod,
                            temp_buffer, tape->flutter_mod_length);
        }
        return NOERR;;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}

/*******************************************************************************
Set Saturation */
Error_t
TapeSetSaturation(Tape* tape, float saturation)
{

    if (tape)
    {
        float n = calculate_n(saturation, tape->speed);
        tape->saturation = saturation;
        return PolySaturatorSetN(tape->polysat, n);
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}


/*******************************************************************************
 Set Hysteresis */
Error_t
TapeSetHysteresis(Tape* tape, float hysteresis)
{
    if (tape)
    {
        tape->hysteresis = hysteresis;
        return NOERR;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}


/*******************************************************************************
 Set Flutter */
Error_t
TapeSetFlutter(Tape* tape, float flutter)
{
    if (tape)
    {
        tape->flutter = flutter;
        return NOERR;
    }
    else
    {
        return NULL_PTR_ERROR;
    }
}

/*******************************************************************************
 Get Saturation */
float
TapeGetSaturation(Tape* tape)
{
    if (tape)
    {
        return tape->saturation;
    }
    else
    {
        return -1.0;
    }
}

/*******************************************************************************
 Get Hysteresis */
float
TapeGetHysteresis(Tape* tape)
{
    if (tape)
    {
        return tape->hysteresis;
    }
    else
    {
        return -1.0;
    }
}

/*******************************************************************************
 TapeProcess */
Error_t
TapeProcess(Tape*           tape,
            float*          out_buffer,
            const float*    in_buffer,
            unsigned        n_samples)
{
    return NOERR;
}



/*******************************************************************************
TapeTick */
float
TapeTick(Tape* tape, float in_sample)
{
    
    float hysteresis = tape->hysteresis * 0.05;
    float output = 0.0;
    if (in_sample >= 0)
    {
        tape->neg_peak = 0.0;
        if (in_sample > tape->pos_peak)
        {
            tape->pos_peak = in_sample;
            output = in_sample;
        }
        else if (in_sample > (1 - hysteresis) * tape->pos_peak)
        {
            output = tape->pos_peak;
        }
        else
        {
            output = in_sample + hysteresis * tape->pos_peak;
        }
    }
    else
    {
        tape->pos_peak = 0.0;
        if (in_sample < tape->neg_peak)
        {
            tape->neg_peak = in_sample;
            output = in_sample;
        }
        
        else if (in_sample < (1 - hysteresis) * tape->neg_peak)
        {
            output = tape->neg_peak;
        }
        
        else
        {
            output = in_sample + hysteresis * tape->neg_peak;
        }
    }
    return PolySaturatorTick(tape->polysat, output);
}

