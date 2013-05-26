#include <stdio.h>
#include "testBiquad.h"
#include "testFIR.h"
#include "testWindow.h"
#include "FtAudioWindowFunction.h"

int main (int argc, const char * argv[]) {
   /*
    float window[10];
    gaussian(9, 0.4, window);
    for (unsigned i = 0; i < 10; ++i)
    {
        printf("%f\n", window[i]);
    }
    */
    // insert code here...
    runBiquadFilterTests();
    runFIRFilterTests();
    runWindowFunctionTests();
    
    return 0;
}
