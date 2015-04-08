//
//  Compressor.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 4/9/14.
//  Copyright (c) 2014 Hamilton Kibbe. All rights reserved.
//


#include "Compressor.h"
#include "OnePole.h"
#include "RMSEstimator.h"
#include "Optocoupler.h"
#include "Utilities.h"

#include <stdlib.h>


/* Compressor *************************************************************/
struct Compressor
{
    Compressor_t        type;
    float               threshold;
    float               ratio;
    float               attack;
    float               release;
    float               gain;
    float               sample_rate;
    float               attack_cutoff;
    float               release_cutoff;
    float               envelope;
    float               previous;
    float               setpoint;
    unsigned            stat;
    OnePole*  lp;
    RMSEstimator*   rms;
    Opto*           opto;
};



Compressor*
CompressorInit(Compressor_t type,
                   float        threshold,
                   float        ratio,
                   float        attack,
                   float        release,
                   float        gain,
                   float        sample_rate)
{
    // Allocate and create struct
    Compressor* comp = (Compressor*)malloc(sizeof(Compressor));
    
    // Set parameters
    comp->type = type;
    comp->threshold = threshold;
    comp->ratio = ratio;
    comp->gain = gain;
    comp->sample_rate = sample_rate;
    comp->envelope = 0.0;
    comp->previous = 0.0;
    comp->setpoint = 1.0;
    comp->stat = 0;
    
    // Initialize sub-modules
    comp->lp = OnePoleInit(0, sample_rate, LOWPASS);
    comp->rms = RMSEstimatorInit(0.1, sample_rate);
    comp->opto = OptoInit(OPTO_PHOTOTRANSISTOR, 0.5, sample_rate);
    
    // Set attack, release, attack_cutoff, release_cutoff
    CompressorSetAttack(comp, attack);
    CompressorSetRelease(comp, release);

    // Set up gain filter...
    comp->stat = 1; // 1: attack  0: release
    OnePoleSetCutoff(comp->lp, comp->attack_cutoff);
    
    return comp;
}



Error_t
CompressorFree(Compressor* compressor)
{
    if(compressor)
    {
        OnePoleFree(compressor->lp);
        RMSEstimatorFree(compressor->rms);
        OptoFree(compressor->opto);
        free(compressor);
    }
    
    return NOERR;
}



Error_t
CompressorSetType(Compressor*   compressor,
                      Compressor_t      type)
{
    compressor->type = type;
    return NOERR;
}



Error_t
CompressorSetThreshold(Compressor* compressor, float threshold)
{
    compressor->threshold = threshold;
    return NOERR;
}



Error_t
CompressorSetRatio(Compressor* compressor, float ratio)
{
    compressor->ratio = ratio;
    return NOERR;
}



Error_t
CompressorSetAttack(Compressor* compressor, float attack)
{
    compressor->attack = attack;
    compressor->attack_cutoff = 1.0 / attack;
    if (compressor->stat == 1)
    {
        OnePoleSetCutoff(compressor->lp, compressor->attack_cutoff);
    }
    return NOERR;
}


Error_t
CompressorSetRelease(Compressor* compressor, float release)
{
    compressor->release = release;
    compressor->release_cutoff = 1.0 / release;
    if (compressor->stat == 0)
    {
        OnePoleSetCutoff(compressor->lp, compressor->release_cutoff);
    }
    return NOERR;
}


Error_t
CompressorSetGain(Compressor* compressor, float gain)
{
    compressor->gain = gain;
    return NOERR;
}


Error_t
CompressorUpdate(Compressor*    compressor,
                     float              threshold,
                     float              ratio,
                     float              attack,
                     float              release,
                     float              gain)
{
    compressor->threshold = threshold;
    compressor->ratio = ratio;
    compressor->attack = attack;
    compressor->attack_cutoff = 1.0 / attack;
    compressor->release = release;
    compressor->release_cutoff = 1.0 / release;
    compressor->gain = gain;
    float cutoff = compressor->stat == 1 ? compressor->attack_cutoff : compressor->release_cutoff;
    OnePoleSetCutoff(compressor->lp, cutoff);
    
    return NOERR;
}

float
CompressorTick(Compressor* compressor, float sample)
{
    // Update our Envelope
    switch (compressor->type)
    {
        case PEAK_COMP:
            compressor->envelope = f_abs(sample);
            break;
        case RMS_COMP:
            compressor->envelope = RMSEstimatorTick(compressor->rms, sample);
            break;
        case UA1176_COMP:
            break;
        case OPTO_COMP:
            compressor->envelope = OptoTick(compressor->opto, sample);
            break;
    }
    
    // Update Gain setting
    
    
    return sample;
}









