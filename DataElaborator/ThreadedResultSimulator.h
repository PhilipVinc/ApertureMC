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
#include "WorkerThread.h"

class ThreadedResultSimulator
{
public:
    ThreadedResultSimulator(DataSet * _experimentalData, int _simulationsN, int _nThreads);
    virtual ~ThreadedResultSimulator();
    
    void Simulate();
protected:
    virtual ExperimentSimulator* CreateSim(int id)=0;
    virtual void CheckBestSim()= 0;
    virtual void OnPreSimulation()=0;
    virtual void OnPostSimulation()=0;
    
    DataSet * experimentalData;
    std::vector<ExperimentSimulator*> simulators;
    std::vector <long double> cycleResults;//long double * cycleResults;
    
    int num_threads;
    int simulationsN;
    std::thread * threads;
    
    int simDataN;
    int likelihoodsN = 3;
    
    int lastId;

    GlobalSettings * settings;
};

#endif /* defined(__DataElaborator__ThreadedResultSimulator__) */
