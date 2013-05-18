//
//  FtAudioWavetable.h
//  Test
//
//  Created by Hamilton Kibbe on 5/17/13.
//
//

#ifndef Test_FtAudioWavetable_h
#define Test_FtAudioWavetable_h

#include "FtAudioError.h"

#define NUMBER_OF_TABLES (128)

/** Opaque FtAudioBiquadFilter structure */
typedef struct FtAudioWavetable FtAudioWavetable;

/** wavetable Generator function pointer */
typedef float* (*WavetableGenerator)(float,float,unsigned *);


typedef enum _WaveType_t
{
    SQUARE = 0,
    SAWTOOTH = 1,
    TRIANGLE = 2,
    OTHER = 3,
    N_WAVE_TYPES
    
}WaveType_t;

FtAudioWavetable *
FtAudioWavetableInit(WaveType_t type, float sampleRate);


FtAudioWavetable *
FtAudioCustomWavetableInit(WavetableGenerator generator, const char* waveName, float sampleRate);


FtAudioError_t
FtAudioWavetableFree(FtAudioWavetable *table);



float *
generateSawTable(float frequency, float sampleRate, unsigned *length);

float *
generateSquareTable(float frequency, float sampleRate, unsigned *length);

float *
generateTriangleTable(float frequency, float sampleRate, unsigned *length);

#endif
