//
//  bs1770.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 5/27/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#ifndef BS1770_H_
#define BS1770_H_

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BS1770Meter BS1770Meter;
typedef struct BS1770MeterD BS1770MeterD;
typedef struct KWeightingFilter KWeightingFilter;
typedef struct KWeightingFilterD KWeightingFilterD;


KWeightingFilter*
KWeightingFilterInit(float sample_rate);

KWeightingFilterD*
KWeightingFilterInitD(double sample_rate);

Error_t
KWeightingFilterProcess(KWeightingFilter*   filter,
                        float*              dest,
                        const float*        src,
                        unsigned            length);

Error_t
KWeightingFilterProcessD(KWeightingFilterD* filter,
                         double*            dest,
                         const double*      src,
                         unsigned           length);


Error_t
KWeightingFilterFlush(KWeightingFilter* filter);

Error_t
KWeightingFilterFlushD(KWeightingFilterD* filter);


Error_t
KWeightingFilterFree(KWeightingFilter* filter);

Error_t
KWeightingFilterFreeD(KWeightingFilterD* filter);


BS1770Meter*
BS1770MeterInit(unsigned n_channels, float sample_rate);

BS1770MeterD*
BS1770MeterInitD(unsigned n_channels, double sample_rate);

Error_t
BS1770MeterProcess(BS1770Meter*     meter,
                   float*           loudness,
                   float**          peaks,
                   const float**    samples,
                   unsigned         n_samples);

Error_t
BS1770MeterProcessD(BS1770MeterD*   meter,
                    double*         loudness,
                    double**        peaks,
                    const double**  samples,
                    unsigned        n_samples);

Error_t
BS1770MeterFree(BS1770Meter* meter);

Error_t
BS1770MeterFreeD(BS1770MeterD* meter);

#ifdef __cplusplus
}
#endif

#endif /* defined(BS1770_H_) */
