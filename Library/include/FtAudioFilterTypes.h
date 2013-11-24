//
//  FtAudioFilterTypes.h
//  FutureTechAudio
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#ifndef FutureTechAudio_FtAudioFilterTypes_h
#define FutureTechAudio_FtAudioFilterTypes_h

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
