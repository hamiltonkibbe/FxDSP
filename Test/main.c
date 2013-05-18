#include <stdio.h>
#include "testBiquad.h"
#include "testFIR.h"
#include "testWindow.h"
#include "FtAudioWavetable.h"
int main (int argc, const char * argv[]) {
    
    FtAudioWavetable* table = FtAudioWavetableInit(SQUARE, 48000);
    FtAudioWavetableFree(table);
    
    // insert code here...
    runBiquadFilterTests();
    runFIRFilterTests();
    runWindowFunctionTests();
    
    return 0;
}
