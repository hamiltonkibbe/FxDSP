#include <stdio.h>
#include "testFIR.h"

int main (int argc, const char * argv[]) {
    // insert code here...
    
    for (unsigned i = 0; i < 10000; ++i)
        runFIRFilterTests();
    return 0;
}
