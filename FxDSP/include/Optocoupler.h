/*
 *  Optocoupler.h
 *
 *  Created by Hamilton Kibbe on 5/10/13.
 *  Copyright 2014 HTK Audio. All rights reserved.
 *
 *
 *
 */


#ifndef OPTOCOUPLER_H_
#define OPTOCOUPLER_H_

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Opaque Opto structure */
typedef struct Opto Opto;
typedef struct OptoD OptoD;

    /** Optocoupler types */
typedef enum _Opto_t
{
    /** Light-Dependent-Resistor output. Based
     on Vactrol VTL series. datasheet:
     http://pdf.datasheetcatalog.com/datasheet/perkinelmer/VT500.pdf

     Midpoint Delay values:
       Turn-on delay:   ~10ms
       Turn-off delay:  ~133ms
     */
    OPTO_LDR,

    /** TODO: Add Phototransistor/voltage output opto model*/
    OPTO_PHOTOTRANSISTOR
} Opto_t;


/** Create a new Opto
 *
 * @details Allocates memory and returns an initialized Opto.
 *			Play nice and call OptoFree when you're done with it.
 *
 * @param opto_type         Optocoupler model type.
 * @param delay             Amount of delay in the optocoupler. The halfway
 *                          point is a good approximation of the actual device,
 *                          higher and lower values are based on model
 *                          extrapolation. The effect is most pronounced with
 *                          the LDR(Vactrol) model as the LDR response is slow.
 *                          while this is not a realistic parameter with higher-
 *                          bandwidth models, higher settings of this parameter
 *                          result in an artifically exaggerated effect.
 * @param sample_rate       system sampling rate.
 * @return                  An initialized Opto
 */
Opto*
OptoInit(Opto_t opto_type, float delay, float sample_rate);

OptoD*
OptoInitD(Opto_t opto_type, double delay, double sample_rate);


Error_t
OptoFree(Opto* optocoupler);

Error_t
OptoFreeD(OptoD* optocoupler);



Error_t
OptoSetDelay(Opto* optocoupler, float delay);

Error_t
OptoSetDelayD(OptoD* optocoupler, double delay);



Error_t
OptoProcess(Opto*           optocoupler,
            float*          out_buffer,
            const float*    in_buffer,
            unsigned        n_samples);

Error_t
OptoProcessD(OptoD*         optocoupler,
             double*        out_buffer,
             const double*  in_buffer,
             unsigned       n_samples);

float
OptoTick(Opto* optocoupler, float in_sample);

double
OptoTickD(OptoD* optocoupler, double in_sample);


#ifdef __cplusplus
}
#endif

#endif
