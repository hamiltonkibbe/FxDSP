//
//  Signals.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 7/9/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "Signals.h"
#include <math.h>


void
sinewave(float* dest, unsigned n, float f, float phase, float amp, float Fs)
{
    for (unsigned i = 0; i < n; ++i)
    {
        *dest++ = amp * sinf((2 * M_PI * i * f / Fs) + phase);
    }
}

void
sinewaveD(double* dest, unsigned n, double f, double phase, double amp, double Fs)
{
    for (unsigned i = 0; i < n; ++i)
    {
        *dest++ = amp * sin((2 * M_PI * i * f / Fs) + phase);
    }
}
