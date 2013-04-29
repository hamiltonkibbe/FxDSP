/** 
 * @file FtAudioRBJFilter.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */
 
#ifndef FTAUDIORBJFILTER_H_
#define FTAUDIORBJFILTER_H_

#include "FtAudioError.h"
#include "FtAudioUtilities.h"
#include "FtAudioBiquadFilter.h"

#ifdef __cplusplus
extern "C" {
#endif

/** RBJ filter type */
typedef enum RBJFilter_t
{
	/** Lowpass */
	RBJ_LPF,

	/** Highpass */
	RBJ_HPF,

	/** Bandpass */
	RBJ_BPF,

	/** Allpass */
	RBJ_APF,  

	/** Notch */
	RBJ_NOTCH,

	/** Peaking */
	RBJ_PEAK,

	/** Low Shelf */
	RBJ_LSHELF,

	/** High Shelf */
	RBJ_HSHELF,

	/** Number of RBJ Filter types */
	RBJ_N_TYPES
}RBJFilter_t;


/** Opaque FtAudioRBJFilter structure */
typedef struct FtAudioRBJFilter FtAudioRBJFilter;

/** Update FtAudioRBJFilter coefficients
 *  
 * @details recalculates filter coefficients based on filter parameters. This
 *			is called when updating any of the filter parameters.
 *
 * @param filter	FtAudioRBJFilter to update
 * @return			Error code, 0 on success
 */
static FtAudioError_t
FtAudioRBJFilterUpdate(FtAudioRBJFilter* filter);


/** Create a new FtAudioRBJFilter
 *
 * @details Allocates memory and returns an initialized FtAudioRBJFilter.
 *			Play nice and call FtAudioRBJFilterFree on the filter when you're
 *          done with it.
 *
 * @param type			The filter type
 * @param cutoff		The starting cutoff frequency to use
 * @param sampleRate	The sample rate in Samp/s
 * @return 				An initialized FtAudioRBJFilter	
 */
FtAudioRBJFilter* 
FtAudioRBJFilterInit(RBJFilter_t 	type, 
					 float 			cutoff,
					 long long 		sampleRate);


/** Free memory associated with a FtAudioRBJFilter
 *  
 * @details release all memory allocated by FtAudioRBJFilterInit for the
 *			supplied filter.
 *
 * @param filter	FtAudioRBJFilter to free
 * @return			Error code, 0 on success
 */
FtAudioError_t 
FtAudioRBJFilterFree(FtAudioRBJFilter* 	filter);

/** Update FtAudioRBJFilter type
 *  
 * @details Update the filter type and recalculate filter coefficients.
 *
 * @param filter	FtAudioRBJFilter to update
 * @param type		New filter type
 * @return			Error code, 0 on success
 */
FtAudioError_t 
FtAudioRBJFilterSetType(FtAudioRBJFilter* 	filter, 
						RBJFilter_t 		type);

/** Update FtAudioRBJFilter Cutoff
 *  
 * @details Update the filter cutoff/center frequency and recalculate filter
 *			coefficients.
 *
 * @param filter	FtAudioRBJFilter to update
 * @param cutoff	New filter cutoff/center frequency
 * @return			Error code, 0 on success
 */
FtAudioError_t 
FtAudioRBJFilterSetCutoff(FtAudioRBJFilter* filter,
						  float 			cutoff);

/** Update FtAudioRBJFilter Q
 *  
 * @details Update the filter Q and recalculate filter coefficients.
 *
 * @param filter	FtAudioRBJFilter to update
 * @param Q			New filter Q
 * @return			Error code, 0 on success
 */
FtAudioError_t 
FtAudioRBJFilterSetQ(FtAudioRBJFilter* 	filter, 
					 float 				Q);

/** Filter a buffer of samples
 * @details Uses an RBJ-style filter to filter input samples
 *
 * @param filter	The FtAudioRBJFilter to use.
 * @param outBuffer	The buffer to write the output to.
 * @param inBuffer	The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return			Error code, 0 on success
 */
FtAudioError_t
FtAudioRBJFilterProcess(FtAudioRBJFilter* 	filter,
						float* 				outBuffer,
						const float* 		inBuffer, 
						unsigned 			n_samples);



#ifdef __cplusplus
}
#endif

#endif /* FTAUDIORBJFILTER_H_ */
