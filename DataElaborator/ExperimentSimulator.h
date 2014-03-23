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
    void Setup(int fissureN, double * setupValues, double range = 1.0);
    
    
    void Work();
    
    void CreateExperiment();
    void SimulateExperiment();
    void Check();
    
    void PrintSimulatedDataToFile();
    
    double error;
    double errorSmall;
    double newError;
    int uniqueID;
    
protected:
    PhysicalScene2 * scene;
    DataSet * experimentalData;
    DataSet * simulatedData;
    
    int fissureN;
    double * values;
    double range;
    int xMinIndex;
    int xMaxIndex;
    
    double scaleValue;
    
};


#endif /* defined(__DataElaborator__ExperimentSimulator__) */
