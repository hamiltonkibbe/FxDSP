//
//  FtAudioWavetableGenerator.c
//  Test
//
//  Created by Hamilton Kibbe on 5/17/13.
//
//

#include <stdio.h>
#include <math.h>
#include "FtAudioWavetableGenerator.h"
#include "FtAudioMidiUtils.h"

float fs = 48000;
float nyquist = 24000;


void
generateSawTable(float **dest)
{
    printf("Generating wavetable...\n");
    for (unsigned i = 0; i < 127; ++i)
    {

        float frequency = midiNoteToFrequency(i);
        float cycle_len = fs / frequency;
        float wave[50000];
        for (unsigned j = 0; j < cycle_len; ++j)
        {
            float sample = 0;
            float t = (j/fs);
            unsigned harmonic = 1;
            while(harmonic * frequency < nyquist)
            {
                float signterm = pow((-1.), (harmonic - 1));
                sample += signterm * (sin(2 * M_PI * frequency * harmonic * t) / harmonic);
                harmonic++;
            }
            wave[j] = sample;
        }
        dest[i] = wave;
    }
    
}