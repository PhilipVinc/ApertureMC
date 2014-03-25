//
//  ExperimentSimulator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 20/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ExperimentSimulatorFissures.h"
#include <cmath>
#include "CalculatorMax.h"
#include "TransformerSimple.h"
#include "CalculatorSimple.h"

/* ------------------------   Init Functions ---------------------- */
ExperimentSimulatorFissures::ExperimentSimulatorFissures(DataSet * expData) : ExperimentSimulator(expData)
{
}

ExperimentSimulatorFissures::~ExperimentSimulatorFissures()
{
}

/* Setup() is called right after object creation. Should be merged. Usage is not clear */
void ExperimentSimulatorFissures::Setup(int _fissureN, long double * _setupValues, long double _range)
{
    // Find the
    fissureN = _fissureN;
    values = new long double[fissureN*3];
    for (int i = 0; i != fissureN*3; i++)
    {
        values[i]= _setupValues[i];
    }
    
    BaseSetup(_range);
}

void ExperimentSimulatorFissures::CreateExperiment()
{
    simulatedData = new DataSet();
    scene =  new PhysicalSceneFissures();
    for (int i = 0; i != fissureN; i++)
    {
        scene->AddFissure(values[i*3], values[i*3+1], values[i*3+2]);
    }
}

void ExperimentSimulatorFissures::SimulateExperiment()
{
    for (int i = xMinIndex; i != xMaxIndex; i++)
    {
        long double position = experimentalData->x(i);
        long double value = (*scene)(position);
        simulatedData->AddMeasure(position, value);
    }
    
    // Rescale data
    simulatedData->ComputeSplineCoefficients();
    CalculatorMax * cMax = new CalculatorMax(simulatedData);
    scaleValue = 1/cMax->GetMaxYPosition();
    TransformerSimple::ScaleY(simulatedData,scaleValue);
    delete cMax;
}

void ExperimentSimulatorFissures::Check()
{
    likelihoodCalculators.push_back(new StepLikelihoodCalculator(experimentalData, simulatedData, (xMinIndex*3+xMaxIndex)/4, (3*xMaxIndex+xMinIndex)/4));
    likelihoodCalculators.push_back(new StepLikelihoodCalculator(experimentalData, simulatedData, (xMinIndex+xMaxIndex)/2 - 4, (xMinIndex+xMaxIndex)/2 + 4));
    //likelihoodCalculators.push_back(new SplineDeltaCalculator(experimentalData, simulatedData, -0.03, 0.03, 0.001));
    likelihoodCalculators.push_back(new SplineLikelihoodCalculator(experimentalData, simulatedData, xMinIndex, xMaxIndex, 0.01));
    
    GetError(0);
    GetError(1);
    GetError(2);
    
    //PrintSimulatedDataToFile();
}

void ExperimentSimulatorFissures::PrintSimulatedDataToFile()
{
    std::ofstream datFile("sim-f"+std::to_string(fissureN)+"-"+ std::to_string(uniqueID)+".dat");
    datFile << "#Simulated data with id:"<< uniqueID<< std::endl;
    datFile << "# "; scene->PrintFormula(datFile); datFile << "*" << scaleValue << std::endl;
    simulatedData->PrintData(datFile);
    datFile.close();
    
    std::ofstream splineFile("ssp-f"+std::to_string(fissureN)+"-"+ std::to_string(uniqueID)+".dat");
    splineFile << "#Simulated data with id:"<< uniqueID<< std::endl;
    splineFile << "# "; scene->PrintFormula(splineFile); splineFile << "*" << scaleValue << std::endl;
    simulatedData->PrintSplineWithDerivate1(splineFile);
    splineFile.close();
}


