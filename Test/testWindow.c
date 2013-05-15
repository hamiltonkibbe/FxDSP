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
	
	if (testBlackman())
		printf("PASSED\n");
	else
	{
		printf("FAILED\n");
		passed = 0;
	}
	
	if (testKaiser())
		printf("PASSED\n");
	else
	{
		printf("FAILED\n");
		passed = 0;
	}
	return passed;

}

unsigned
testBlackman()
{
    printf("Testing Blackman window...");
    float window[10];
    blackman(10, 0.16, window);
    return CompareFloatBuffers(window, matlabBlackman, 10, 0.000001);
}

unsigned
testKaiser()
{
    printf("Testing Kaiser window...");
    float window[10];
    kaiser(10, 0.5, window);
    return CompareFloatBuffers(window, matlabKaiser, 10, 0.000001);
}