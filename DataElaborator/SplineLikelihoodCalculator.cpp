//
//  SplineDeltaCalculator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "SplineLikelihoodCalculator.h"
#include "CalculatorSimple.h"

SplineLikelihoodCalculator::SplineLikelihoodCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int xMaxIndex) : LikelihoodCalculator(expData, simData, xMinIndex, xMaxIndex)
{
    stepSet = false;
}

SplineLikelihoodCalculator::SplineLikelihoodCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int xMaxIndex, long double _step) : LikelihoodCalculator(expData, simData, xMinIndex, xMaxIndex)
{
    step = _step;
    stepSet = true;
}

SplineLikelihoodCalculator::SplineLikelihoodCalculator(DataSet * expData, DataSet * simData, long double _xMin, long double _xMax, long double _step) : LikelihoodCalculator(expData, simData, _xMin, _xMax)
{
    stepSet = true;
    step = _step;
}

SplineLikelihoodCalculator::~SplineLikelihoodCalculator()
{
}

long double SplineLikelihoodCalculator::ElaborateDelta()
{
    if (!stepSet)
        CalculateStep();
    return CalculatorSimple::SplineDiff(simulatedData, experimentalData, xMin, xMax, step);
}

void SplineLikelihoodCalculator::CalculateStep()
{
    step = fabs(simulatedData->x(0)-simulatedData->x(1))/10.0;
    stepSet = true;
}