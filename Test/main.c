#include <stdio.h>
#include "testBiquad.h"
#include "testFIR.h"
#include "testWindow.h"
#include "FtAudioWavetableGenerator.h"
int main (int argc, const char * argv[]) {
    float* destination[131];
    generateSawTable(destination);
    
    
    // insert code here...
    runBiquadFilterTests();
    runFIRFilterTests();
    runWindowFunctionTests();
    
    return 0;
}
