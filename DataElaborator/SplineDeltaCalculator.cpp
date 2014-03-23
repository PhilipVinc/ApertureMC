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
    
}

long double SplineDeltaCalculator::GetDelta()
{
    return CalculatorSimple::SplineDiff(simulatedData, experimentalData, simulatedData->x(0), simulatedData->x(simulatedData->n -1), 0.01);
}