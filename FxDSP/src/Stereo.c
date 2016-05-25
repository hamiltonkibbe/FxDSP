//
//  Stereo.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/25/16.
//  Copyright © 2016 Hamilton Kibbe. All rights reserved.
//

#include "Stereo.h"
#include "Utilities.h"
#include "Dsp.h"


void
StereoToMono(float*        dest,
             const float*  left,
             const float*  right,
             unsigned      length)
{
  float scale = SQRT_TWO_OVER_TWO;
  VectorVectorSumScale(dest, left, right, &scale, length);
}


void
StereoToMonoD(double*       dest,
              const double* left,
              const double* right,
              unsigned      length)
{
  double scale = SQRT_TWO_OVER_TWO;
  VectorVectorSumScaleD(dest, left, right, &scale, length);
}



void
MonoToStereo(float*         left,
             float*         right,
             const float*   mono,
             unsigned       length)
{
  float scale = SQRT_TWO_OVER_TWO;
  VectorScalarMultiply(left, mono, scale, length);
  CopyBuffer(right, left, length);
}

void
MonoToStereoD(double*       left,
              double*       right,
              const double* mono,
              unsigned      length)
{
  double scale = SQRT_TWO_OVER_TWO;
  VectorScalarMultiplyD(left, mono, scale, length);
  CopyBufferD(right, left, length);
}