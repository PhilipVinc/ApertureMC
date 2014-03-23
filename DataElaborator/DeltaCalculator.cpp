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


DeltaCalculator::DeltaCalculator(DataSet * expData, DataSet * simData, long double _xMin, long double _xMax)
{
    experimentalData = expData;
    simulatedData = simData;
    rangeSet = true;
    xMin = _xMin;
    xMax = _xMax;
}

long double DeltaCalculator::GetDelta()
{
    if (resultDirty)
    {
        SetupIndices();
        ElaborateDelta();
    }
    return result;
}

void DeltaCalculator::SetupIndices()
{
    if (!rangeSet)
    {
        xMinExpIndex = xMinIndex;
        xMaxExpIndex = xMaxIndex;
        
        for (int i = 0; i != simulatedData->n; i++)
        {
            if (simulatedData->x(i) >= experimentalData->x(xMinExpIndex))
            {
                xMinSimIndex = i;
                break;
            }
        }
        
        for (int i = simulatedData->n-1; i != 0; i--)
        {
            if (simulatedData->x(i) <= experimentalData->x(xMaxExpIndex))
            {
                xMaxSimIndex = i;
                break;
            }
        }
        
    }
    else
    {
        
    }
}
