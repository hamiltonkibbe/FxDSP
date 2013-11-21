//
//  testBiquad.c
//  Test
//
//  Created by Hamilton Kibbe on 5/14/13.
//
//
#include "test.h"
#include "testBiquad.h"
#include "FtAudioDsp.h"
#include "FtAudioBiquadFilter.h"
#include <stdio.h>

unsigned
testBiquadAgainstMatlab()
{
    printf("Testing filter results against matlab...");
    
    // Set up
    float output[10];
    FtAudioFillBuffer(output, 10, 0.0);
    FtAudioBiquadFilter *filter = FtAudioBiquadFilterInit(b, a);
    
    // Process
    FtAudioBiquadFilterProcess(filter, output, input, 10);
    
    // Clean up
    FtAudioBiquadFilterFree(filter);
    
    // Return
    return CompareFloatBuffers(output, MatlabOutput, 10, 0.00001);
}

unsigned
runBiquadFilterTests()
{
	unsigned passed = 1;
	printf("\n[FtAudioBiquadFilter] RUNNING TESTS\n");
	
	if (testBiquadAgainstMatlab())
		printf("PASSED\n");
	else
	{
		printf("FAILED\n");
		passed = 0;
	}
    return 0;
}