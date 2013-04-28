/*
 *  FtAudioUtilities.c
 *  c
 *
 *  Created by Hamilton Kibbe on 6/6/12.
 *  Copyright 2012 HTK Audio. All rights reserved.
 *
 */

#include "FtAudioUtilities.h"



inline float to_dB(float ratio)
{
	return 20.0*log10((double)ratio);
}

inline float to_ratio(float dB)
{
	return pow(10.0,(dB/20.0));
}


inline float radians_to_Hz(float radians, long long sampleRate)
{
	return radians * (sampleRate/(2.0 * M_PI));
}


inline float Hz_to_radians(float Hz, long long sampleRate)
{
	return 2.0 * M_PI * Hz/sampleRate;
}

