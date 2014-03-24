//
//  SplineDeltaCalculator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "SplineDeltaCalculator.h"
#include "CalculatorSimple.h"

SplineDeltaCalculator::SplineDeltaCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int xMaxIndex) : DeltaCalculator(expData, simData, xMinIndex, xMaxIndex)
{
    stepSet = false;
}

SplineDeltaCalculator::SplineDeltaCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int xMaxIndex, long double _step) : DeltaCalculator(expData, simData, xMinIndex, xMaxIndex)
{
    step = _step;
    stepSet = true;
}

SplineDeltaCalculator::SplineDeltaCalculator(DataSet * expData, DataSet * simData, long double _xMin, long double _xMax, long double _step) : DeltaCalculator(expData, simData, _xMin, _xMax)
{
    stepSet = true;
    step = _step;
}

SplineDeltaCalculator::~SplineDeltaCalculator()
{
}

long double SplineDeltaCalculator::ElaborateDelta()
{
    if (!stepSet)
        CalculateStep();
    return CalculatorSimple::SplineDiff(simulatedData, experimentalData, xMin, xMax, step);
}

void SplineDeltaCalculator::CalculateStep()
{
    step = fabs(simulatedData->x(0)-simulatedData->x(1))/10.0;
    stepSet = true;
}