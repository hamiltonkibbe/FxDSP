/**
 * @file        Tape.h
 * @author      Hamilton Kibbe <ham@hamiltonkib.be>
 * @copyright   2015 Hamilton Kibbe. All rights reserved.
 * @brief       Magnetic Tape Effect
 *
 */


#ifndef TAPE_H_
#define TAPE_H_

#include "Error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum  TapeSpeed
{
    TS_3_75IPS,
    TS_7_5IPS,
    TS_15IPS,
    TS_30IPS
}TapeSpeed;

typedef struct Tape Tape;


/** Create a new Tape
 */
Tape*
TapeInit(TapeSpeed speed, float saturation, float hysteresis, float flutter, float sample_rate);



/** Free memory associated with a Tape
 *
 * @details release all memory allocated by TapeInit for the
 *			given saturator.
 *
 * @param saturator Tape to free
 * @return			Error code, 0 on success
 */
Error_t
TapeFree(Tape* tape);

Error_t
TapeSetSpeed(Tape* tape, TapeSpeed speed);

Error_t
TapeSetSaturation(Tape* tape, float saturation);

Error_t
TapeSetHysteresis(Tape* tape, float hysteresis);

Error_t
TapeSetFlutter(Tape* tape, float flutter);

float
TapeGetSaturation(Tape* tape);

float
TapeGetHysteresis(Tape* tape);

Error_t
TapeProcess(Tape*           tape,
            float*          out_buffer,
            const float*    in_buffer,
            unsigned        n_samples);



/** Process a single sample
 * @details Uses a Tape model to process an input sample
 *
 * @param taoe      The Tape to use.
 * @param in_sample	The sample to process.
 * @return			A processed sample.
 */
float
TapeTick(Tape* tape, float in_sample);


#ifdef __cplusplus
}
#endif


#endif
