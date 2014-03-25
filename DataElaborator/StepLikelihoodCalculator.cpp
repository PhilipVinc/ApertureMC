//
//  StepDeltaCalculator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "StepLikelihoodCalculator.h"

StepLikelihoodCalculator::StepLikelihoodCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int xMaxIndex) : LikelihoodCalculator(expData, simData, xMinIndex, xMaxIndex)
{
    
}

StepLikelihoodCalculator::StepLikelihoodCalculator(DataSet * expData, DataSet * simData, long double xMin, long double xMax) : LikelihoodCalculator(expData, simData, xMin, xMax)
{
    
}

StepLikelihoodCalculator::~StepLikelihoodCalculator()
{
    
}

long double StepLikelihoodCalculator::ElaborateDelta()
{    
    int simIndex  = 0;
    double diff = 0.0;
    double error = 0.0;
    for (int i = xMinExpIndex; i != xMaxExpIndex; i++)
    {
        simIndex = i - xMinExpIndex + xMinSimIndex;
        long double a1= simulatedData->y(simIndex);
        long double a2 = experimentalData->y(i);
        diff = fabs(a1-a2);
        error += diff*diff;
    }
    error /= xMaxIndex-xMinIndex;
    error *= 100;
    return error;
}
