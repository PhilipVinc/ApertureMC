//
//  ExperimentSimulator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 20/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ExperimentSimulator.h"
#include <cmath>
#include "CalculatorMax.h"
#include "TransformerSimple.h"
#include "CalculatorSimple.h"

/* ------------------------   Init Functions ---------------------- */
ExperimentSimulator::ExperimentSimulator(DataSet * expData) : ExperimentSimulatorBase(expData)
{
}

ExperimentSimulator::~ExperimentSimulator()
{
}

/* Setup() is called right after object creation. Should be merged. Usage is not clear */
void ExperimentSimulator::Setup(int _fissureN, long double * _setupValues, long double _range)
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

void ExperimentSimulator::CreateExperiment()
{
    simulatedData = new DataSet();
    scene =  new PhysicalScene2();
    for (int i = 0; i != fissureN; i++)
    {
        scene->AddFissure(values[i*3], values[i*3+1], values[i*3+2]);
    }
}

void ExperimentSimulator::SimulateExperiment()
{
    for (int i = xMinIndex; i != xMaxIndex; i++)
    {
        long double position = experimentalData->x(i);
        long double value = (*scene)(position);
        simulatedData->AddMeasure(position, value);
    }
}

void ExperimentSimulator::Check()
{
    // Rescale data
    simulatedData->ComputeSplineCoefficients();
    CalculatorMax * cMax = new CalculatorMax(simulatedData);
    scaleValue = 1/cMax->GetMaxYPosition();
    TransformerSimple::ScaleY(simulatedData,scaleValue);
    
    error = 0.0;
    
    long double diff = 0.0;
    int simIndex = 0;
   
    //for (int i = xMinIndex; i != xMaxIndex; i++)
    for (int i = (xMinIndex*3+xMaxIndex)/4; i != (3*xMaxIndex+xMinIndex)/4; i++)
    {
        simIndex = i - xMinIndex;
        long double a1= simulatedData->y(simIndex);
        long double a2 = experimentalData->y(i);
        diff = fabs(a1-a2);
        error += diff*diff;
    }
    error /= xMaxIndex-xMinIndex;
    error *= 100;
    
    int center = (xMinIndex+xMaxIndex)/2;
    errorSmall = 0.0;
    for (int i = center - 5; i != center + 5; i++)
    {
        simIndex = i - xMinIndex;
        long double a1= simulatedData->y(simIndex);
        long double a2 = experimentalData->y(i);
        diff = fabs(a1-a2);
        errorSmall += diff*diff;
    }
    errorSmall /= 10 ;
    errorSmall *= 100;
    
    /*
    long long double pos = -range;
    int itt = 0;
    newError = 0.0;
    do {
        long double a1= simulatedData->SplineValue(pos);
        long double a2 = experimentalData->SplineValue(pos);
        diff = fabs(a1-a2);
        newError += diff*diff;
        itt++;
    } while (pos <= range);
    newError /= long double(itt);
    */
    newError = CalculatorSimple::SplineDiff(simulatedData, experimentalData, -range, +range, 0.01);

    //std::cout << "worker ID: "<< uniqueID << " - has calculated error= "<< error<<std::endl;
    delete cMax;
    //PrintSimulatedDataToFile();
}

void ExperimentSimulator::PrintSimulatedDataToFile()
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


