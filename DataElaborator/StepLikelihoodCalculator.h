//
//  StepLikelihoodCalculator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__StepLikelihoodCalculator__
#define __DataElaborator__StepLikelihoodCalculator__

#include <iostream>
#include "LikelihoodCalculator.h"

class StepLikelihoodCalculator : public LikelihoodCalculator
{
public:
    StepLikelihoodCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int yMinIndex);
    StepLikelihoodCalculator(DataSet * expData, DataSet * simData, long double xMin, long double xMax);
    ~StepLikelihoodCalculator();
    
protected:
    long double ElaborateDelta();

};

#endif /* defined(__DataElaborator__StepLikelihoodCalculator__) */
