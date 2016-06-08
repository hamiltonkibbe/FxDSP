//
//  TestStereo.cpp
//  FxDSP
//
//  Created by Hamilton Kibbe on 6/8/16.
//  Copyright © 2016 Hamilton Kibbe. All rights reserved.
//

#include "Stereo.h"
#include <math.h>
#include <gtest/gtest.h>

TEST(StereoSingle, StereoToMono)
{
  float left[100];
  float right[100];
  float out[100];

  for (unsigned i = 0; i < 100; ++i)
  {
    left[i] = sinf(4.0 * M_PI * i / 100.0);
    right[i] = -1.0 * sinf(4.0 * M_PI * i / 100.0);
    out[i] = 999.0;
  }

  StereoToMono(out, left, right, 100);

  for (unsigned i = 0; i < 100; ++i)
  {
    ASSERT_FLOAT_EQ(0.0, out[i]);
  }
}

TEST(StereoSingle, MonoToStereo)
{
  float mono[100];
  float left[100];
  float right[100];


  for (unsigned i = 0; i < 100; ++i)
  {
    mono[i] = sinf(4.0 * M_PI * i / 100.0);
    left[i] = 999.;
    right[i] = -999.;
  }

  MonoToStereo(left, right, mono, 100);

  for (unsigned i = 0; i < 100; ++i)
  {
    ASSERT_FLOAT_EQ(left[i], right[i]);
  }
}


TEST(StereoDouble, StereoToMono)
{
  double left[100];
  double right[100];
  double out[100];

  for (unsigned i = 0; i < 100; ++i)
  {
    left[i] = sin(4.0 * M_PI * i / 100.0);
    right[i] = -1.0 * sin(4.0 * M_PI * i / 100.0);
    out[i] = 999.0;
  }

  StereoToMonoD(out, left, right, 100);

  for (unsigned i = 0; i < 100; ++i)
  {
    ASSERT_DOUBLE_EQ(0.0, out[i]);
  }
}

TEST(StereoDouble, MonoToStereo)
{
  double mono[100];
  double left[100];
  double right[100];


  for (unsigned i = 0; i < 100; ++i)
  {
    mono[i] = sin(4.0 * M_PI * i / 100.0);
    left[i] = 999.;
    right[i] = -999.;
  }

  MonoToStereoD(left, right, mono, 100);

  for (unsigned i = 0; i < 100; ++i)
  {
    ASSERT_DOUBLE_EQ(left[i], right[i]);
  }
}

