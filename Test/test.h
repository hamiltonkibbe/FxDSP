/*
 *  test.h
 *  Test functions
 *
 *  Created by Hamilton Kibbe on 4/28/13.
 *  Copyright 2013 HTK Audio. All rights reserved.
 *
 */

#ifndef TEST_H
#define TEST_H
#include "signals.h"

/* test function pointer */
typedef unsigned(*testFunction)();

unsigned
CompareFloat(float val1, float val2, float precision);


unsigned
CompareFloatBuffers(const float *buf1, const float *buf2, unsigned len, float precision);



#endif //TEST_H
