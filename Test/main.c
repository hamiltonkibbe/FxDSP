#include <stdio.h>
#include "testBiquad.h"
#include "testFIR.h"
#include "testWindow.h"
#include "FtAudioWindowFunction.h"

int main (int argc, const char * argv[]) {
    
    
    // insert code here...
    runBiquadFilterTests();
    runFIRFilterTests();
    runWindowFunctionTests();
    
    return 0;
}
