/**
 * @file        DiodeRectifier.h
 * @author      Hamilton Kibbe <ham@hamiltonkib.be>
 * @copyright   2014 Hamilton Kibbe. All rights reserved.
 * @brief       A diode rectifier VA model for signal rectification.
 *
 * This diode model can be used instead of a hard threshold for rectifying a
 * signal. The `bias` parameter sets the configuration of the diode, such that
 * a diode with `FORWARD_BIAS` will pass positive signals and clamp negative
 * signals to 0, wheras a diode with `REVERSE_BIAS` will pass negative signals
 * and clamp positive signals to ground. The threshold parameter sets the
 * value at which the diode starts to pass the signal. The threshold value is
 * normalized to [0 1] and is independent of the `bias` setting.
 * e.g. for `threshold=0.5` and `FORWARD_BIAS` the output will be ~0.0 when the
 * input is less than 0.5 and ~input otherwise. For `threshold=0.5` and
 * `REVERSE_BIAS` the output will be ~0.0 when the input is greater than -0.5
 * and ~input otherwise.
 */


#ifndef DIODE_H_
#define DIODE_H_

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct DiodeRectifier DiodeRectifier;
typedef struct DiodeRectifierD DiodeRectifierD;

typedef enum _bias_t
{
    /** Pass positive signals, clamp netagive signals to 0 */
    FORWARD_BIAS,

    /** Pass negative signals, clamp positive signals to 0 */
    REVERSE_BIAS,

    /** Full-wave rectification. */
    FULL_WAVE
}bias_t;


/** Create a new DiodeRectifier
 *
 * @details Allocates memory and returns an initialized DiodeRectifier.
 *			Play nice and call DiodeRectifierFree when you're done with it.
 *
 * @param threshold         Normalized voltage threshold
 * @param bias              DiodeRectifier bias, FORWARD_BIAS or REVERSE_BIAS
 *                          Forward-bias will pass positive signals and clamp
 *                          negative signals to 0.
 * @return                  An initialized DiodeRectifier
 */
DiodeRectifier*
DiodeRectifierInit(bias_t bias, float threshold);

DiodeRectifierD*
DiodeRectifierInitD(bias_t bias, double threshold);


/** Free memory associated with a DiodeRectifier
 *
 * @details release all memory allocated by DiodeRectifierInit for the
 *			given DiodeRectifier.
 *
 * @param DiodeRectifier     DiodeRectifier to free
 * @return			Error code, 0 on success
 */
Error_t
DiodeRectifierFree(DiodeRectifier* diode);

Error_t
DiodeRectifierFreeD(DiodeRectifierD* diode);


/** Update DiodeRectifier threshold voltage
 *
 * @details Update the diode model's threshold voltage
 *
 * @param diode     DiodeRectifier instance to update
 * @param threshold	New diode threshold [0 1]
 * @return			Error code, 0 on success
 */
Error_t
DiodeRectifierSetThreshold(DiodeRectifier* diode, float threshold);

Error_t
DiodeRectifierSetThresholdD(DiodeRectifierD* diode, double threshold);


/** Process a buffer of samples
 * @details Uses a diode rectifier to process input samples.
 *
 * @param diode     The DiodeRectifier instance to use.
 * @param outBuffer	The buffer to write the output to.
 * @param inBuffer	The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return			Error code, 0 on success
 */
Error_t
DiodeRectifierProcess(DiodeRectifier*   diode,
                      float*            out_buffer,
                      const float*      in_buffer,
                      unsigned          n_samples);

Error_t
DiodeRectifierProcessD(DiodeRectifierD* diode,
                       double*          out_buffer,
                       const double*    in_buffer,
                       unsigned         n_samples);


/** Process a single sample
 * @details Uses a diode rectifier model to process an input sample
 *
 * @param diode     The DiodeRectifier instance to use.
 * @param in_sample	The sample to process.
 * @return			A processed sample.
 */
float
DiodeRectifierTick(DiodeRectifier* diode, float in_sample);

double
DiodeRectifierTickD(DiodeRectifierD* diode, double in_sample);

#ifdef __cplusplus
}
#endif


#endif
