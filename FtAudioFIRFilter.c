/*
 * FtAudioFIRFilter.c
 * Hamilton Kibbe
 * Copyright 2012 Hamilton Kibbe
 */

#include "FtAudioFIRFilter.h"


struct FtAudioFIRFilter
{
	float * kernel;
	float * kernel_end;
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
	unsigned kernel_length = length;
	unsigned overlap_length = kernel_length - 1;
	size_t kernel_size = kernel_length * sizeof(float);
	size_t overlap_size = overlap_length * sizeof(float);

	// Allocate Memory
	FtAudioFIRFilter* filter = (FtAudioFIRFilter*)malloc(sizeof(FtAudioFIRFilter));
	float* kernel = (float*)malloc(kernel_size);
	float* overlap = (float*)malloc(overlap_size);


	// Initialize Buffers
	memcpy(kernel, filter_kernel, kernel_size);
    vDSP_vfill(&zero, overlap, 1, overlap_length);

	filter->kernel = kernel;
	filter->kernel_end = filter_kernel + kernel_length - 1;
	filter->overlap = overlap;
	filter->kernel_length = kernel_length;
	filter->overlap_length = overlap_length;
	
	return filter;
}



void FtAudioFIRFilterFree(FtAudioFIRFilter * filter)
{
	free(filter->kernel);
	free(filter->overlap);
	free(filter);
}

FtAudioError_t
FtAudioFIRFilterFlush(FtAudioFIRFilter* filter)
{
	float zero = 0.0;
	vDSP_vfill(&zero, filter->overlap, 1, filter->overlap_length);
}

FtAudioError_t
FtAudioFIRFilterProcess(FtAudioFIRFilter* filter,
						float*	outBuffer, 
						const float* inBuffer, 
						unsigned n_samples)
{
	// Local scratch buffer
	float buffer[n_samples + filter->overlap_length];
	
	// Filter the samples
	vDSP_conv(inBuffer, 1, filter->kernel_end, -1, buffer, 1, n_samples + filter->overlap_length);
	vDSP_vadd(buffer, 1, filter->overlap, 1, buffer, 1, filter->overlap_length);
	memcpy(filter->overlap, buffer + n_samples, filter->overlap_length * sizeof(float);
	
	// write output
	memcpy(outBuffer, buffer, n_samples * sizeof(float));
}


FtAudioError_t
FtAudioFIRFilterUpdateKernel(FtAudioFIRFilter*	filter, const float* filter_kernel)
{
	memcpy(filter->kernel, filter_kernel, filter->kernel_length * sizeof(float));
}