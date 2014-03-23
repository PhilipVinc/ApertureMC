//
//  ExperimentSimulatorBase.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ExperimentSimulatorBase.h"

#include <string>

/* ------------------------   Init Functions ---------------------- */
ExperimentSimulatorBase::ExperimentSimulatorBase(DataSet * expData)
{
    experimentalData = expData;
}

ExperimentSimulatorBase::~ExperimentSimulatorBase()
{
    delete [] values;
    delete scene;
    delete simulatedData;
}

void ExperimentSimulatorBase::BaseSetup(long double _range)
{
    // Find the range in the experimental data that we care about
    range = _range;
    for (int i = 0; i < experimentalData->n; i++)
    {
        if (experimentalData->x(i) > (0-range) )
        {
            xMinIndex = i;
            break;
        }
    }
    for (int i = 0; i < experimentalData->n; i++)
    {
        if (experimentalData->x(i) > range )
        {
            xMaxIndex = i-1;
            break;
        }
    }
}

void ExperimentSimulatorBase::BaseSetup(long double _minX, long double _maxX)
{
    // Find the range in the experimental data that we care about
    for (int i = 0; i < experimentalData->n; i++)
    {
        if (experimentalData->x(i) > _minX )
        {
            xMinIndex = i;
            break;
        }
    }
    for (int i = 0; i < experimentalData->n; i++)
    {
        if (experimentalData->x(i) > _maxX )
        {
            xMaxIndex = i-1;
            break;
        }
    }
}


void ExperimentSimulatorBase::Work()
{
    CreateExperiment();
    SimulateExperiment();
    Check();
}

void ExperimentSimulatorBase::PrintSimulatedDataToFile(std::string _filename)
{
    std::ofstream datFile(_filename);
    datFile << "#Simulated data with id:"<< uniqueID<< std::endl;
    datFile << "# "; scene->PrintFormula(datFile); datFile << std::endl;
    simulatedData->PrintData(datFile);
    datFile.close();
}