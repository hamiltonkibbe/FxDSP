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
#include "coefficients.h"
#include "signals.h"

unsigned
testFIRFilterAgainstMatlab(void)
{
    printf("Testing filter results against matlab...");
    float output[100] = {};
    FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit(MatlabFilter, 21);
    FtAudioFIRFilterProcess(theFilter, output, MatlabSignal, 100);
    FtAudioFilterFree(theFilter);
    /*
    unsigned passed = 1;
    for (unsigned i = 0; i < 100; ++i)
    {
        if(fabs(fabs(output[i]) - fabs(MatlabLowpassOutput[i])) > 0.000001)
        {
            passed = 0;
            break;
        }
            
    }
    return passed;
    */
    return CompareFloatBuffers(output, MatlabLowpassOutput, 0.000001);
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

    FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit(taps, 21);
    FtAudioFIRFilterProcess(theFilter, outfull, input, inputLength);
    
    FtAudioFIRFilterFlush(theFilter);
    FtAudioFIRFilterProcess(theFilter, outchunk, input, (inputLength / 2));
    FtAudioFIRFilterProcess(theFilter, outchunk+(inputLength / 2), input+(inputLength / 2), (inputLength / 2));
    FtAudioFIRFilterFree(theFilter);
    /*
    // Check 
    unsigned passed = 1;
    for (unsigned i = 0; i < inputLength; ++i)
    {
        if(fabs(fabs(outfull[i]) - fabs(outchunk[i])) > 0.000001)
        {
            passed = 0;
            break;
        }
    }
    return passed;
    */
    return CompareFloatBuffers(outfull, outchunk, 0.000001);
};


unsigned
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