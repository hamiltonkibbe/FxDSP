/*
 *  MidiUtils.h
 *
 *
 *  Created by Hamilton Kibbe on 5/10/13.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */


#ifndef MIDIUTILS_H
#define MIDIUTILS_H


#ifdef __cplusplus
extern "C" {
#endif

float
midiNoteToFrequency(unsigned note);


unsigned
frequencyToMidiNote(float f);


#ifdef __cplusplus
}
#endif

#endif //MIDIUTILS_H