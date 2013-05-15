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


#endif
