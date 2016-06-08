About FxDSP
===========

FxDSP is a DSP library written in C for audio processing and synthesis.  The
library supports both single- and double-precision calculations, and uses
vectorized instructions where possible.

FxDSP contains a set of objects which are building blocks for audio processing
and synthesis applications.


Installation
------------
FxDSP cam be built and installed on most platforms using `CMake
<http://www.cmake.org>`_.

.. code-block:: bash

    # From the top-level FxDSP directory
    mkdir build
    cd build
    cmake ../FxDSP
    make && make install

On **Mac OSX** FxDSP can be built and installed using the included XCode
project.

Useage
------
FxDSP is written in an object-oriented style.  Each module has an object
type and a set of functions that act on that object. Here's an example:

.. code-block:: c

    #include "FxDSP/CircularBuffer.h"

    // Pointers to input and output buffers
    float* in;
    float* out;

    unsigned buffer_length = 1024;

    // Create an instance of a CircularBuffer "object"
    CircularBuffer* buffer = CircularBufferInit(buffer_length);

    // Write 128 samples to the circular buffer
    CircularBufferWrite(buffer, in, 128);

    // Read 64 samples from the circular buffer
    CircularBufferRead(buffer, out, 64);

    // Delete the circular buffer
    CircularBufferFree(buffer);

The double-precision version is nearly identical, with a "D" added to the end
of each object type and function name:

.. code-block:: c

    #include "FxDSP/CircularBuffer.h"

    // Pointers to input and output buffers
    double* in;
    double* out;

    unsigned buffer_length = 1024;

    // Create an instance of a CircularBuffer "object"
    CircularBufferD* buffer = CircularBufferInitD(buffer_length);

    // Write 128 samples to the circular buffer
    CircularBufferWriteD(buffer, in, 128);

    // Read 64 samples from the circular buffer
    CircularBufferReadD(buffer, out, 64);

    // Delete the circular buffer
    CircularBufferFreeD(buffer);

