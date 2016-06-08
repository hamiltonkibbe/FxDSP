/**
 * @file        LadderFilter.h
 * @author      Hamilton Kibbe <ham@hamiltonkib.be>
 * @copyright   2014 Hamilton Kibbe. All rights reserved.
 * @brief       VA Moog VCF
 *
 * A Virtual Analog implementation of the Moog Ladder Filter. Based on
 * "Non-Linear Digital Implementation of the Moog Ladder Filter" by Antti
 * Huovilainen
 *
 */

#ifndef LADDERFILTER_H_
#define LADDERFILTER_H_

#include <string.h>
#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Boltzman's constant */
static const float BOLTZMANS_CONSTANT = 1.38e-23;

/** Magnitude of electron charge */
static const float Q = 1.609e-19;

/** Opaque LadderFilter structure */
typedef struct LadderFilter LadderFilter;


/** Initialize a Ladder Filter
 *
 *  @details allocates memory on the heap for the filter. Make sure to call
 *  LadderFilterFree when you're finished.
 *
 */
LadderFilter*
LadderFilterInit(float _sample_rate);


Error_t
LadderFilterFree(LadderFilter* filter);


Error_t
LadderFilterFlush(LadderFilter* filter);


Error_t
LadderFilterProcess(LadderFilter  *filter,
                           float                *outBuffer,
                           float                *inBuffer,
                           unsigned             n_samples);


Error_t
LadderFilterSetCutoff(LadderFilter *filter, float _cutoff);


Error_t
LadderFilterSetResonance(LadderFilter *filter,
                                float               _resonance);


Error_t
LadderFilterSetTemperature(LadderFilter   *filter,
                                  float                 tempC);


#ifdef __cplusplus
}
#endif

#endif /* LADDERFILTER_H_ */