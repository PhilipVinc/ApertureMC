//
//  DeltaCalculator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__DeltaCalculator__
#define __DataElaborator__DeltaCalculator__

#include <iostream>
#include <cmath>
#include "DataSet.h"

class DeltaCalculator
{
public:
    DeltaCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int xMaxIndex);
    DeltaCalculator(DataSet * expData, DataSet * simData, long double xMin, long double xMax);
    virtual ~DeltaCalculator();
    
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

#endif /* defined(__DataElaborator__DeltaCalculator__) */
