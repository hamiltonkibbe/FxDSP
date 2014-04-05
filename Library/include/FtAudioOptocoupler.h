/*
 *  FtAudioOptocoupler.h
 *
 *  Created by Hamilton Kibbe on 5/10/13.
 *  Copyright 2014 HTK Audio. All rights reserved.
 *
 *
 *
 */


#ifndef FTAUDIOOPTOCOUPLER_H_
#define FTAUDIOOPTOCOUPLER_H_

#include "FtAudioError.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Opaque FTA_Opto structure */
typedef struct FTA_Opto FTA_Opto;

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
    OPTO_PHOTOTRANSISTOR,
    
} Opto_t;

    
/** Create a new FTA_Opto
 *
 * @details Allocates memory and returns an initialized FTA_Opto.
 *			Play nice and call FTA_OptoFree when you're done with it.
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
 * @return                  An initialized FTA_Opto
 */
FTA_Opto*
FTA_OptoInit(Opto_t opto_type, float delay, float sample_rate);
  
    
FTA_Error_t
FTA_OptoFree(FTA_Opto* optocoupler);

    
FTA_Error_t
FTA_OptoSetDelay(FTA_Opto* optocoupler, float delay);

    
FTA_Error_t
FTA_OptoProcess(FTA_Opto*       optocoupler,
                float*          out_buffer,
                const float*    in_buffer,
                unsigned        n_samples);

float
FTA_OptoTick(FTA_Opto* optocoupler, float in_sample);

    

#ifdef __cplusplus
}
#endif

#endif
