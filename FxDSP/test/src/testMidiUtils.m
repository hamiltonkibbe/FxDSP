//
//  testMidiUtils.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/22/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "MidiUtils.h"
@interface testMidiUtils : XCTestCase

@end

@implementation testMidiUtils

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testNoteToFrequency
{
    unsigned note[3] = {0, 69,  127};
    float freq[3] = {8.1757989156, 440.0, 12543.8539514160};
    for (unsigned i = 0; i < 3; ++i)
    {
        float out = midiNoteToFrequency(note[i]);
        XCTAssertEqualWithAccuracy(out, freq[i], 0.01, @"Failed. Expected: %f Got: %f\n",out, freq[i]);
    }
}

- (void)testFrequencyToNote
{
    unsigned note[3] = {0, 69,  127};
    float freq[3] = {8.1757989156, 440.0, 12543.8539514160};
    for (unsigned i = 0; i < 3; ++i)
    {
        unsigned out = frequencyToMidiNote(freq[i]);
        XCTAssertEqualWithAccuracy(out, note[i], 0.01, @"Failed. Expected: %d Got: %d\n",note[i], out);
    }
}


@end
