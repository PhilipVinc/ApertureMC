//
//  ExperimentSimulator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 20/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ExperimentSimulatorFissures__
#define __DataElaborator__ExperimentSimulatorFissures__

#include <iostream>
#include "DataSet.h"
#include "PhysicalSceneFissures.h"
#include "ExperimentSimulator.h"
#include "StepLikelihoodCalculator.h"
#include "SplineLikelihoodCalculator.h"

class ExperimentSimulatorFissures : public ExperimentSimulator
{
public:
    /* ------------------------   Init Functions ---------------------- */
    ExperimentSimulatorFissures(DataSet * expData);
    ~ExperimentSimulatorFissures();
    
    void Setup(int fissureN, long double * setupValues, long double range = 1.0);
    
    void CreateExperiment();
    void SimulateExperiment();
    void Check();
    
    void PrintSimulatedDataToFile();
    
    int uniqueID;
    
protected:
    int fissureN;
    
    long double scaleValue;
};


#endif /* defined(__DataElaborator__ExperimentSimulatorFissures__) */
