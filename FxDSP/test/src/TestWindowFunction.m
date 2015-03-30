//
//  FxDSPWindowFunction_Tests.m
//  FxDSP
//
//  Created by Hamilton Kibbe on 10/28/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "WindowFunction.h"
#include "testWindow.h"

@interface FxDSPWindowFunction_Tests : XCTestCase

@end

@implementation FxDSPWindowFunction_Tests

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


- (void) testBoxcar
{
    float window[TEST_WINDOW_LENGTH];
    boxcar(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabBoxcar[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabBoxcar[i]);
    }
}


- (void) testHann
{
    float window[TEST_WINDOW_LENGTH];
    hann(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabHann[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabHann[i]);
    }
}


- (void)testHamming
{
    float window[TEST_WINDOW_LENGTH];
    hamming(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabHamming[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabHamming[i]);
    }
}

- (void)testBlackman
{
    float window[TEST_WINDOW_LENGTH];
    blackman(TEST_WINDOW_LENGTH, 0.16, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabBlackman[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabBlackman[i]);
    }
}


- (void) testTukey
{
    float window[TEST_WINDOW_LENGTH];
    tukey(TEST_WINDOW_LENGTH, 0.5, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabTukey[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabTukey[i]);
    }
}


- (void)testBartlett
{
    float window[TEST_WINDOW_LENGTH];
    bartlett(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabBartlett[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabBartlett[i]);
    }
}


- (void)testGaussian
{
    float window[TEST_WINDOW_LENGTH];
    gaussian(TEST_WINDOW_LENGTH, 0.4, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabGaussian[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabGaussian[i]);
    }
}


- (void)testBartlettHann
{
    float window[TEST_WINDOW_LENGTH];
    bartlett_hann(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabBartlettHann[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabBartlettHann[i]);
    }
}


- (void)testKaiser
{
    float window[TEST_WINDOW_LENGTH];
    kaiser(TEST_WINDOW_LENGTH, 0.5, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabKaiser[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabKaiser[i]);
    }
}


- (void)testNuttall
{
    float window[TEST_WINDOW_LENGTH];
    nuttall(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabNuttall[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabNuttall[i]);
    }
}


- (void)testBlackmanHarris
{
    float window[TEST_WINDOW_LENGTH];
    blackman_harris(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabBlackmanHarris[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabBlackmanHarris[i]);
    }
}


- (void)testFlatTop
{
    float window[TEST_WINDOW_LENGTH];
    flat_top(TEST_WINDOW_LENGTH, window);
    for (unsigned i = 0; i < TEST_WINDOW_LENGTH; ++i)
    {
        XCTAssertEqualWithAccuracy(window[i], matlabFlatTop[i], 0.000001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, window[i], matlabFlatTop[i]);
    }
}

@end
