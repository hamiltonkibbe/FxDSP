/*
 *  testFIR.c
 *  Test
 *
 *  Created by Hamilton Kibbe on 4/28/13.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */
#include <stdio.h>
#include <math.h>
#include "testFIR.h"
#include "test.h"
#include "FtAudioFIRFilter.h"
#include "signals.h"

unsigned
testFIRFilterAgainstMatlab(void)
{
    printf("Testing filter results against matlab...");
    float output[100] = {};
	
	// Set up
    FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit(MatlabFilter, 21);
	
	// Process
    FtAudioFIRFilterProcess(theFilter, output, MatlabSignal, 100);
	
	// Tear down
    FtAudioFIRFilterFree(theFilter);

	// Check results
    return CompareFloatBuffers(output, MatlabLowpassOutput, 100, 0.000001);
}

unsigned
testFIRFilterBlockSize(void)
{
    printf("Testing variable block size...");
    unsigned inputLength = 100;
    float input[inputLength];
    float outfull[inputLength];
    float outchunk[inputLength];
    generate_signal(input, inputLength);
    
	// Set up
	FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit(taps, 21);
	
	// Process in one block
    FtAudioFIRFilterProcess(theFilter, outfull, input, inputLength);
    
	// Process in two blocks
    FtAudioFIRFilterFlush(theFilter);
    FtAudioFIRFilterProcess(theFilter, outchunk, input, (inputLength / 2));
    FtAudioFIRFilterProcess(theFilter, outchunk+(inputLength / 2), input+(inputLength / 2), (inputLength / 2));
    
	// Tear down
	FtAudioFIRFilterFree(theFilter);

	// Check results
    return CompareFloatBuffers(outfull, outchunk, inputLength, 0.000001);
};


unsigned
runFIRFilterTests(void)
{
	unsigned passed = 1;
	printf("[FtAudioFIRFilter] RUNNING TESTS\n");
	
	if (testFIRFilterAgainstMatlab())
		printf("PASSED\n");
	else
	{
		printf("FAILED\n");
		passed = 0;
	}
	
	if (testFIRFilterBlockSize())
		printf("PASSED\n");
	else
	{
		printf("FAILED\n");
		passed = 0;
	}
	return passed;
}


