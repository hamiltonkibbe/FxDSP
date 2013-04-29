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

unsigned
test_fir(void)
{
	
	bool passed = 1;
	unsigned inputLength = 100;
	float input[inputLength];
	float outputfull[inputLength];
	float outputchunk[inputLength];
	generate_signal(input, inputLength);

	FtAudioFIRFilter *theFilter = FtAudioFIRFilterInit((const float*)taps, 21);
	FtAudioFIRFilterProcess(theFilter, outputfull, input, inputLength);
	
	FtAudioFIRFilterFlush(theFilter);
	FtAudioFIRFilterProcess(theFilter, outputchunk, input, (inputLength / 2));
	FtAudioFIRFilterProcess(theFilter, outputchunk+(inputLength / 2), input+(inputLength / 2), (inputLength / 2));
	
	// Check 
	for (unsigned i = 0; i < inputLength; ++i)
	{
		if(fabs(fabs(outputfull[i]) - fabs(outputchunk[i])) > 0.000001)
		{
			passed = 0;
			break;
		}

	}
	
	FtAudioFIRFilterFree(theFilter);
	return passed;
};

