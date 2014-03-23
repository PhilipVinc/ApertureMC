//
//  ExperimentSimulator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 20/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ExperimentSimulator__
#define __DataElaborator__ExperimentSimulator__

#include <iostream>
#include "DataSet.h"
#include "PhysicalScene2.h"

class ExperimentSimulator
{
public:
    /* ------------------------   Init Functions ---------------------- */
    ExperimentSimulator(DataSet * expData);
    ~ExperimentSimulator();
    void Setup(int fissureN, long double * setupValues, long double range = 1.0);
    
    
    void Work();
    
    void CreateExperiment();
    void SimulateExperiment();
    void Check();
    
    void PrintSimulatedDataToFile();
    
    long double error;
    long double errorSmall;
    long double newError;
    int uniqueID;
    
protected:
    PhysicalScene2 * scene;
    DataSet * experimentalData;
    DataSet * simulatedData;
    
    int fissureN;
    long double * values;
    long double range;
    int xMinIndex;
    int xMaxIndex;
    
    long double scaleValue;
    
};


#endif /* defined(__DataElaborator__ExperimentSimulator__) */
