//
//  TestMidiUtils.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/8/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "MidiUtils.h"
#include <gtest/gtest.h>

TEST(MidiUtils, NoteToFrequencyConversion)
{
    unsigned note[3] = {0, 69,  127};
    float freq[3] = {8.1757989156, 440.0, 12543.8539514160};
    for (unsigned i = 0; i < 3; ++i)
    {
        float out = midiNoteToFrequency(note[i]);
        ASSERT_FLOAT_EQ(freq[i], out);
    }
}


TEST(MidiUtils, FrequencyToNoteConversion)
{
    unsigned note[3] = {0, 69,  127};
    float freq[3] = {8.1757989156, 440.0, 12543.8539514160};
    for (unsigned i = 0; i < 3; ++i)
    {
        unsigned out = frequencyToMidiNote(freq[i]);
        ASSERT_FLOAT_EQ(note[i], out);
    }
}




