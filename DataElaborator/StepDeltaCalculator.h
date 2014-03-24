//
//  StepDeltaCalculator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__StepDeltaCalculator__
#define __DataElaborator__StepDeltaCalculator__

#include <iostream>
#include "DeltaCalculator.h"

class StepDeltaCalculator : public DeltaCalculator
{
public:
    StepDeltaCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int yMinIndex);
    StepDeltaCalculator(DataSet * expData, DataSet * simData, long double xMin, long double xMax);
    ~StepDeltaCalculator();
    
protected:
    long double ElaborateDelta();

};

#endif /* defined(__DataElaborator__StepDeltaCalculator__) */
