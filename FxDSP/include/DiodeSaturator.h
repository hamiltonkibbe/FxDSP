/**
 * @file        DiodeSaturator.h
 * @author      Hamilton Kibbe <ham@hamiltonkib.be>
 * @copyright   2015 Hamilton Kibbe. All rights reserved.
 * @brief       A diode saturator VA model
 *
 */


#ifndef DIODESATURATOR_H_
#define DIODESATURATOR_H_

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct DiodeSaturator DiodeSaturator;
typedef struct DiodeSaturatorD DiodeSaturatorD;

typedef enum _bias_t
{
    FORWARD_BIAS,
    REVERSE_BIAS
}bias_t;


/** Create a new DiodeSaturator
 *
 * @details Allocates memory and returns an initialized DiodeSaturator.
 *          call DiodeSaturatorFree to release allocated memory
 *
 * @param amount    Clipping amount
 * @param bias      Diode bias, FORWARD_BIAS or REVERSE_BIAS
 *                  Forward-bias will clip positive signals and leave negative
 *                  signals untouched.
 * @return          An initialized DiodeSaturator
 */
DiodeSaturator*
DiodeSaturatorInit(bias_t bias, float amount);

DiodeSaturatorD*
DiodeSaturatorInitD(bias_t bias, double amount);


/** Free memory associated with a DiodeSaturator
 *
 * @details release all memory allocated by DiodeSaturatorInit for the
 *          given diode.
 *
 * @param diode     DiodeSaturator to free
 * @return          Error code, 0 on success
 */
Error_t
DiodeSaturatorFree(DiodeSaturator* saturator);

Error_t
DiodeSaturatorFreeD(DiodeSaturatorD* saturator);


/** Update DiodeSaturator clipping amount
 *
 * @details Update the diode model's clipping amount
 *
 * @param diode     DiodeSaturator to update
 * @param amount    New diode clipping amount [0 1]
 * @return          Error code, 0 on success
 */
Error_t
DiodeSaturatorSetAmount(DiodeSaturator* saturator, float amount);

Error_t
DiodeSaturatorSetAmountD(DiodeSaturatorD* saturator, double amount);


/** Process a buffer of samples
 * @details Uses a diode saturator model to process input samples
 *
 * @param diode     The DiodeSaturator to use.
 * @param outBuffer The buffer to write the output to.
 * @param inBuffer  The buffer to filter.
 * @param n_samples The number of samples to filter.
 * @return          Error code, 0 on success
 */
Error_t
DiodeSaturatorProcess(DiodeSaturator*   saturator,
                      float*            out_buffer,
                      const float*      in_buffer,
                      unsigned          n_samples);

Error_t
DiodeSaturatorProcessD(DiodeSaturatorD* saturator,
                       double*          out_buffer,
                       const double*    in_buffer,
                       unsigned         n_samples);


/** Process a single sample
 * @details Uses a diode saturator model to process an input sample
 *
 * @param diode     The DiodeSaturator to use.
 * @param in_sample The sample to process.
 * @return          A processed sample.
 */
float
DiodeSaturatorTick(DiodeSaturator* saturator, float in_sample);

double
DiodeSaturatorTickD(DiodeSaturatorD* saturator, double in_sample);

#ifdef __cplusplus
}
#endif


#endif
