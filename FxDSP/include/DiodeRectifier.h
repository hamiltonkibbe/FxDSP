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



typedef struct Diode Diode;
typedef struct DiodeD DiodeD;

typedef enum _bias_t
{
    FORWARD_BIAS,
    REVERSE_BIAS
}bias_t;


/** Create a new Diode
 *
 * @details Allocates memory and returns an initialized Diode.
 *			Play nice and call DiodeFree when you're done with it.
 *
 * @param threshold         Normalized voltage threshold
 * @param bias              Diode bias, FORWARD_BIAS or REVERSE_BIAS
 *                          Forward-bias will pass positive signals and clamp
 *                          negative signals to 0.
 * @return                  An initialized Diode
 */
Diode*
DiodeInit(bias_t bias, float threshold);

DiodeD*
DiodeInitD(bias_t bias, double threshold);


/** Free memory associated with a Diode
 *
 * @details release all memory allocated by DiodeInit for the
 *			given diode.
 *
 * @param diode     Diode to free
 * @return			Error code, 0 on success
 */
Error_t
DiodeFree(Diode* diode);

Error_t
DiodeFreeD(DiodeD* diode);


/** Update Diode threshold voltage
 *
 * @details Update the diode model's threshold voltage
 *
 * @param diode     Diode to update
 * @param threshold	New diode threshold [0 1]
 * @return			Error code, 0 on success
 */
Error_t
DiodeSetThreshold(Diode* diode, float threshold);

Error_t
DiodeSetThresholdD(DiodeD* diode, double threshold);


/** Process a buffer of samples
 * @details Uses a diode clipper model to process input samples
 *
 * @param diode     The Diode to use.
 * @param outBuffer	The buffer to write the output to.
 * @param inBuffer	The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return			Error code, 0 on success
 */
Error_t
DiodeProcess(Diode*         diode,
             float*         out_buffer,
             const float*   in_buffer,
             unsigned       n_samples);

Error_t
DiodeProcessD(DiodeD*       diode,
              double*       out_buffer,
              const double* in_buffer,
              unsigned      n_samples);

    
/** Process a single sample
 * @details Uses a diode clipper model to process an input sample
 *
 * @param diode     The Diode to use.
 * @param in_sample	The sample to process.
 * @return			A processed sample.
 */
float
DiodeTick(Diode* diode, float in_sample);
    
double
DiodeTickD(DiodeD* diode, double in_sample);
    
#ifdef __cplusplus
}
#endif


#endif
