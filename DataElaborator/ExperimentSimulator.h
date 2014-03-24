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
#include "PhysicalSceneFissures.h"
#include "ExperimentSimulatorBase.h"
#include "StepDeltaCalculator.h"
#include "SplineDeltaCalculator.h"

class ExperimentSimulator : public ExperimentSimulatorBase
{
public:
    /* ------------------------   Init Functions ---------------------- */
    ExperimentSimulator(DataSet * expData);
    ~ExperimentSimulator();
    
    void Setup(int fissureN, long double * setupValues, long double range = 1.0);
        
    void CreateExperiment();
    void SimulateExperiment();
    void Check();
    
    void PrintSimulatedDataToFile();
    
    long double error;
    long double errorSmall;
    long double newError;
    int uniqueID;
    
protected:
    int fissureN;
    
    long double scaleValue;
};


#endif /* defined(__DataElaborator__ExperimentSimulator__) */
