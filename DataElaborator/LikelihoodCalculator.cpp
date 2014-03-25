//
//  DeltaCalculator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "LikelihoodCalculator.h"

LikelihoodCalculator::LikelihoodCalculator(DataSet * expData, DataSet * simData, int _xMinIndex, int _xMaxIndex)
{
    experimentalData = expData;
    simulatedData = simData;
    xMinIndex = _xMinIndex;
    xMaxIndex = _xMaxIndex;
    rangeSet = false;
}


LikelihoodCalculator::LikelihoodCalculator(DataSet * expData, DataSet * simData, long double _xMin, long double _xMax)
{
    experimentalData = expData;
    simulatedData = simData;
    rangeSet = true;
    xMin = _xMin;
    xMax = _xMax;
}

LikelihoodCalculator::~LikelihoodCalculator()
{
    
}

long double LikelihoodCalculator::GetDelta()
{
    if (resultDirty)
    {
        SetupIndices();
        result = ElaborateDelta();
        resultDirty = false;
    }
    return result;
}

void LikelihoodCalculator::SetupIndices()
{
    if (!rangeSet)
    {
        xMinExpIndex = xMinIndex;
        xMaxExpIndex = xMaxIndex;
        
        xMin = experimentalData->x(xMinExpIndex);
        xMax = experimentalData->x(xMaxExpIndex);
        
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
        for (int i = 0; i != experimentalData->n; i++)
        {
            if (experimentalData->x(i) >= xMin)
            {
                xMinExpIndex = i;
                break;
            }
        }
        
        for (int i = experimentalData->n-1; i != 0; i--)
        {
            if (experimentalData->x(i) <= xMax)
            {
                xMaxExpIndex = i;
                break;
            }
        }
        
        for (int i = 0; i != simulatedData->n; i++)
        {
            if (simulatedData->x(i) >= xMin)
            {
                xMinSimIndex = i;
                break;
            }
        }
        
        for (int i = simulatedData->n-1; i != 0; i--)
        {
            if (simulatedData->x(i) <= xMax)
            {
                xMaxSimIndex = i;
                break;
            }
        }
    }
}
