//
//  TestRBJFilter.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/20/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "FtAudioRBJFilter.h"
#include "FtAudioDsp.h"

@interface TestRBJFilter : XCTestCase

@end

@implementation TestRBJFilter

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

- (void)testRBJFilter
{
    
    FtAudioRBJFilter *filter = FtAudioRBJFilterInit(RBJ_LPF, 45, 100);
    float output[10];
    float input[10] = {0,0,0,0,0,0,0,0,0,0};
    FtAudioFillBuffer(output, 10, 0.0);
    
    FtAudioRBJFilterProcess(filter, output, input, 10);
    for (unsigned i = 0; i < 10; ++i)
    {
        XCTAssertEqualWithAccuracy(output[i], 0, 0.01, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, output[i], 0.0);
    }
}

@end
