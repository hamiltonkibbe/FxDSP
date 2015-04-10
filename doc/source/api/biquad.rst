:mod:`BiquadFilter.h` --- Biquad Filters
=========================================

.. module:: BiquadFilter.h
    :synopsis: Digital Biquad Filter

The BiquadFilter module provides a biquad filter implementation. A biquad
filter is a second-order linear Infinite Impulse Response (IIR) filter with
two poles and two zeros.  Biquad filters are often cascaded together and used
in place of individual higher-order filters because they are much less
sensitive to quantization of their coefficients.

The basic biquad filter implementation is known as Direct-Form I (or `DF-I
<http://ccrma.stanford.edu/~jos/fp/Direct_Form_I.html>`_):

.. math::

    y_{[n]} = b_0x_{[n]} + b_1x_{[n - 1]} + b_2x_{[n - 2]} - a_1y_{[n - 1]} - a_2y_{[n - 2]}

FxDSP uses the canonical, or Direct-Form II (`DF-II
<http://ccrma.stanford.edu/~jos/fp/Direct_Form_II.html>`_) implementation:

.. math::
    w_{[n]} = x_{[n]} - a_1w_{[n - 1]} - a_2w_{[n - 2]}
.. math::
    y_{[n]} = b_0w_{[n]} + b_1w_{[n - 1]} + b_2w_{[n - 2]}

which uses fewer multiplies, adds and delays to implement an identical filter.



Initialization and Deletion
---------------------------

.. doxygenfunction:: BiquadFilterInit
    :project: FxDSP

.. doxygenfunction:: BiquadFilterFree
    :project: FxDSP
    
Resetting
---------

.. doxygenfunction:: BiquadFilterFlush
    :project: FxDSP


Setting Parameters
------------------

.. doxygenfunction:: BiquadFilterUpdateKernel
    :project: FxDSP
    

Processing Audio
----------------

.. doxygenfunction:: BiquadFilterProcess
    :project: FxDSP

.. doxygenfunction:: BiquadFilterTick
    :project: FxDSP