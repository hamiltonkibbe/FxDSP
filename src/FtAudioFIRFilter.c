/*
 * FtAudioFIRFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioFIRFilter.h"
#include <stdio.h>

struct FtAudioFIRFilter
{
	float * kernel;
	const float * kernel_end;
	float * overlap;
	unsigned kernel_length;
	unsigned overlap_length;
};



FtAudioFIRFilter* 
FtAudioFIRFilterInit(const float*	filter_kernel,
					 unsigned length)
{
	float zero = 0.0;

	// Array lengths and sizes
	unsigned kernel_length = length;						// IN SAMPLES!
	unsigned overlap_length = kernel_length - 1;			// IN SAMPLES!
	size_t kernel_size = kernel_length * sizeof(float);		//IN BYTES!
	size_t overlap_size = overlap_length * sizeof(float);	//IN BYTES!

	// Allocate Memory
	FtAudioFIRFilter* filter = (FtAudioFIRFilter*)malloc(sizeof(FtAudioFIRFilter));
	float* kernel = (float*)malloc(kernel_size);
	float* overlap = (float*)malloc(overlap_size);


	// Initialize Buffers
	memcpy(kernel, filter_kernel, kernel_size);
    vDSP_vfill(&zero, overlap, 1, overlap_length);

	filter->kernel = kernel;
	filter->kernel_end = filter_kernel + (kernel_length - 1);
	filter->overlap = overlap;
	filter->kernel_length = kernel_length;
	filter->overlap_length = overlap_length;
	return filter;
}



FtAudioError_t 
FtAudioFIRFilterFree(FtAudioFIRFilter * filter)
{
	free(filter->kernel);
	free(filter->overlap);
	free(filter);
	return FT_NOERR;
}

FtAudioError_t
FtAudioFIRFilterFlush(FtAudioFIRFilter* filter)
{
	float zero = 0.0;
	vDSP_vfill(&zero, filter->overlap, 1, filter->overlap_length);
	return FT_NOERR;
}

FtAudioError_t
FtAudioFIRFilterProcess(FtAudioFIRFilter* filter,
						float*	outBuffer, 
						const float* inBuffer, 
						unsigned n_samples)
{
	float zero = 0.0;
	unsigned resultLength = n_samples + (filter->kernel_length - 1);
	unsigned signalLength = (filter->kernel_length + resultLength);
	
	// Temporary buffer to store full result of filtering..
	float buffer[resultLength];
	
	// So there's some hella weird requirement that the signal input to vDSP_conv has to be
	// larger than (result_length + filter_length - 1)_, and it has to be zero-padded. What. The. Fuck!
	// Pad the input signal with (filter_length - 1) zeros.
	float padded[signalLength];
	vDSP_vfill(&zero, padded, 1, signalLength);
	memcpy(padded  + (filter->kernel_length - 1), inBuffer, n_samples * sizeof(float));
	
	// Filter the samples
	vDSP_conv(padded, 1, filter->kernel_end, -1, buffer, 1, resultLength, filter->kernel_length);
	 
	// Add in the overlap from the last block
	vDSP_vadd(buffer, 1, filter->overlap, 1, buffer, 1, filter->overlap_length);
	
	// Save the overlap from this block
	memcpy(filter->overlap, buffer + n_samples, filter->overlap_length * sizeof(float));
	
	// write output
	memcpy(outBuffer, buffer, n_samples * sizeof(float));
	return FT_NOERR;
}


FtAudioError_t
FtAudioFIRFilterUpdateKernel(FtAudioFIRFilter*	filter, const float* filter_kernel)
{
	memcpy(filter->kernel, filter_kernel, filter->kernel_length * sizeof(float));
	return FT_NOERR;
}