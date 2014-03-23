//
//  ExperimentSimulatorBase.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ExperimentSimulatorBase__
#define __DataElaborator__ExperimentSimulatorBase__

#include <iostream>
#include "DataSet.h"
#include "PhysicalScene2.h"

class ExperimentSimulatorBase
{
public:
    /* ------------------------   Init Functions ---------------------- */
    ExperimentSimulatorBase(DataSet * expData);
    ~ExperimentSimulatorBase();
    void BaseSetup(long double range);
    void BaseSetup(long double _minX, long double _maxX);
    
    void Work();
    
    virtual void CreateExperiment()=0;
    virtual void SimulateExperiment()=0;
    virtual void Check()=0;
    
    void PrintSimulatedDataToFile(std::string filename);
    
    int uniqueID;
protected:
    PhysicalScene2 * scene;
    DataSet * experimentalData;
    DataSet * simulatedData;
    
    long double * values;
    long double range;
    int xMinIndex;
    int xMaxIndex;
};
#endif /* defined(__DataElaborator__ExperimentSimulatorBase__) */
