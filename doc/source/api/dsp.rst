:mod:`Dsp.h` --- DSP Utilities
==============================

The DSP module contains basic functions for working with buffers of samples.

Sample Type Conversion
----------------------

Floating Point to Integer Conversion
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. doxygenfunction:: FloatBufferToInt16
    :project: FxDSP
    
 

Integer to Floating Point Conversion
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. doxygenfunction:: Int16BufferToFloat
    :project: FxDSP
    


Basic Buffer Operations
-----------------------

.. doxygenfunction:: FillBuffer
    :project: FxDSP
    
.. doxygenfunction:: ClearBuffer
    :project: FxDSP
    
.. doxygenfunction:: CopyBuffer
    :project: FxDSP


Vector Absolute Value
---------------------

.. doxygenfunction:: VectorAbs
    :project: FxDSP
    
Vector Negation
---------------

.. doxygenfunction:: VectorNegate
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
    
    