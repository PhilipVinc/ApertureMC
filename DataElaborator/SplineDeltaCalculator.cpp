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

SplineDeltaCalculator::SplineDeltaCalculator(DataSet * expData, DataSet * simData, long double _xMin, long double _xMax) : DeltaCalculator(expData, simData, _xMin, _xMax)
{
}
    
SplineDeltaCalculator::~SplineDeltaCalculator()
{
}

long double SplineDeltaCalculator::ElaborateDelta()
{
    return CalculatorSimple::SplineDiff(simulatedData, experimentalData, xMin, xMax, 0.01);
}