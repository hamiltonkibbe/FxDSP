//
//  testBiquad.h
//  Test
//
//  Created by Hamilton Kibbe on 5/14/13.
//
//

#ifndef TESTBIQUAD_H
#define TESTBIQUAD_H

unsigned
testBiquadAgainstMatlab();


unsigned
runBiquadFilterTests();

static const float b[3] = {0.1, 0.2, 0.3};
static const float a[2] = {0.1, 0.2};

static const float input[10] =
{
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0
};

static const float MatlabOutput[10] =
{
    0.100000,
    0.290000,
    0.551000,
    0.486900,
    0.441110,
    0.458509,
    0.465927,
    0.461706,
    0.460644,
    0.461594

};

#endif
