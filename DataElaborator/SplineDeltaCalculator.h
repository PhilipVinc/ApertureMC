//
//  SplineDeltaCalculator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__SplineDeltaCalculator__
#define __DataElaborator__SplineDeltaCalculator__

#include <iostream>
#include "DeltaCalculator.h"

class SplineDeltaCalculator : public DeltaCalculator
{
public:
    SplineDeltaCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int yMinIndex);
    SplineDeltaCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int yMinIndex, long double step);
    SplineDeltaCalculator(DataSet * expData, DataSet * simData, long double xMin, long double xMax, long double step);
    ~SplineDeltaCalculator();
    
protected:
    long double ElaborateDelta();
    
    void CalculateStep();
    long double step;
    bool stepSet = false;
};


#endif /* defined(__DataElaborator__SplineDeltaCalculator__) */
