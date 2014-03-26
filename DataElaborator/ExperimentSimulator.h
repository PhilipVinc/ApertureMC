//
//  ExperimentSimulatorBase.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ExperimentSimulator__
#define __DataElaborator__ExperimentSimulator__

#include <vector>
#include "DataSet.h"
#include "PhysicalScene.h"
#include "SplineLikelihoodCalculator.h"

class ExperimentSimulator
{
public:
    /* ------------------------   Init Functions ---------------------- */
    ExperimentSimulator(DataSet * expData);
    virtual ~ExperimentSimulator();
    void BaseSetup(long double range);
    void BaseSetup(long double _minX, long double _maxX);
    
    void Work();
    
    void PrintSimulatedDataToFile(std::string filename);
    
    int GetNumberOfLikelihoods();
    long double GetLikelihood(int id);
    
    int uniqueID;
    bool ready;
protected:
    virtual void CreateExperiment()=0;
    virtual void SimulateExperiment()=0;
    virtual void Check()=0;
    
    PhysicalScene * scene;
    DataSet * experimentalData;
    DataSet * simulatedData;
    
    std::vector<LikelihoodCalculator*> likelihoodCalculators;
    
    long double range;
    int xMinIndex;
    int xMaxIndex;
};
#endif /* defined(__DataElaborator__ExperimentSimulator__) */
