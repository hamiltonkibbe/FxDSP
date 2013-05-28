#include <stdio.h>
#include "testBiquad.h"
#include "testFIR.h"
#include "testWindow.h"
#include "FtAudioWindowFunction.h"
#include "FtAudioFFT.h"

int main (int argc, const char * argv[]) {
/*
    float window[10];
    flat_top(10,window);
    for (unsigned i = 0; i < 10; ++i)
    {
        printf("%f\n", window[i]);
    }
*/
    FtAudioFFTConfig* fft = FtAudioFFTInit(64);
    
    float in[64];
    float mag[32];
    float phase[32];
    for (unsigned i = 0; i < 64; ++i)
    {
        in[i] = sinf(((4*M_PI)/64) * i) + 0.5 * sinf(((16*M_PI)/64) * i);
    }

    FtAudioFFTForward(fft, in, mag, phase);
    for (unsigned i = 0; i < 32; ++i)
    {
        printf("%f\n", mag[i]);
    }
    // insert code here...
    runBiquadFilterTests();
    runFIRFilterTests();
    runWindowFunctionTests();
    
    return 0;
}
