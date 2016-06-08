:mod:`Dsp.h` --- DSP Utilities
==============================

The DSP module contains basic functions for working with buffers of samples.

Sample Type Conversion
----------------------

Floating Point to Integer Conversion
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. doxygenfunction:: FloatBufferToInt16
    :project: FxDSP

.. doxygenfunction:: DoubleBufferToInt16
    :project: FxDSP

Integer to Floating Point Conversion
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. doxygenfunction:: Int16BufferToFloat
    :project: FxDSP

.. doxygenfunction:: Int16BufferToDouble
    :project: FxDSP

Single- to Double-Precision Conversion
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. doxygenfunction:: DoubleToFloat
    :project: FxDSP

.. doxygenfunction:: FloatToDouble
    :project: FxDSP


Basic Buffer Operations
-----------------------
.. doxygenfunction:: FillBuffer
    :project: FxDSP

.. doxygenfunction:: ClearBuffer
    :project: FxDSP

.. doxygenfunction:: CopyBuffer
    :project: FxDSP

.. doxygenfunction:: CopyBufferStride
    :project: FxDSP

Vector Min/Max
--------------

Min
^^^
.. doxygenfunction:: VectorMin
    :project: FxDSP

.. doxygenfunction:: VectorMinVI
    :project: FxDSP

Max
^^^
.. doxygenfunction:: VectorMax
    :project: FxDSP

.. doxygenfunction:: VectorMaxVI
    :project: FxDSP


Vector Absolute Value
---------------------
.. doxygenfunction:: VectorAbs
    :project: FxDSP

Vector Negation
---------------

.. doxygenfunction:: VectorNegate
    :project: FxDSP

Vector Summation
----------------
.. doxygenfunction:: VectorSum
    :project: FxDSP

Vector Addition
---------------
.. doxygenfunction:: VectorVectorAdd
    :project: FxDSP

.. doxygenfunction:: VectorScalarAdd
    :project: FxDSP

Vector Multiplication
---------------------
.. doxygenfunction:: VectorVectorMultiply
    :project: FxDSP

.. doxygenfunction:: VectorScalarMultiply
    :project: FxDSP

Vector Mixing
-------------
.. doxygenfunction:: VectorVectorMix
    :project: FxDSP

.. doxygenfunction:: VectorVectorSumScale
    :project: FxDSP

Vector Power
------------
.. doxygenfunction:: VectorPower
    :project: FxDSP

Vector Convolution
------------------
.. doxygenfunction:: Convolve
    :project: FxDSP
