//
//  LinkwitzRileyFilter.c
//  FxDSP
//
//  Created by Hamilton Kibbe on 11/24/13.
//  Copyright (c) 2013 Hamilton Kibbe. All rights reserved.
//

#include "LinkwitzRileyFilter.h"
#include "RBJFilter.h"
#include <stdlib.h>

/* LRFilter ***************************************************************/
struct LRFilter
{
    RBJFilter*  filterA;
    RBJFilter*  filterB;
    Filter_t    type;
    float       cutoff;
    float       Q;
    float       sampleRate;
};

struct LRFilterD
{
    RBJFilterD* filterA;
    RBJFilterD* filterB;
    Filter_t    type;
    double      cutoff;
    double      Q;
    double      sampleRate;
};

/* LRFilterInit ***********************************************************/
LRFilter*
LRFilterInit(Filter_t   type,
             float 		cutoff,
             float      Q,
             float      sampleRate)
{
    LRFilter *filter = (LRFilter*) malloc(sizeof(LRFilter));
    filter->type = type;
    filter->cutoff = cutoff;
    filter->Q = Q;
    filter->sampleRate = sampleRate;
    filter->filterA = RBJFilterInit(filter->type, filter->cutoff, filter->sampleRate);
    filter->filterB = RBJFilterInit(filter->type, filter->cutoff, filter->sampleRate);
    RBJFilterSetQ(filter->filterA, filter->Q);
    RBJFilterSetQ(filter->filterB, filter->Q);
    return filter;
}

LRFilterD*
LRFilterInitD(Filter_t  type,
              double 	cutoff,
              double    Q,
              double    sampleRate)
{
    LRFilterD *filter = (LRFilterD*) malloc(sizeof(LRFilterD));
    filter->type = type;
    filter->cutoff = cutoff;
    filter->Q = Q;
    filter->sampleRate = sampleRate;
    filter->filterA = RBJFilterInitD(filter->type, filter->cutoff, filter->sampleRate);
    filter->filterB = RBJFilterInitD(filter->type, filter->cutoff, filter->sampleRate);
    RBJFilterSetQD(filter->filterA, filter->Q);
    RBJFilterSetQD(filter->filterB, filter->Q);
    return filter;
}

/* LRFilterFree ***********************************************************/
Error_t
LRFilterFree(LRFilter* 	filter)
{
    RBJFilterFree(filter->filterA);
    RBJFilterFree(filter->filterB);
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    
    return NOERR;
}

Error_t
LRFilterFreeD(LRFilterD* filter)
{
    RBJFilterFreeD(filter->filterA);
    RBJFilterFreeD(filter->filterB);
    if (filter)
    {
        free(filter);
        filter = NULL;
    }
    
    return NOERR;
}

/* LRFilterFlush **********************************************************/
Error_t
LRFilterFlush(LRFilter* filter)
{
    RBJFilterFlush(filter->filterA);
    RBJFilterFlush(filter->filterB);
    
    return NOERR;
}

Error_t
LRFilterFlushD(LRFilterD* filter)
{
    RBJFilterFlushD(filter->filterA);
    RBJFilterFlushD(filter->filterB);
    
    return NOERR;
}

/* LRFilterSetParams ******************************************************/
Error_t
LRFilterSetParams(LRFilter* filter,
                      Filter_t  type,
                      float     cutoff,
                      float     Q)
{
    filter->type = type;
    filter->cutoff = cutoff;
    filter->Q = Q;
    RBJFilterSetParams(filter->filterA, filter->type, filter->cutoff, filter->Q);
    RBJFilterSetParams(filter->filterB, filter->type, filter->cutoff, filter->Q);
    
    return NOERR;
}

Error_t
LRFilterSetParamsD(LRFilterD*   filter,
                  Filter_t      type,
                  double        cutoff,
                  double        Q)
{
    filter->type = type;
    filter->cutoff = cutoff;
    filter->Q = Q;
    RBJFilterSetParamsD(filter->filterA, filter->type, filter->cutoff, filter->Q);
    RBJFilterSetParamsD(filter->filterB, filter->type, filter->cutoff, filter->Q);
    
    return NOERR;
}

/* LRFilterProcess ********************************************************/
Error_t
LRFilterProcess(LRFilter*       filter,
                float*          outBuffer,
                const float* 	inBuffer,
                unsigned 		n_samples)

{
    float tempBuffer[n_samples];
    RBJFilterProcess(filter->filterA, tempBuffer, inBuffer, n_samples);
    RBJFilterProcess(filter->filterB, outBuffer, tempBuffer, n_samples);
    return NOERR;
}

Error_t
LRFilterProcessD(LRFilterD*     filter,
                 double*        outBuffer,
                 const double*  inBuffer,
                 unsigned 		n_samples)

{
    double tempBuffer[n_samples];
    RBJFilterProcessD(filter->filterA, tempBuffer, inBuffer, n_samples);
    RBJFilterProcessD(filter->filterB, outBuffer, tempBuffer, n_samples);
    return NOERR;
}


