//
//  ThreadedResultSimulator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 24/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ThreadedResultSimulator__
#define __DataElaborator__ThreadedResultSimulator__

#include <iostream>
#include <vector>
#include <thread>
#include "DataSet.h"
#include "ExperimentSimulator.h"
#include "GlobalSettings.h"

class ThreadedResultSimulator
{
public:
    ThreadedResultSimulator(DataSet * _experimentalData, int _simulationsN, int _nThreads);
    virtual ~ThreadedResultSimulator();
    
    void Simulate();
protected:
    GlobalSettings * settings;
    
    int num_threads;
    int simulationsN;
    std::thread * threads;
    
    DataSet * experimentalData;
    std::vector<ExperimentSimulatorBase*> simulators;
    
    int lastId;

    //virtual ExperimentSimulator * CreateSim(int id)=0;

};

#endif /* defined(__DataElaborator__ThreadedResultSimulator__) */
