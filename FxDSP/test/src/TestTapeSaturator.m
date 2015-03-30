//
//  TestTapeSaturator.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 3/16/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#import "Tape.h"
#import "signals.h"
#import <math.h>
#import <Accelerate/Accelerate.h>
#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>

@interface TestTapeSaturator : XCTestCase

@end

@implementation TestTapeSaturator


- (void) testTapeProperties
{
    Tape* tape = TapeInit(TS_3_75IPS, 0.0, 0, 0, 44100);
    XCTAssertEqual(TapeGetHysteresis(tape), 0.0);
    XCTAssertEqual(TapeGetSaturation(tape), 0.0);
    
    TapeSetHysteresis(tape, 0.5);
    XCTAssertEqual(TapeGetHysteresis(tape), 0.5);
    
    TapeSetSaturation(tape, 0.5);
    XCTAssertEqual(TapeGetSaturation(tape), 0.5);
}

- (void) testTapeSaturation
{
    
    float result[] = {
        0,         0.2019320, 0.3967104, 0.5774218, 0.7373007, 0.8677393,
        0.9557083, 0.9939346, 0.9999731, 0.9999731, 0.9963614, 0.9528959,
        0.8539907, 0.7135545, 0.5460452, 0.3599012, 0.1619539, -0.0676664,
        -0.2680014, -0.4588417, -0.6333798, -0.7846094, -0.9024743, -0.9737067,
        -0.9979685, -1.0000000, -1.0000000, -0.9880634, -0.9257124, -0.8109745,
        -0.6602830, -0.4857824, -0.2949427, -0.0946077, 0.1350657, 0.3330131,
        0.5191602, 0.6867718, 0.8282618, 0.9319279, 0.9862090, 0.9995818,
        0.9999731, 0.9997172, 0.9737074, 0.8924777, 0.7639291, 0.6042982,
        0.4235985, 0.2288202, 0.0268882, -0.2019320, -0.3967104, -0.5774218,
        -0.7373007, -0.8677393, -0.9557083, -0.9939346, -0.9999731, -0.9999731,
        -0.9963614,-0.9528959,-0.8539907,-0.7135545,-0.5460452,-0.3599012,
        -0.1619539,0.0676664,0.2680014,0.4588417,0.6333798,0.7846094, 0.9024743,
        0.9737067,
        0.9979685,
        1.0000000,
        1.0000000,
        0.9880634,
        0.9257124,
        0.8109745,
        0.6602830,
        0.4857824,
        0.2949427,
        0.0946077,
        -0.1350657,
        -0.3330131,
        -0.5191602,
        -0.6867718,
        -0.8282618,
        -0.9319279,
        -0.9862090,
        -0.9995818,
        -0.9999731,
        -0.9997172,
        -0.9737074,
        -0.8924777,
        -0.7639291,
        -0.6042982,
        -0.4235985,
        -0.2288202};
    
    Tape* tape = TapeInit(TS_3_75IPS, 0.5, 0.5, 0, 44100);
    for (unsigned i = 0; i < 100; ++i)
    {
        XCTAssertEqualWithAccuracy(TapeTick(tape, MatlabSignal[i]), result[i], 0.0000001);
    }
}




@end
