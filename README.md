FutureTechAudio Library
=============================

A Fast DSP library in C. Currently optimized for iOS/OSX but should work on any platform.

FtAudioFIRFilter
----------------
FIR Filter for processing audio with arbitrary FIR filters


FtAudioBiquadFilter
-------------------
Digital Biquad filter


FtAudioRBJFilter
----------------
Biquad EQ stages, implements shelves, low/high-pass, bandpass, and notch filters with adjustable cutoff and Q.
Ex.
	FtAudioRbjFilter *filter = FtAudioRBJFilterInit(RBJ_LSHELF, 100, 44100);

creates a low shelf filter with a cutoff at 100Hz, which you could use to process a 2048-sample audio buffer like:
	FtAudioRBJFilterProcess(filter, output, input, 2048);
 

FtAudioWindowFunction
---------------------
Window functions, including Blackman, Kaiser, Hamming, Hann and many more windows


FtAudioLadderFilter
-------------------
Digital implementation of a MOOG Ladder filter.



