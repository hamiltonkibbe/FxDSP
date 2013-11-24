//
//  FtAudioWavetable.h
//  Test
//
//  Created by Hamilton Kibbe on 5/17/13.
//
//

#ifndef Test_FTA_Wavetable_h
#define Test_FTA_Wavetable_h

#include "FtAudioError.h"

#define NUMBER_OF_TABLES (128)


#ifdef __cplusplus
extern "C" {
#endif


/** Opaque FTA_BiquadFilter structure */
typedef struct FTA_Wavetable FTA_Wavetable;

/** wavetable Generator function pointer */
typedef float* (*WavetableGenerator)(float,float,unsigned );


typedef enum _WaveType_t
{
    SQUARE = 0,
    SAWTOOTH = 1,
    TRIANGLE = 2,
    OTHER = 3,
    N_WAVE_TYPES
    
}WaveType_t;

FTA_Wavetable *
FTA_WavetableInit(WaveType_t type, float sampleRate);


FTA_Wavetable *
FTA_CustomWavetableInit(WavetableGenerator generator, const char* waveName, float sampleRate);


FTA_Error_t
FTA_WavetableFree(FTA_Wavetable *table);



float *
generateSawTable(float frequency, float sampleRate, unsigned length);

float *
generateSquareTable(float frequency, float sampleRate, unsigned length);

float *
generateTriangleTable(float frequency, float sampleRate, unsigned length);

    
#ifdef __cplusplus
}
#endif
        
#endif
