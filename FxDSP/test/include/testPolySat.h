//
//  testPolySat.h
//  FxDSP
//
//  Created by Hamilton Kibbe on 3/19/15.
//  Copyright (c) 2015 Hamilton Kibbe. All rights reserved.
//

#ifndef FxDSP_testPolySat_h
#define FxDSP_testPolySat_h

#define TEST_VECTOR_LENGTH (9)

static const float test_vector[] = {
    -1.0,
    -0.75,
    -0.5,
    -0.25,
    0,
    0.25,
    0.5,
    0.75,
    1.0
};

static const float n_vector[] = {
    1,
    2,
    5,
    16,
};


static const float result_1[] = {
    -1.000000,
    -0.937500,
    -0.750000,
    -0.437500,
    0.000000,
    0.437500,
    0.750000,
    0.937500,
    1.000000,
};


static const float result_2[] = {
    -1.0,
    -0.9140625,
    -0.6875,
    -0.3671875,
    0,
    0.3671875,
    0.6875,
    0.9140625,
    1.0
};

static const float result_5[] = {
    -1.000000,
    -0.864404,
    -0.596875,
    -0.299951,
    0.000000,
    0.299951,
    0.596875,
    0.864404,
    1.000000,
};

static const float result_16[] = {
    -1.000000,
    -0.796405,
    -0.531250,
    -0.265625,
    0.000000,
    0.265625,
    0.531250,
    0.796405,
    1.000000,
};

static const float* results[] = {
    result_1,
    result_2,
    result_5,
    result_16
};

#endif
