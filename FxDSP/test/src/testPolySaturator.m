//
//  testPolySaturator.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 3/19/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//


#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>

#include "PolySaturator.h"
#include "testPolySat.h"

@interface testPolySaturator : XCTestCase

@end

@implementation testPolySaturator


-(void) testPolySaturator
{
    float output[TEST_VECTOR_LENGTH];
    
    for (unsigned n = 0; n < 4; ++n)
    {
        printf("N: %d", n);
        PolySaturator* sat = PolySaturatorInit(n_vector[n]);
        
        PolySaturatorProcess(sat, output, test_vector, TEST_VECTOR_LENGTH);
        PolySaturatorFree(sat);
        
        for (unsigned i = 0; i < TEST_VECTOR_LENGTH; ++i)
        {
            XCTAssertEqualWithAccuracy(output[i], results[n][i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__, i, output[i], results[n][i]);
        }
    }
    
}

@end
