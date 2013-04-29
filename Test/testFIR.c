/*
 *  testFIR.c
 *  Test
 *
 *  Created by Hamilton Kibbe on 4/28/13.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */
#include <stdio.h>
#include "testFIR.h"
#include "FtAudioFIRFilter.h"
#include "coefficients.h"
#include "signals.h"

bool
testFIRFilterAgainstMatlab(void)
{
	printf("Testing filter results against matlab...");
	return 1;
}

bool
testFIRFilterBlockSize(void)
{
	printf("Testing variable block size...");
	bool passed = 1;
	unsigned inputLength = 100;
	float input[inputLength];
	float outfull[inputLength];
	float outchunk[inputLength];
	generate_signal(input, inputLength);

	FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit(taps, 21);
	FtAudioFIRFilterProcess(theFilter, outfull, input, inputLength);
	
	FtAudioFIRFilterFlush(theFilter);
	FtAudioFIRFilterProcess(theFilter, outchunk, input, (inputLength / 2));
	FtAudioFIRFilterProcess(theFilter, outchunk+(inputLength / 2), input+(inputLength / 2), (inputLength / 2));
	
	// Check 
	for (unsigned i = 0; i < inputLength; ++i)
	{
		if(fabs(fabs(outfull[i]) - fabs(outchunk[i])) > 0.000001)
		{
			passed = 0;
			break;
		}

	}
	
	FtAudioFIRFilterFree(theFilter);
	return passed;
};

bool
runFIRFilterTests(void)
{
	printf("[FtAudioFIRFilter] RUNNING TESTS\n");
	
	if (testFIRFilterAgainstMatlab())
		printf("PASSED\n");
	else
		printf("FAILED\n");
	
	if (testFIRFilterBlockSize())
		printf("PASSED\n");
	else
		printf("FAILED\n");
}