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
    ~DeltaCalculator();
    
    virtual long double GetDelta()=0;
protected:
    DataSet * experimentalData;
    DataSet * simulatedData;
    
    int xMinIndex;
    int xMaxIndex;
};

#endif /* defined(__DataElaborator__DeltaCalculator__) */
