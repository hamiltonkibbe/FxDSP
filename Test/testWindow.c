//
//  testWindow.c
//  Test
//
//  Created by Hamilton Kibbe on 5/14/13.
//
//

#include <stdio.h>
#include "test.h"
#include "testWindow.h"
#include "FtAudioWindowFunction.h"

unsigned
runWindowFunctionTests()
{
    unsigned passed = 1;
	printf("\n[FtAudioWindowFunction] RUNNING TESTS\n");
	
    for (unsigned i = 0; i < NUMBER_OF_WINDOW_TESTS; ++i)
    {
        if (tests[i]())
            printf("PASSED\n");
        else
        {
            printf("FAILED\n");
            passed = 0;
        }
    }
	return passed;

}

unsigned
testBoxcar()
{
    printf("Testing Boxcar window...");
    float window[10];
    boxcar(10, window);
    return CompareFloatBuffers(window, matlabBoxcar, TEST_WINDOW_LENGTH, 0.000001);
}


unsigned
testHann()
{
    printf("Testing Hann window...");
    float window[10];
    hann(10, window);
    return CompareFloatBuffers(window, matlabHann, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testHamming()
{
    printf("Testing Hamming window...");
    float window[10];
    hamming(10, window);
    return CompareFloatBuffers(window, matlabHamming, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testBlackman()
{
    printf("Testing Blackman window...");
    float window[10];
    blackman(10, 0.16, window);
    return CompareFloatBuffers(window, matlabBlackman, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testTukey()
{
    printf("Testing Tukey window...");
    float window[10];
    tukey(10, 0.5, window);
    return CompareFloatBuffers(window, matlabTukey, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testBartlett()
{
    printf("Testing Bartlett window...");
    float window[10];
    bartlett(10, window);
    return CompareFloatBuffers(window, matlabBartlett, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testGaussian()
{
    printf("Testing Gaussian window...");
    float window[10];
    gaussian(10, 0.4, window);
    return CompareFloatBuffers(window, matlabGaussian, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testBartlettHann()
{
    printf("Testing Bartlett-Hann window...");
    float window[10];
    bartlett_hann(10, window);
    return CompareFloatBuffers(window, matlabBartlettHann, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testKaiser()
{
    printf("Testing Kaiser window...");
    float window[10];
    kaiser(10, 0.5, window);
    return CompareFloatBuffers(window, matlabKaiser, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testNuttall()
{
    printf("Testing Nuttall window...");
    float window[10];
    nuttall(10, window);
    return CompareFloatBuffers(window, matlabNuttall, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testBlackmanHarris()
{
    printf("Testing Blackman-Harris window...");
    float window[10];
    blackman_harris(10, window);
    return CompareFloatBuffers(window, matlabBlackmanHarris, TEST_WINDOW_LENGTH, 0.000001);
}

unsigned
testFlatTop()
{
    printf("Testing flat top window...");
    float window[10];
    flat_top(10, window);
    return CompareFloatBuffers(window, matlabFlatTop, TEST_WINDOW_LENGTH, 0.000001);
}