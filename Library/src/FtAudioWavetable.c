//
//  FtAudioWavetable.c
//  Test
//
//  Created by Hamilton Kibbe on 5/17/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "FtAudioWavetable.h"
#include "FtAudioMidiUtils.h"

float fs = 48000;
float nyquist = 24000;

/* FTA_Wavetable ***********************************************************/
struct FTA_Wavetable
{
    char        wave_name[64];
    WaveType_t  wave_type;
    unsigned    table_sizes[NUMBER_OF_TABLES];
    float       table_frequencies[NUMBER_OF_TABLES];
    float *     wavetables[NUMBER_OF_TABLES];
};

/* WavetableGenerators ********************************************************/
WavetableGenerator wavetableGenerators[3] =
{
    &generateSquareTable,
    &generateSawTable,
    &generateTriangleTable
};


/* FTA_WavetableInit *******************************************************/
FTA_Wavetable *
FTA_WavetableInit(WaveType_t type, float sampleRate)
{
    // Create a wavetable
    FTA_Wavetable* wavetable = malloc(sizeof(FTA_Wavetable));
    
    // set type
    wavetable->wave_type = type;
    
    // Assign wavetable type
    switch (type)
    {
        case SQUARE:
            strcpy(wavetable->wave_name, "Square");
            break;
            
        case SAWTOOTH:
            strcpy(wavetable->wave_name, "Sawtooth");
            break;
        
        case TRIANGLE:
            strcpy(wavetable->wave_name, "Triange");
            break;
            
        case OTHER:
            return NULL;
            break;
            
        case N_WAVE_TYPES:
            return NULL;
            break;
    }
    
    // Loop through each midi note and create a table for it
    for (unsigned freq_idx = 0; freq_idx < NUMBER_OF_TABLES; ++freq_idx)
    {
        // Get frequency from midi note
        float frequency = midiNoteToFrequency(freq_idx);
        wavetable->table_frequencies[freq_idx] = frequency;
        
        // Generate the table
        wavetable->wavetables[freq_idx] = wavetableGenerators[type](frequency, sampleRate, *(wavetable->table_sizes + freq_idx));
    }
    return wavetable;
}


/* FTA_CustomWavetableInit *************************************************/
FTA_Wavetable *
FTA_CustomWavetableInit(WavetableGenerator generator, const char* waveName, float sampleRate)
{
    // Create a wavetable
    FTA_Wavetable* wavetable = malloc(sizeof(FTA_Wavetable));
    
    // Assign wavetable type
    wavetable->wave_type = OTHER;
    strcpy(wavetable->wave_name, waveName);
    
    
    // Loop through each midi note and create a table for it
    for (unsigned freq_idx = 0; freq_idx < NUMBER_OF_TABLES; ++freq_idx)
    {
        // Get frequency from midi note
        float frequency = midiNoteToFrequency(freq_idx);
        wavetable->table_frequencies[freq_idx] = frequency;
        
        // Generate the table
        wavetable->wavetables[freq_idx] = generator(frequency, sampleRate, *(wavetable->table_sizes + freq_idx));
    }
    return wavetable;
}



FTA_Error_t
FTA_WavetableFree(FTA_Wavetable *table)
{
    for (unsigned i = 0; i < NUMBER_OF_TABLES; ++i)
    {
        free(table->wavetables[i]);
    }
    return FT_NOERR;
}


float *
generateSquareTable(float frequency, float sampleRate, unsigned length)
{
    // Pre-calculate some shiz
    float fNyquist = sampleRate / 2;
    float cycle_length_s = sampleRate / frequency;
    
    // Allocate memory for the wavetable
    float* wave = malloc((unsigned)floor(cycle_length_s) * sizeof(float));
    
    // Generate each sample
    for (unsigned phase_tick = 0; phase_tick < cycle_length_s; ++phase_tick)
    {
        float sample = 0;
        float T = (phase_tick / sampleRate);    // Time in seconds
        unsigned k = 1;                         // Harmonic counter
        
        // Calculate the sample from the fourier series up to nyquist
        while ((k * frequency) < fNyquist)
        {
            sample += sin(2 * M_PI * frequency * k * T) / k;
            k += 2;
        }
        
        // Scale the sample and store it in the table
        wave[phase_tick] = (sample * (M_2_PI + M_2_PI));
    }
    length = (unsigned)(floor(cycle_length_s));
    return wave;
}



float *
generateSawTable(float frequency, float sampleRate,unsigned length)
{
    // Pre-calculate some shiz
    float fNyquist = sampleRate / 2;
    float cycle_length_s = sampleRate / frequency;
    
    // Allocate memory for the wavetable
    float* wave = malloc((unsigned)floor(cycle_length_s) * sizeof(float));
    
    // Generate each sample
    for (unsigned phase_tick = 0; phase_tick < cycle_length_s; ++phase_tick)
    {
        float sample = 0;
        float T = (phase_tick / sampleRate);    // Time in seconds
        unsigned k = 1;                         // Harmonic counter
        
        // Calculate the sample from the fourier series up to nyquist
        while ((k * frequency) < fNyquist)
        {
            sample += (powf(-1.0, (k - 1)) * (sin(2 * M_PI * frequency * k * T) / k));
            k++;
        }
        
        // Scale the sample and store it in the table
        wave[phase_tick] = (sample * M_2_PI);
    }
    length = (unsigned)(floor(cycle_length_s));
    return wave;
}


float *
generateTriangleTable(float frequency, float sampleRate, unsigned length)
{
    // Pre-calculate some shiz
    float fNyquist = sampleRate / 2;
    float cycle_length_s = sampleRate / frequency;
    
    // Allocate memory for the wavetable
    float* wave = malloc((unsigned)floor(cycle_length_s) * sizeof(float));
    
    // Generate each sample
    for (unsigned phase_tick = 0; phase_tick < cycle_length_s; ++phase_tick)
    {
        float sample = 0;
        float T = (phase_tick / sampleRate);    // Time in seconds
        unsigned k = 0;                         // Harmonic counter
        
        // Calculate the sample from the fourier series up to nyquist
        while ((k * frequency) < fNyquist)
        {
            sample += (powf(-1.0, (k)) * sin(2 * M_PI * frequency * (2 * k + 1)* T) / (powf((2 * k + 1), 2)));
            k++;
        }
        
        // Scale the sample and store it in the table
        wave[phase_tick] = (sample * (8/(M_PI * M_PI)));
    }
    length = (unsigned)(floor(cycle_length_s));
    return wave;
}

