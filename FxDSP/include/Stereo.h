//
//  Stereo.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/25/16.
//  Copyright © 2016 Hamilton Kibbe. All rights reserved.
//

#ifndef Stereo_h
#define Stereo_h

#ifdef __cplusplus
extern "C" {
#endif

void
StereoToMono(float*        dest,
             const float*  left,
             const float*  right,
             unsigned      length);

void
StereoToMonoD(double*        dest,
              const double*  left,
              const double*  right,
              unsigned       length);

void
MonoToStereo(float*         left,
             float*         right,
             const float*   mono,
             unsigned       length);

void
MonoToStereoD(double*       left,
              double*       right,
              const double* mono,
              unsigned      length);

#ifdef __cplusplus
}
#endif

#endif /* Stereo_h */
