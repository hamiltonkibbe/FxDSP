//
//  FilterTypes.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#ifndef FxDSP_FilterTypes_h
#define FxDSP_FilterTypes_h

#ifdef __cplusplus
extern "C" {
#endif


/** Filter types */
typedef enum Filter_t
{
    /** Lowpass */
    LOWPASS,

    /** Highpass */
    HIGHPASS,

    /** Bandpass */
    BANDPASS,

    /** Allpass */
    ALLPASS,

    /** Notch */
    NOTCH,

    /** Peaking */
    PEAK,

    /** Low Shelf */
    LOW_SHELF,

    /** High Shelf */
    HIGH_SHELF,

    /** Number of Filter types */
    N_FILTER_TYPES
}Filter_t;


#ifdef __cplusplus
}
#endif

#endif
