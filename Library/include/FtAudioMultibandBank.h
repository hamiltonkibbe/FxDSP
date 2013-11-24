//
//  FtAudioMultibandBank.h
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#ifndef FutureTechAudio_FtAudioMultibandBank_h
#define FutureTechAudio_FtAudioMultibandBank_h

#include "FtAudioError.h"


#ifdef __cplusplus
extern "C" {
#endif
    
    
/** Opaque FTA_MultibandFilter structure */
typedef struct FTA_MultibandFilter FTA_MultibandFilter;


/** Create a new FTA_MultibandFilter
 *
 * @details Allocates memory and returns an initialized FTA_MultibandFilter.
 *          Play nice and call FTA_MultibandFilterFree on the filter when
 *          you're done with it.
 *
 * @param lowCutoff     Low/Mid band split frequency
 * @param highCutoff    Mid/High band split frequency
 * @param sampleRate
 * @return              An initialized FTA_MultibandFilter.
 */
FTA_MultibandFilter*
FTA_MultibandFilterInit(float       lowCutoff,
                        float       highCutoff,
                        long long   sampleRate);


/** Free memory associated with a FTA_MultibandFilter
 *
 * @details release all memory allocated by FTA_MultibandFilterInit for the
 *          supplied filter.
 * @param filter    FTA_MultibandFilter to free.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_MultibandFilterFree(FTA_MultibandFilter* filter);


/** Flush filter state buffers
 *
 * @param filter    FTA_MultibandFilter to flush.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_MultibandFilterFlush(FTA_MultibandFilter* filter);



/** Filter a buffer of samples
 * @details Uses a DF-I biquad implementation to filter input samples
 *
 * @param filter    The FTA_MultibandFilter to use.
 * @param lowOut    The buffer to write the low band output to.
 * @param midOut    The buffer to write the mid band output to.
 * @param highOut   The buffer to write the high band output to.
 * @param inBuffer  The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return          Error code, 0 on success
 */
FTA_Error_t
FTA_MultibandFilterProcess(FTA_MultibandFilter* filter,
                           float*               lowOut,
                           float*               midOut,
                           float*               highOut,
                           const float*         inBuffer,
                           unsigned             n_samples);


#ifdef __cplusplus
    }
#endif

#endif
