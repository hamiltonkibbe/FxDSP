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


unsigned
CompareFloat(float val1, float val2, precision);


unsigned
CompareFloatBuffers(float *buf1, float *buf2, unsigned len, float precision);



#endif TEST_H