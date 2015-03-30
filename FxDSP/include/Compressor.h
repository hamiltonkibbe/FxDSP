/*
 *  Compressor.h
 *
 *  Created by Hamilton Kibbe on 5/10/13.
 *  Copyright 2014 HTK Audio. All rights reserved.
 *
 *
 *
 */


#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    
/** Opaque Compressor structure */
typedef struct Compressor Compressor;
    
/** Compressor types */
typedef enum _Compressor_t
{
    /** Peak Compressor*/
    PEAK_COMP,
    
    /** RMS Compressor */
    RMS_COMP,
    
    /** 1176 Compressor */
    UA1176_COMP,
    
    /** Opto Compressor */
    OPTO_COMP
    
} Compressor_t;


Compressor*
CompressorInit(Compressor_t type,
               float        threshold,
               float        ratio,
               float        attack,
               float        release,
               float        gain,
               float        sample_rate);
    
Error_t
CompressorFree(Compressor* compressor);

Error_t
CompressorSetType(Compressor* compressor, Compressor_t type);
    
    
Error_t
CompressorSetThreshold(Compressor* compressor, float threshold);

    
Error_t
CompressorSetRatio(Compressor* compressor, float ratio);
    
    
Error_t
CompressorSetAttack(Compressor* compressor, float attack);
    

Error_t
CompressorSetRelease(Compressor* compressor, float release);
   
    
Error_t
CompressorSetGain(Compressor* compressor, float gain);
    
    
Error_t
CompressorUpdate(Compressor*    compressor,
                 float          threshold,
                 float          ratio,
                 float          attack,
                 float          release,
                 float          gain);
    

Error_t
CompressorProcess(Compressor*   compressor,
                  float*        outBuffer,
                  const float*  inBuffer,
                  unsigned      n_samples);

float
CompressorTick(Compressor* compressor, float sample);
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif
