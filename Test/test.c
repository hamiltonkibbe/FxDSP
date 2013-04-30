/*
 *  test.c
 *  Test
 *
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */
 
 #include "test.h"
 #include <math.h>
 
 
unsigned
CompareFloat(float val1, float val2, precision)
{
    return (fabs(fabs(val1) - fabs(val2)) <= precision)? 1 : 0;
}


unsigned
CompareFloatBuffers(float *buf1, float *buf2, unsigned len, float precision)
{
    unsigned equal = 1;
    for (unsigned i = 0; i <len; ++i)
    {
        if(!CompareFloat(buf1[i], buf2[i], precision))
        {
            equal = 0;
            break;
        }
    }
    return equal;
}