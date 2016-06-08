//
//  MidiUtils.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/3/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "MidiUtils.h"
#include "Utilities.h"

#include <math.h>


float
midiNoteToFrequency(unsigned note)
{
    return powf(2.0, ((note - 69.0)/12.)) * 440.0;
}

unsigned
frequencyToMidiNote(float f)
{
    return (unsigned)(69 + (12 * log2f(f / 440.0)));
}