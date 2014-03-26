//
//  LikelihoodCalculator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__LikelihoodCalculator__
#define __DataElaborator__LikelihoodCalculator__

#include <cmath>
#include "DataSet.h"

class LikelihoodCalculator
{
public:
    LikelihoodCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int xMaxIndex);
    LikelihoodCalculator(DataSet * expData, DataSet * simData, long double xMin, long double xMax);
    virtual ~LikelihoodCalculator();
    
    void SetupIndices();
    
    long double GetDelta();
protected:
    virtual long double ElaborateDelta() = 0;
    
    DataSet * experimentalData;
    DataSet * simulatedData;
    
    int xMinIndex;
    int xMaxIndex;
    
    int xMinExpIndex, xMaxExpIndex, xMinSimIndex, xMaxSimIndex;
    
    bool rangeSet = false;
    long double xMin = NAN;
    long double xMax = NAN;
    
    bool resultDirty = true;
    long double result;
};

#endif /* defined(__DataElaborator__LikelihoodCalculator__) */
