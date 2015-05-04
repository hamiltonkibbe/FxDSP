*************
FxDSP Library
*************

.. image:: https://travis-ci.org/hamiltonkibbe/FxDSP.svg?branch=master
    :target: https://travis-ci.org/hamiltonkibbe/FxDSP
    :alt: Build Status
.. image:: https://readthedocs.org/projects/fxdsp/badge/?version=latest
    :target: https://readthedocs.org/projects/fxdsp/?badge=latest
    :alt: Documentation Status
.. image:: https://coveralls.io/repos/hamiltonkibbe/FxDSP/badge.svg?branch=master 
    :target: https://coveralls.io/r/hamiltonkibbe/FxDSP?branch=master
    :alt: Coverage Status
A Fast DSP library for effect and synthesizer development in C. Optimized for 
iOS/OSX using the Accelerate framework but should run using non-vectorized 
implementations for use on any platform. 

============
Installation
============
FxDSP can be built and installed on most platforms using CMake

.. code-block:: bash
    
    mkdir build
    cd build
    cmake ../FxDSP
    make && make install

On **Mac OS X**, FxDSP can be built and installed using the included Xcode Project

==============
Useage Example
==============

Create a low-shelf filter and use it to process some samples:

.. code-block:: c
    
    #include "FxDSP/RBJFilter.h"    

    unsigned num_samples;

    // Single Precision example ------------------------------------------
    float* input_samples;
    float* output_samples;

    // Initialize the Filter
    RBJFilter *filter = RBJFilterInit(LOW_SHELF, 44100, 100);

    // Process some data
    RBJFilterProcess(filter, output_samples, input_samples, num_samples);

    // Cleanup
    RBJFilterFree(filter);
    
    // Double Precision example ------------------------------------------
    double* input_samplesD;
    double* output_samplesD;

    // Initialize the Filter
    RBJFilterD *filter = RBJFilterInitD(LOW_SHELF, 44100, 100);

    // Process some data
    RBJFilterProcessD(filter, output_samplesD, input_samplesD, num_samples);

    // Cleanup
    RBJFilterFreeD(filter);

=======
Modules
=======

CircularBuffer
--------------
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
with adjustable cutoff and Q. Each stage is a single biquad section. Chain a few 
of these stages together to create a parametric EQ.
 

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
















