FxDSP Library
=============
.. image:: https://travis-ci.org/hamiltonkibbe/FxDSP.svg?branch=master
    :target: https://travis-ci.org/hamiltonkibbe/FxDSP
A Fast DSP library for effect and synthesizer development in C. Optimized for 
iOS/OSX using the Accelerate framework but should run using non-vectorized 
implementations for use on any platform. 

    
CircularBuffer
------------------
A Circular buffer with fast wrapping. 


BiquadFilter
-------------------
Digital Biquad filter. A building block for more complex filters. Uses a vectorized
implementation if the Accelerate framework is available, otherwise uses a stil-pretty-
efficient DF-II implementation


FIRFilter
---------
FIR Filter for processing audio with arbitrary FIR filters. Under the hood, the filter 
uses vectorized  convolution for short kernels, and FFT convolution for fast processing 
with larger kernels.


RBJFilter
----------------
Biquad EQ stages, implements shelves, low/high-pass, bandpass, and notch filters
with adjustable cutoff and Q. Each stage is a single biquad section.
Ex.
```C
	RBJFilter *filter = RBJFilterInit(LOW_SHELF, 100, 44100);
```
creates a low shelf filter with a cutoff at 100Hz, which you would use to 
process a 2048-sample audio buffer like:
```C
	RBJFilterProcess(filter, destP, sourceP, 2048);
```
Chain a few of these stages together to create a parametric EQ.
 

OnePoleFilter
-----------------
A single pole filter, good for anywhere you need a quick and easy way to roll off some 
lows/highs, such as for band-limiting control parameter changes. They can also be used to
generate envelope attack/decay/release curves if you use it to filter an envelope defined 
as a bunch of step changes.


MultibandFilter
-----------------
A Multiband filter bank. Splits the signal into 3 bands (low, mid, and high), that mix 
back flat. The split points can be set anywhere.


FFT
----------
Fast Fourier Transforms. Allows for processing audio in the frequency domain. The FFT 
functions abstract away most of the setup, real/complex packing, etc. that you've probably
come to expect from an FFT library so you can focus on making cool 
plugins/visualizers/etc. without having to go digging through your textbooks to refresh 
your memory on twiddle factors.


WindowFunction
---------------------
Window functions. Blackman, Kaiser, Hamming, Hann and many more. Used for removing  edge 
effects from a sliding window when doing frequency-domain analysis, eg. for a spectrum 
analyzer.


LadderFilter
-------------------
Digital implementation of a MOOG Ladder filter with adjustable cutoff and 
resonance. 






