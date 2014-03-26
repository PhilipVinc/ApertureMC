//
//  SplineDeltaCalculator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__SplineLikelihoodCalculator__
#define __DataElaborator__SplineLikelihoodCalculator__

#include "LikelihoodCalculator.h"

class SplineLikelihoodCalculator : public LikelihoodCalculator
{
public:
    SplineLikelihoodCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int yMinIndex);
    SplineLikelihoodCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int yMinIndex, long double step);
    SplineLikelihoodCalculator(DataSet * expData, DataSet * simData, long double xMin, long double xMax, long double step);
    ~SplineLikelihoodCalculator();
    
protected:
    long double ElaborateDelta();
    
    void CalculateStep();
    long double step;
    bool stepSet = false;
};


#endif /* defined(__DataElaborator__SplineLikelihoodCalculator__) */
