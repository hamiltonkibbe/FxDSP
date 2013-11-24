/*
 *  FtAudioMidiUtils.h
 *  
 *
 *  Created by Hamilton Kibbe on 5/10/13.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */
 

#ifndef FTAUDIOMIDIUTILS_H
#define FTAUDIOMIDIUTILS_H

//includes
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif

static inline float
midiNoteToFrequency(unsigned note)
{
    return powf(2.0, ((note - 69.0)/12.)) * 440.0;
}

static inline unsigned
frequencyToMidiNote(float f)
{
    return (unsigned)(69 + (12 * log2f(f / 440.0)));
}

#ifdef __cplusplus
}
#endif

#endif //FTAUDIOMIDIUTILS_H