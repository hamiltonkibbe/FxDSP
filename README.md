FutureTechAudio Library
=============================

A Fast DSP library in C. Currently optimized for iOS/OSX using the accelerate 
framework but it provides non-vectorized implementations for use on any 
platform. 

Although the library is written in C it is quasi-OO, so calling from or writing 
wrappers for an object oriented language would be trivial.

All of the "objects" have a similar interface, and they look something like this:

```C
	// Create an Object (this isn't a real thing) that has a a cutoff frequency, a sample 
	// rate, a Q and a Gain (maybe its some sort of filter or something)
	FTA_Object * an_object = FTA_ObjectInit(cutoff, sampleRate, Q, Gain);
	
	// ...Later, in your render_callback, update the parameters and process the data
	FTA_ObjectSetParams(an_object, cutoff, sampleRate, Q, Gain);	
	FTA_ObjectProcess(an_object, dest_pointer, source_pointer, number_of_samples);
	
	// And when you're done...
	FTA_ObjectFree(an_object);
```
    
FTA_BiquadFilter
-------------------
Digital Biquad filter. A building block for more complex filters. Uses a vectorized
implementation if the Accelerate framework is available, otherwise uses a stil-pretty-
efficient DF-II implementation


FTA_FIRFilter
----------------
FIR Filter for processing audio with arbitrary FIR filters. Under the hood, the filter 
uses vectorized  convolution for short kernels, and FFT convolution for fast processing 
with larger kernels.


FTA_RBJFilter
----------------
Biquad EQ stages, implements shelves, low/high-pass, bandpass, and notch filters
with adjustable cutoff and Q. Each stage is a single biquad section.
Ex.
```C
	FTA_RBJFilter *filter = FTA_RBJFilterInit(LOW_SHELF, 100, 44100);
```
creates a low shelf filter with a cutoff at 100Hz, which you would use to 
process a 2048-sample audio buffer like:
```C
	FTA_RBJFilterProcess(filter, destP, sourceP, 2048);
```
Chain a few of these stages together to create a parametric EQ.
 

FTA_OnePoleFilter
-----------------
A single pole filter, good for anywhere you need a quick and easy way to roll off some 
lows/highs, such as for band-limiting control parameter changes. They can also be used to
generate envelope attack/decay/release curves if you use it to filter an envelope defined 
as a bunch of step changes.


FTA_MultibandFilter
-----------------
A Multiband filter bank. Splits the signal into 3 bands (low, mid, and high), that mix 
back flat. The split points can be set anywhere.


FTA_FFT
----------
Fast Fourier Transforms. Allows for processing audio in the frequency domain. The FFT 
functions abstract away most of the setup, real/complex packing, etc. that you've probably
come to expect from an FFT library so you can focus on making cool 
plugins/visualizers/etc. without having to go digging through your textbooks to refresh 
your memory on twiddle factors.


FTA_WindowFunction
---------------------
Window functions. Blackman, Kaiser, Hamming, Hann and many more. Used for removing  edge 
effects from a sliding window when doing frequency-domain analysis, eg. for a spectrum 
analyzer.


FTA_LadderFilter
-------------------
Digital implementation of a MOOG Ladder filter with adjustable cutoff and 
resonance. Because this is a model of the analog circuitry, you can even
play with the temperature of the filter.


Demos
-------
The EQDemo project builds a simple EQ Audio unit made up of a lowpass and highpass filter 
with adjustable cutoff and Q.




