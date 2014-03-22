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

/* ------------------------   Init Functions ---------------------- */
ExperimentSimulator::ExperimentSimulator(DataSet * expData)
{
    experimentalData = expData;
}

ExperimentSimulator::~ExperimentSimulator()
{
    delete [] values;
    delete scene;
    delete simulatedData;
}

/* Setup() is called right after object creation. Should be merged. Usage is not clear */
void ExperimentSimulator::Setup(int _fissureN, double * _setupValues, double _range)
{
    // Find the
    fissureN = _fissureN;
    values = new double[fissureN*3];
    for (int i = 0; i != fissureN*3; i++)
    {
        values[i]= _setupValues[i];
    }
    
    // Find the range in the experimental data that we care about
    range = _range;
    for (int i = 0; i < experimentalData->n; i++)
    {
        if (experimentalData->xp[i] > (0-range) )
        {
            xMinIndex = i;
            break;
        }
    }
    for (int i = 0; i < experimentalData->n; i++)
    {
        if (experimentalData->xp[i] > range )
        {
            xMaxIndex = i-1;
            break;
        }
    }
}

void ExperimentSimulator::Work()
{
    CreateExperiment();
    SimulateExperiment();
    Check();
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
        double position = experimentalData->xp[i];
        double value = (*scene)(position);
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
    
    double diff = 0.0;
    int simIndex = 0;
   
    //for (int i = xMinIndex; i != xMaxIndex; i++)
    for (int i = (xMinIndex*3+xMaxIndex)/4; i != (3*xMaxIndex+xMinIndex)/4; i++)
    {
        simIndex = i - xMinIndex;
        double a1= simulatedData->yp[simIndex];
        double a2 = experimentalData->yp[i];
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
        double a1= simulatedData->yp[simIndex];
        double a2 = experimentalData->yp[i];
        diff = fabs(a1-a2);
        errorSmall += diff*diff;
    }
    errorSmall /= 10 ;
    errorSmall *= 100;

    //std::cout << "worker ID: "<< uniqueID << " - has calculated error= "<< error<<std::endl;
    delete cMax;
    //PrintSimulatedDataToFile();
}

void ExperimentSimulator::PrintSimulatedDataToFile()
{
    std::ofstream datFile("sim-" + std::to_string(uniqueID)+".dat");
    datFile << "#Simulated data with id:"<< uniqueID<< std::endl;
    datFile << "# "; scene->PrintFormula(datFile); datFile << "*" << scaleValue << std::endl;
    simulatedData->PrintData(datFile);
    datFile.close();
}





