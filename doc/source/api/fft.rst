:mod:`FFT.h` --- Fast Fourier Transforms
========================================

The FFT module provides a unified FFT interface for several popular FFT
libraries. It also provides a fallback implementation if none of the supported
backends are available.

Supported Backends are
  - `FFTW3 <http://www.fftw.org/>`_
  - `Apple Accelerate <http://developer.apple.com/library/prerelease/ios/documentation/Accelerate/Reference/AccelerateFWRef/index.html>`_

If none of the supported backends are available, the FFT module will use an
implementation based on Takuya Ooura's `FFT library <http://www.kurims.kyoto-u.ac.jp/~ooura/fft.html>`_.


Real-To-Complex Forward FFT
---------------------------
.. doxygenfunction:: FFT_R2C
    :project: FxDSP

.. doxygenfunction:: FFT_R2CD
    :project: FxDSP
    
Complex-To-Real Inverse FFT
---------------------------
.. doxygenfunction:: IFFT_C2R
    :project: FxDSP

.. doxygenfunction:: IFFT_C2RD
    :project: FxDSP


FFT Convolution
---------------
Convolution of two real signals using the FFT.

.. doxygenfunction:: FFTConvolve
    :project: FxDSP
    
.. doxygenfunction:: FFTConvolveD
    :project: FxDSP
    
FFT Convolution With Pre-Transformed kernel
-------------------------------------------
Convolve a signal using a pre-transformed kernel. This is useful when using FFT
convolution for filtering, as FFT(filter_kernel) only needs to be calculated
once.

.. doxygenfunction:: FFTFilterConvolve
    :project: FxDSP
    
.. doxygenfunction:: FFTFilterConvolveD
    :project: FxDSP