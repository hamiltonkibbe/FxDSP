/**
 * @file        WindowFunction.h
 * @author      Hamilton Kibbe
 * @copyright   2012 Hamilton Kibbe. All rights reserved.
 */

#ifndef WINDOWFUNCTION_H_
#define WINDOWFUNCTION_H_

#include <math.h>
#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif

/** WindowFunction type */
typedef struct WindowFunction WindowFunction;
typedef struct WindowFunctionD WindowFunctionD;

/** Window function type */
typedef enum _Window_t
{
    /** Rectangular window */
    BOXCAR,

    /** Hann window */
    HANN,

    /** Hamming window */
    HAMMING,

    /** Blackman window */
    BLACKMAN,

    /** Tukey window */
    TUKEY,

    /** Cosine window */
    COSINE,

    /** Lanczos window */
    LANCZOS,

    /** Bartlett window */
    BARTLETT,

    /** Gauss window */
    GAUSSIAN,

    /** Bartlett-Hann window */
    BARTLETT_HANN,

    /** Kaiser window */
    KAISER,

    /** Nuttall window */
    NUTTALL,

    /** Blackaman-Harris window */
    BLACKMAN_HARRIS,

    /** Blackman-Nuttall window */
    BLACKMAN_NUTTALL,

    /** Flat top window */
    FLATTOP,

    /** Poisson window */
    POISSON,

    /** The number of window types */
    N_WINDOWTYPES
} Window_t;


/** Generate a Boxcar window of length n
 *
 * @details Create an n-point boxcar window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
boxcar(unsigned n, float* dest);
Error_t
boxcarD(unsigned n, double* dest);


/** Generate a Hann window of length n
 *
 * @details Create an n-point Hann window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
hann(unsigned n, float* dest);
Error_t
hannD(unsigned n, double* dest);

/** Generate a Hamming window of length n
 *
 * @details Create an n-point Hamming window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
hamming(unsigned n, float* dest);

Error_t
hammingD(unsigned n, double* dest);


/** Generate a Blackman window of length n for given alpha
 *
 * @details Create an n-point Blackman window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param a     Alpha value
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
blackman(unsigned n, float a, float* dest);

Error_t
blackmanD(unsigned n, double a, double* dest);

/** Generate a Tukey window of length n for given alpha
 *
 * @details Create an n-point Tukey window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param a     Alpha value
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
tukey(unsigned n, float a, float* dest);

Error_t
tukeyD(unsigned n, double a, double* dest);


/** Generate a cosine window of length n
 *
 * @details Create an n-point Cosine window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
cosine(unsigned n, float* dest);

Error_t
cosineD(unsigned n, double* dest);


/** Generate a Lanczos window of length n
 *
 * @details Creates an n-point Lanczos window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
lanczos(unsigned n, float* dest);

Error_t
lanczosD(unsigned n, double* dest);


/** Generate a Bartlett window of length n
 *
 * @details Creates an n-point Bartlett window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
bartlett(unsigned n, float* dest);

Error_t
bartlettD(unsigned n, double* dest);


/** Generate a Gaussian window of length n for given sigma
 *
 * @details Creates an n-point Gaussian window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param sigma Sigma value
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
gaussian(unsigned n, float sigma, float* dest);

Error_t
gaussianD(unsigned n, double sigma, double* dest);


/** Generate a Bartlett-Hann window of length n
 *
 * @details Creates an n-point Bartlett-Hann window in the supplied buffer.
 *          Does notallocate memory, so the user is responsible for ensuring
 *          that the destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
bartlett_hann(unsigned n, float* dest);

Error_t
bartlett_hannD(unsigned n, double* dest);

/** Generate a Kaiser window of length n for given alpha
 *
 * @details Create an n-point Kaiser window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param a     Alpha value = (Beta / PI)
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
kaiser(unsigned n, float a, float* dest);

Error_t
kaiserD(unsigned n, double a, double* dest);


/** Generate a Nuttall window of length n
 *
 * @details Create an n-point Nuttall window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
nuttall(unsigned n, float* dest);

Error_t
nuttallD(unsigned n, double* dest);


/** Generate a Blackman-Harris window of length n
 *
 * @details Create an n-point Blackman-Harris window in the supplied buffer.
 *          Does not allocate memory, so the user is responsible for ensuring
 *          that the destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
blackman_harris(unsigned n, float* dest);

Error_t
blackman_harrisD(unsigned n, double* dest);


/** Generate a Blackman-Nuttall window of length n
 *
 * @details Create an n-point Blackman-Nuttall window in the supplied buffer.
 *          Does not allocate memory, so the user is responsible for ensuring
 *          that the destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
blackman_nuttall(unsigned n, float* dest);

Error_t
blackman_nuttallD(unsigned n, double* dest);


/** Generate a flat top window of length n
 *
 * @details Create an n-point flat top window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
flat_top(unsigned n, float* dest);

Error_t
flat_topD(unsigned n, double* dest);


/** Generate a Poisson window of length n and given D
 *
 * @details Create an n-point Poisson    window in the supplied buffer. Does not
 *          allocate memory, so the user is responsible for ensuring that the
 *          destination buffer can hold the entire window
 *
 * @param n     The length of the window
 * @param D     Target decay in dB over 1/2 window length
 * @param dest  Buffer where the window is written. Buffer size must be at
 *              least n * sizeof(float)
 * @return      Error code, 0 on success
 */
Error_t
poisson(unsigned n, float D, float* dest);

Error_t
poissonD(unsigned n, double D, double* dest);




Error_t
chebyshev(unsigned n, float A, float* dest);

Error_t
chebyshevD(unsigned n, double A, double* dest);



/** Create a new WindowFunction
 *
 * @details Allocates memory and returns an initialized WindowFunction.
 *          Play nice and call WindowFunctionFree on the window when
 *          you're done with it.
 *
 * @param n     Number of points in the window.
 * @param type  Type of window function to generate.
 * @return          Error code, 0 on success
 */
WindowFunction*
WindowFunctionInit(unsigned n, Window_t type);

WindowFunctionD*
WindowFunctionInitD(unsigned n, Window_t type);


/** Free memory associated with a WindowFunction
 *
 * @details release all memory allocated by WindowFunctionInit for the
 *          supplied window.
 *
 * @param window    The window to free
 * @return          Error code, 0 on success
 */
Error_t
WindowFunctionFree(WindowFunction* window);

Error_t
WindowFunctionFreeD(WindowFunctionD* window);


/** Window a buffer of samples
 *
 * @details Applies the window to the buffer of samples passed to it
 *
 * @param window    The WindowFunction to use
 * @param outBuffer The buffer to write the output to
 * @param inBuffer  The buffer to filter
 * @param n_samples The number of samples to window
 * @return          Error code, 0 on success
 */
Error_t
WindowFunctionProcess(WindowFunction*   window,
                      float*            outBuffer,
                      const float*      inBuffer,
                      unsigned          n_samples);

Error_t
WindowFunctionProcessD(WindowFunctionD* window,
                       double*          outBuffer,
                       const double*    inBuffer,
                       unsigned         n_samples);


#ifdef __cplusplus
}
#endif

#endif /* WINDOWFUNCTION_H_ */
