/**
 * @file Utilities.h
 * @author Hamilton Kibbe
 * @copyright 2013 Hamilton Kibbe
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>

/* Macro Constants ************************************************************/

/* Scalar for converting int to float samples (1/32767.0) */
#define INT16_TO_FLOAT_SCALAR (0.00003051850947599719f)

/* 1/ln(2) */
#define INV_LN2 (1.442695040888963f)

/* 2*pi */
#define TWO_PI (6.283185307179586f)

/* pi/2 */
#define PI_OVER_TWO (1.5707963267948966f)

/* 1/(TWO_PI) */
#define INVERSE_TWO_PI (0.159154943091895f)

/* ln(10.0)/20.0 */
#define LOG_TEN_OVER_TWENTY (0.11512925464970228420089957273422)

/* 20.0/ln(10.0) */
#define TWENTY_OVER_LOG_TEN (8.6858896380650365530225783783321)

#define SQRT_TWO_OVER_TWO (0.70710678118654757273731092936941422522068023681641)

/* Utility Function Macros ****************************************************/

/* Limit value value to the range (l, u) */
#define LIMIT(value,lower,upper) ((value) < (lower) ? (lower) : \
                                 ((value) > (upper) ? (upper) : (value)))


/* Linearly interpolate between y0 and y1

  y(x) = (1 - x) * y(0) + x * y(1) | x in (0, 1)

 Function-style signature:
    float LIN_INTERP(float x, float y0, float y1)
    double LIN_INTERP(double x, double y0, double y1)

 Parameters:
    x:  x-value at which to calculate y.
    y0: y-value at x = 0.
    y1: y-value at x = 1.

 Returns:
    Interpolated y-value at specified x.
 */
#define LIN_INTERP(x, y0, y1) ((y0) + (x) * ((y1) - (y0)))


/* Convert frequency from Hz to Radians per second

 Function-style signature:
    float HZ_TO_RAD(float frequency)
    double HZ_TO_RAD(double frequency)

 Parameters:
    frequency:  Frequency in Hz.

 Returns:
    Frequency in Radians per second.
 */
#define HZ_TO_RAD(f) (TWO_PI * (f))


/* Convert frequency from Radians per second to Hz

 Function-style signature:
    float RAD_TO_HZ(float frequency)
    double RAD_TO_HZ(double frequency)

 Parameters:
 frequency:  Frequency in Radians per second.

 Returns:
 Frequency in Hz.
 */
#define RAD_TO_HZ(omega) (INVERSE_TWO_PI * (omega))


/* Fast exponentiation function

 y = e^x

 Function-style signature:
    float F_EXP(float x)
    double F_EXP(double x)

 Parameters:
    x:  Value to exponentiate.

 Returns:
    e^x.
 */
#define F_EXP(x) ((362880 + (x) * (362880 + (x) * (181440 + (x) * \
                  (60480 + (x) * (15120 + (x) * (3024 + (x) * (504 + (x) * \
                  (72 + (x) * (9 + (x) ))))))))) * 2.75573192e-6)


/* Decibel to Amplitude Conversion */
#define DB_TO_AMP(x) ((x) > -150.0 ? expf((x) * LOG_TEN_OVER_TWENTY) : 0.0f)
#define DB_TO_AMPD(x) ((x) > -150.0 ? exp((x) * LOG_TEN_OVER_TWENTY) : 0.0)

/* Amplitude to Decibel Conversion */
#define AMP_TO_DB(x) (((x) < 0.0000000298023223876953125) ? -150.0 : \
                      (logf(x) * TWENTY_OVER_LOG_TEN))
#define AMP_TO_DBD(x) (((x) < 0.0000000298023223876953125) ? -150.0 : \
                       (log(x) * TWENTY_OVER_LOG_TEN))

/* Smoothed Absolute Value */
#define SMOOTH_ABS(x) (sqrt(((x) * (x)) + 0.025) - sqrt(0.025))


#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations ******************************************************/

/* Define log2 and log2f for MSVC */
#if !defined(log2) || !defined(log2f)
#define _USE_FXDSP_LOG

double
log2(double n);

float
log2f(float n);

#endif


/**  Find the nearest power of two
 * @param x     number to process
 * @return      Absolute value of f.
 */
int
next_pow2(int x);


/**  Fast absolute value
 * @details     Fast fabs() implementation
 * @param f     Value to process
 * @return      Absolute value of f.
 */
float
f_abs(float f);


/**  Max of two floats
 * @details branchless max() implementation
 * @param   x first value to compare,
 * @param   a second value to compare.
 * @return  the maximum value of the two arguments.
 */
float
f_max(float x, float a);


/**  Min of two floats
 * @details branchless min() implementation
 * @param   x first value to compare,
 * @param   a second value to compare.
 * @return  the minimum value of the two arguments.
 */
float
f_min(float x, float b);


/**  Clamp values to range
 * @details branchless LIMIT() implementation
 * @param x value to clamp
 * @param a lower bound
 * @param b upper bound
 * @return  val clamped to range (a, b)
 */
float
f_clamp(float x, float a, float b);


/** Calculate pow(2, x)
 * @details fast, branchless pow(2,x) approximation
 * @param x     power of 2 to calculate.
 * @return      2^x
 */
float
f_pow2(float x);


/** Calculate tanh_x
* @details fast tanh approximation
* @param x     input
* @return      ~tanh(x)
*/
float
f_tanh(float x);


/** Convert signed sample to float
 *
 * @details convert a signed 16 bit sample to a 32 bit float sample in the range
 * [-1.0, 1,0]
 *
 * @param sample    The sample to convert.
 * @return          The sample as a float.
 */
float
int16ToFloat(signed short sample);


/** Convert a float sample to signed
 *
 * @details convert a 32 bit float sample in the range [-1.0, 1,0] to a signed
 * 16 bit sample.
 *
 * @param sample    The sample to convert.
 * @return          The sample as a 16-bit signed int.
 */
signed short
floatToInt16(float sample);


/** Convert an amplitude to dB
 * @details     Convert a voltage amplitude to dB.
 * @param amp   The amplitude to convert.
 * @return      Amplitude value in dB.
 */
float
AmpToDb(float ratio);

double
AmpToDbD(double ratio);


/** Convert a value in dB to an amplitude
 * @details convert a dBFS value to a voltage amplitude
 * @param dB        The value in dB to convert.
 * @return          dB value as a voltage amplitude.
 */
float
DbToAmp(float dB);

double
DbToAmpD(double dB);


/** Convert complex value to magnitude/phase
 * @param real      Real part of input.
 * @param imag      Imaginary part of input.
 * @param outMag    Magnitude output.
 * @param outPhase  Phase output.
 */
void
RectToPolar(float real, float imag, float* outMag, float* outPhase);

void
RectToPolarD(double real, double imag, double* outMag, double* outPhase);


/** Convert magnitude/phase to complex
 * @param mag       Magnitude input.
 * @param phase     Phase input.
 * @param outReal   Real part output.
 * @param outImag   Imaginary part output.
 */
void
PolarToRect(float mag, float phase, float* outReal, float* outImag);

void
PolarToRectD(double mag, double phase, double* outReal, double* outImag);


#ifdef __cplusplus
}
#endif

#endif /* UTILITIES_H */
