//
//  TestOptocoupler.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/3/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#include "Optocoupler.h"
#include <math.h>
#include <gtest/gtest.h>



TEST(OptocouplerSingle, Smoketest)
{
    const Opto_t types[2] = {OPTO_LDR, OPTO_PHOTOTRANSISTOR};
    float sinewave[10000];
    float out[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sinf((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    for (unsigned i = 0; i < sizeof(types); ++i)
    {
        Opto* opto = OptoInit(types[i], 0.5, 44100);
        ASSERT_NE((void*)opto, (void*)NULL);

        OptoProcess(opto, out, sinewave, 10000);
        OptoFree(opto);
    }
}



TEST(OptocouplerDouble, Smoketest)
{
    const Opto_t types[2] = {OPTO_LDR, OPTO_PHOTOTRANSISTOR};
    double sinewave[10000];
    double out[10000];
    for (unsigned i = 0; i < 10000; ++i)
    {
        sinewave[i] = sin((6000 * M_PI * i)/10000); // 3000Hz sinewave at 44100
    }
    for (unsigned i = 0; i < sizeof(types); ++i)
    {
        OptoD* opto = OptoInitD(types[i], 0.5, 44100);
        ASSERT_NE((void*)opto, (void*)NULL);

        OptoProcessD(opto, out, sinewave, 10000);
        OptoFreeD(opto);
    }
}