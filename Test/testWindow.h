//
//  testWindow.h
//  Test
//
//  Created by Hamilton Kibbe on 5/14/13.
//
//

#ifndef TESTWINDOW_H
#define TESTWINDOW_H

unsigned
runWindowFunctionTests();

unsigned
testBlackman();

unsigned
testKaiser();

// >blackman(10)
static const float matlabBlackman[10] =
{
    -1.38777878e-17,
    5.08696327e-02,
    2.58000502e-01,
    6.30000000e-01,
    9.51129866e-01,
    9.51129866e-01,
    6.30000000e-01,
    2.58000502e-01,
    5.08696327e-02,
    -1.38777878e-17
};

// >kaiser(10,0.5 * pi)
static const float matlabKaiser[10] =
{
    0.58181688,
    0.73247767,
    0.8576826 ,
    0.94732134,
    0.994065,
    0.994065,
    0.94732134,
    0.8576826 ,
    0.73247767,
    0.58181688
};

#endif
