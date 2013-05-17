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

inline float
midiNoteToFrequency(unsigned note)
{
    return powf(2, ((note-69)/12)) x 440.0;
}

inline unsigned
frequencyToMidiNote(float f)
{
    return 69 + (12 * log2(f/440.));
}

#ifdef __cplusplus
}
#endif

#endif //FTAUDIOMIDIUTILS_H