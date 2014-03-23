//
//  DeltaCalculator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "DeltaCalculator.h"

DeltaCalculator::DeltaCalculator(DataSet * expData, DataSet * simData, int _xMinIndex, int _xMaxIndex)
{
    experimentalData = expData;
    simulatedData = simData;
    xMinIndex = _xMinIndex;
    xMaxIndex = _xMaxIndex;
}

DeltaCalculator::~DeltaCalculator()
{
}

