/*
 * FtAudioConvolution.c
 * Hamilton Kibbe
 * Copyright 2013 Hamilton Kibbe
 */
#include <string.h>
#include "FtAudioConvolution.h"
#include "FtAudioUtilities.h"

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

/* FtAudioConvolve ************************************************/
FtAudioError_t
FtAudioConvolve(float       *in1,
                unsigned    in1_length,
                float       *in2, 
                unsigned    in2_length, 
                float       *dest)
{
	
	 unsigned resultLength = in1_length + (in2_length - 1);
	
#ifdef __APPLE__
    //Use Native vectorized convolution function if available
    float    *in2_end = in2 + (in2_length - 1);
    unsigned signalLength = (in2_length + resultLength);
    
    // So there's some hella weird requirement that the signal input to 
    //vDSP_conv has to be larger than (result_length + filter_length - 1),
    // and it has to be zero-padded. What. The. Fuck!
    float padded[signalLength];
    
    //float zero = 0.0;
    //vDSP_vfill(&zero, padded, 1, signalLength);
    FtAudioFillBuffer(padded, signalLength, 0.0);
    
    // Pad the input signal with (filter_length - 1) zeros.
    memcpy(padded  + (in2_length - 1), in1, in1_length * sizeof(float));
    vDSP_conv(padded, 1, in2_end, -1, dest, 1, resultLength, in2_length);
#else
    // Use (boring, slow) canonical implementation
	unsigned i;

	for (i = 0; i <resultLength; ++i)
	{
		unsigned kmin, kmax, k;
		dest[i] = 0;
			
		kmin = (i >= (in2_length - 1)) ? i - (in2_length - 1) : 0;
		kmax = (i < in1_length - 1) ? i : in1_length - 1;
		for (k = kmin; k <= kmax; k++)
		{
			dest[i] += in1[k] * in2[i - k];
		}
	}	
	
	
#endif
	return FT_NOERR;

}
