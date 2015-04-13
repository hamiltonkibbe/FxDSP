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
typedef struct MultibandFilterD MultibandFilterD;


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
MultibandFilterInit(float   lowCutoff,
                    float   highCutoff,
                    float   sampleRate);
    
MultibandFilterD*
MultibandFilterInitD(double lowCutoff,
                     double highCutoff,
                     double sampleRate);


/** Free memory associated with a MultibandFilter
 *
 * @details release all memory allocated by MultibandFilterInit for the
 *          supplied filter.
 * @param filter    MultibandFilter to free.
 * @return          Error code, 0 on success
 */
Error_t
MultibandFilterFree(MultibandFilter* filter);
    
Error_t
MultibandFilterFreeD(MultibandFilterD* filter);


/** Flush filter state buffers
 *
 * @param filter    MultibandFilter to flush.
 * @return          Error code, 0 on success
 */
Error_t
MultibandFilterFlush(MultibandFilter* filter);
    
Error_t
MultibandFilterFlushD(MultibandFilterD* filter);


/** Set low/lid band crossover point
 *
 * @param filter    MultibandFilter to update.
 * @param lowCutoff New low/mid band crossover point
 * @return          Error code, 0 on success
 */
Error_t
MultibandFilterSetLowCutoff(MultibandFilter* filter, float lowCutoff);
    
Error_t
MultibandFilterSetLowCutoffD(MultibandFilterD* filter, double lowCutoff);

    
/** Set mid/high band crossover point
 *
 * @param filter    MultibandFilter to update.
 * @param highCutoff New mid/high band crossover point
 * @return          Error code, 0 on success
 */
Error_t
MultibandFilterSetHighCutoff(MultibandFilter* filter, float highCutoff);

Error_t
MultibandFilterSetHighCutoffD(MultibandFilterD* filter, double highCutoff);
    

    
/** Update filter crossover points
 *
 * @param filter    MultibandFilter to update.
 * @param lowCutoff New low/mid band crossover point
 * @param highCutoff New mid/high band crossover point
 * @return          Error code, 0 on success
 */
Error_t
MultibandFilterUpdate(MultibandFilter*  filter,
                      float             lowCutoff,
                      float             highCutoff);
    
Error_t
MultibandFilterUpdateD(MultibandFilterD*    filter,
                       double               lowCutoff,
                       double               highCutoff);


    
/** Process a buffer of samples
 * @details Split signal 3 signals, one for each frequency band.
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
                           float*       lowOut,
                           float*       midOut,
                           float*       highOut,
                           const float* inBuffer,
                           unsigned     n_samples);
    
Error_t
MultibandFilterProcessD(MultibandFilterD*   filter,
                        double*             lowOut,
                        double*             midOut,
                        double*             highOut,
                        const double*       inBuffer,
                        unsigned            n_samples);


#ifdef __cplusplus
    }
#endif

#endif
