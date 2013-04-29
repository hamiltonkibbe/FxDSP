/*
 *  signals.c
 *  Test
 *
 *  Created by Hamilton Kibbe on 4/28/13.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */

#include "signals.h"
#include <stdio.h>
#include <math.h>

void
generate_signal(float* buffer, int length)
{
	for(unsigned i = 0; i < length; ++i)
	{
		float sample = (float)sin((6*M_PI/length) * i);
		buffer[i] = sample;
	}
}