//
//  testFFT.h
//  Test
//
//  Created by Hamilton Kibbe on 5/28/13.
//
//

#ifndef TESTFFT_H
#define TESTFFT_H


#include "test.h"
#define NUMBER_OF_FFT_TESTS (2)

unsigned
runFFTTests(void);

unsigned
testForwardFFTAgainstMatlab();

unsigned
testForwardToInverseFFT();

static const testFunction fftTests[NUMBER_OF_FFT_TESTS] =
{
    &testForwardFFTAgainstMatlab,
    &testForwardToInverseFFT
};

// >> y = single(abs(fft(linspace(1,64,64))));
// >> y(1:32)'
static const float matlabMagnitude[32] =
{
    2.0800000e+03,
    6.5216052e+02,
    3.2647351e+02,
    2.1808694e+02,
    1.6402660e+02,
    1.3169798e+02,
    1.1023662e+02,
    9.4986534e+01,
    8.3620033e+01,
    7.4844154e+01,
    6.7883369e+01,
    6.2244366e+01,
    5.7598476e+01,
    5.3718376e+01,
    5.0441895e+01,
    4.7650322e+01,
    4.5254833e+01,
    4.3187733e+01,
    4.1396595e+01,
    3.9840263e+01,
    3.8486073e+01,
    3.7307838e+01,
    3.6284420e+01,
    3.5398651e+01,
    3.4636551e+01,
    3.3986725e+01,
    3.3439911e+01,
    3.2988628e+01,
    3.2626919e+01,
    3.2350140e+01,
    3.2154835e+01,
    3.2038593e+01
};

// >> y = single(angle(fft(linspace(1,64,64))));
// >> y(1:32)'
static const float matlabPhase[32] =
{
    0,
    1.6198837,
    1.6689711,
    1.7180585,
    1.7671459,
    1.8162333,
    1.8653207,
    1.9144080,
    1.9634954,
    2.0125828,
    2.0616701,
    2.1107576,
    2.1598449,
    2.2089324,
    2.2580197,
    2.3071072,
    2.3561945,
    2.4052818,
    2.4543693,
    2.5034566,
    2.5525441,
    2.6016314,
    2.6507187,
    2.6998062,
    2.7488935,
    2.7979810,
    2.8470683,
    2.8961558,
    2.9452431,
    2.9943304,
    3.0434179,
    3.0925052
};
#endif