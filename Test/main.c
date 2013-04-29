#include <stdio.h>
//#include "testFIR.h"
#include <Accelerate/Accelerate.h>
int main (int argc, const char * argv[]) {
    // insert code here...
	test_fir();
	/*
	unsigned filterLength = 4;
	unsigned inputLength = 15;
	unsigned resultLength = inputLength + (filterLength - 1);
	unsigned lenSignal = filterLength + resultLength;
	
	float taps[4] = {1.0,2.0,3.0,4.0};
	float input[inputLength];
	float output[resultLength];
	float paddedSignal[lenSignal];
	
	for (unsigned i = 0; i < inputLength; ++i)
	{
		input[i] = 1.0;
	}
	
	for (unsigned i=0; i < lenSignal; ++i)
	{
		paddedSignal[i] = 0.0;
	}
	
	for(unsigned i=0; i < resultLength; ++i)
	{
		output[i] = 0.0;
		if (i>=(filterLength - 1))
			paddedSignal[i] = input[i-(filterLength - 1)];
	}
	
	vDSP_conv(paddedSignal, 1, taps+(filterLength - 1), -1, output, 1, resultLength, filterLength);
	
	for (unsigned i = 0; i < resultLength; ++i)
	{
		printf("%1.2f\n", output[i]);
	}
	
	
	*/
	
    return 0;
}
