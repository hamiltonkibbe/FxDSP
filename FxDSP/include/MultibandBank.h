//
//  MultibandBank.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#ifndef FxDSP_MultibandBank_h
#define FxDSP_MultibandBank_h

#include "Error.h"


#ifdef __cplusplus
extern "C" {
#endif
    
    
/** Opaque MultibandFilter structure */
typedef struct MultibandFilter MultibandFilter;


/** Create a new MultibandFilter
 *
 * @details Allocates memory and returns an initialized MultibandFilter.
 *          Play nice and call MultibandFilterFree on the filter when
 *          you're done with it.
 *
 * @param lowCutoff     Low/Mid band split frequency
 * @param highCutoff    Mid/High band split frequency
 * @param sampleRate
 * @return              An initialized MultibandFilter.
 */
MultibandFilter*
MultibandFilterInit(float       lowCutoff,
                    float       highCutoff,
                    long long   sampleRate);


/** Free memory associated with a MultibandFilter
 *
 * @details release all memory allocated by MultibandFilterInit for the
 *          supplied filter.
 * @param filter    MultibandFilter to free.
 * @return          Error code, 0 on success
 */
Error_t
MultibandFilterFree(MultibandFilter* filter);


/** Flush filter state buffers
 *
 * @param filter    MultibandFilter to flush.
 * @return          Error code, 0 on success
 */
Error_t
MultibandFilterFlush(MultibandFilter* filter);


Error_t
MultibandFilterSetLowCutoff(MultibandFilter* filter, float lowCutoff);

Error_t
MultibandFilterSetHighCutoff(MultibandFilter* filter, float highCutoff);

    Error_t
    MultibandFilterUpdate(MultibandFilter*  filter,
                          float             lowCutoff,
                          float             highCutoff);
    
/** Filter a buffer of samples
 * @details Uses a DF-I biquad implementation to filter input samples
 *
 * @param filter    The MultibandFilter to use.
 * @param lowOut    The buffer to write the low band output to.
 * @param midOut    The buffer to write the mid band output to.
 * @param highOut   The buffer to write the high band output to.
 * @param inBuffer  The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return          Error code, 0 on success
 */
Error_t
MultibandFilterProcess(MultibandFilter* filter,
                           float*               lowOut,
                           float*               midOut,
                           float*               highOut,
                           const float*         inBuffer,
                           unsigned             n_samples);


#ifdef __cplusplus
    }
#endif

#endif
