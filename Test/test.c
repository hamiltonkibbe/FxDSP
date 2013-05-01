/*
 *  test.c
 *  Test
 *
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */
 
 #include "test.h"
 #include <math.h>
 #include <stdio.h>
 
unsigned
CompareFloat(float val1, float val2, float precision)
{
    return (fabs(fabs(val1) - fabs(val2)) <= precision)? 1 : 0;
}


unsigned
CompareFloatBuffers(const float *buf1, const float *buf2, unsigned len, float precision)
{
    unsigned equal = 1;
    for (unsigned i = 0; i <len; ++i)
    {
        if(!CompareFloat(buf1[i], buf2[i], precision))
        {
            equal = 0;
			printf("Buffer Compare Failed at index %d: %1.20f   %1.20f\n", i, buf1[i], buf2[i]);
        }
    }
    return equal;
}