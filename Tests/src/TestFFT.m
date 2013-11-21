//
//  TestFFT.m
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 10/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "testFFT.h"
#include "FtAudioFFT.h"
#include "FtAudioDsp.h"

@interface FutureTechAudioFFT_Tests : XCTestCase

@end

@implementation FutureTechAudioFFT_Tests

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

- (void)testForwardFFTAgainstMatlab
{
    float signal[64];
    float magnitude[32];
    float phase[32];
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = (float)i + 1.0;
    }
    
    FtAudioFFTConfig* fft = FtAudioFFTInit(64);
    FtAudioFFTForward(fft, signal, magnitude, phase);
    FtAudioFFTFree(fft);
    for (unsigned i = 0; i < 32; ++i)
    {
        XCTAssertEqualWithAccuracy(magnitude[i], matlabMagnitude[i], 0.001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, magnitude[i], matlabMagnitude[i]);
        XCTAssertEqualWithAccuracy(phase[i], matlabPhase[i], 0.001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, phase[i], matlabPhase[i]);
    }
}

- (void)testForwardToInverseFFT
{
    float signal[64];
    float magnitude[32];
    float phase[32];
    float output[64];
    
    for (unsigned i = 0; i < 64; ++i)
    {
        signal[i] = sinf(((4*M_PI)/64) * i) + 0.5 * sinf(((8*M_PI)/64) * i);
    }
    
    FtAudioFFTConfig* fft = FtAudioFFTInit(64);
    FtAudioFFTForward(fft, signal, magnitude, phase);
    FtAudioFFTInverse(fft, magnitude, phase, output);
    FtAudioFFTFree(fft);
    for (unsigned i = 0; i < 64; ++i)
    {
        XCTAssertEqualWithAccuracy(signal[i], output[i], 0.00001, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, signal[i], output[i]);
    }
}

- (void) testFFTConvolution
{
    float dest[15];
    float in[3] = {1.0, 2.0, 3.0};
    float in2[4] = {4.0,5.0,6.0, 7};
    FtAudioFFTConfig* fft = FtAudioFFTInit(16);
    FtAudioFFTConvolve(fft, in, 3, in2, 4, dest);
    for (unsigned i = 0; i < 15; ++i)
    {
        XCTAssertEqualWithAccuracy(dest[i], matlabConvolution[i], 0.00025, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, dest[i], matlabConvolution[i]);
    }
}

- (void)testFFTFilterConvolution
{

    float in[3] = {1.0, 2.0, 3.0};
    float in2[4] = {4.0,5.0,6.0,7};
    FtAudioFFTConfig* fft = FtAudioFFTInit(16);
    float padded[16];
    float dest[16];
    FtAudioFillBuffer(padded, 16, 0.0);
    FtAudioCopyBuffer(padded, in2, 4);
    
    DSPSplitComplex splitcomplex;
    splitcomplex.realp = (float*) malloc(16 * sizeof(float));
    splitcomplex.imagp = splitcomplex.realp + 8;
    
    //FtAudioFillBuffer(splitcomplex.realp, 16, 0.0);
    FtAudioFFTForwardSplit(fft, (DSPComplex*)padded, &splitcomplex);
    FtAudioFFTFilterConvolve(fft, in, 3, splitcomplex, dest);
    for (unsigned i = 0; i < 15; ++i)
    {
        XCTAssertEqualWithAccuracy(dest[i], matlabConvolution[i], 0.00025, @"[%s FAILED] Buffer Compare failed at index %d. Got: %0.10f  Expected: %0.10f\n", __PRETTY_FUNCTION__,i, dest[i], matlabConvolution[i]);
    }
}

@end
