//
//  ExperimentSimulator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 20/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ExperimentSimulatorFissures.h"
#include "CalculatorMax.h"
#include "CalculatorMaxSimple.h"
#include "TransformerSimple.h"
#include "CalculatorSimple.h"
#include "StepFunction.h"
#include "GlobalSettings.h"

#include <cmath>

/* ------------------------   Init Functions ---------------------- */
ExperimentSimulatorFissures::ExperimentSimulatorFissures(DataSet * expData) : ExperimentSimulator(expData)
{
}

ExperimentSimulatorFissures::~ExperimentSimulatorFissures()
{
	delete[] values;
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
    PhysicalSceneFissures * _scene =  new PhysicalSceneFissures();
    scene = _scene;
    for (int i = 0; i != fissureN; i++)
    {
        _scene->AddFissure(values[i*3], values[i*3+1], values[i*3+2]);
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
    //CalculatorMax * cMax = new CalculatorMax(simulatedData);
    CalculatorMaxSimple * cMax = new CalculatorMaxSimple(simulatedData);
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
    
    GetLikelihood(0);
    GetLikelihood(1);
    GetLikelihood(2);
    
    //PrintSimulatedDataToFile();
}

void ExperimentSimulatorFissures::PrintSimulatedDataToFile()
{
    std::ofstream datFile("sim-f"+std::to_string(fissureN)+"-"+ std::to_string(uniqueID)+".dat");
    datFile << "#Simulated data with id:"<< uniqueID<< std::endl;
    datFile << "# "; scene->PrintFormula(datFile); datFile << "*" << scaleValue << std::endl;
    simulatedData->PrintData(datFile);
    datFile.close();
    
/*    std::ofstream splineFile("ssp-f"+std::to_string(fissureN)+"-"+ std::to_string(uniqueID)+".dat");
    splineFile << "#Simulated data with id:"<< uniqueID<< std::endl;
    splineFile << "# "; scene->PrintFormula(splineFile); splineFile << "*" << scaleValue << std::endl;
    simulatedData->PrintSplineWithDerivate1(splineFile);
    splineFile.close(); */
}

void ExperimentSimulatorFissures::PrintSimulationFourierSpace()
{
	DataSet * fourierData = new DataSet();
	
	PhysicalScene * _scene =  new PhysicalScene();
	for (int i = 0; i != fissureN; i++)
	{
		_scene->AddFunction(new StepFunction(values[i*3], values[i*3+2], values[i*3+1]));
	}
	
	long double pos = -30.0; //-GlobalSettings::get_instance().FISSURE_POS;
	long double posMax = -pos;
	
	while (pos < posMax)
	{
		fourierData->AddMeasure(pos, (*_scene)(pos));
		pos += 0.1;
	}
	
	CalculatorMaxSimple * cMax = new CalculatorMaxSimple(fourierData);
    TransformerSimple::ScaleY(fourierData,1/cMax->GetMaxYPosition());
    delete cMax;

	
	std::ofstream datFile("fou-f"+std::to_string(fissureN)+"-"+ std::to_string(uniqueID)+".dat");
    datFile << "#Simulated data with id:"<< uniqueID<< std::endl;
    datFile << "# "; scene->PrintFormula(datFile); datFile << "*" << scaleValue << std::endl;
    fourierData->PrintData(datFile);
    datFile.close();
	
	delete fourierData;
	delete _scene;
}


