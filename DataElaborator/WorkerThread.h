//
//  WorkerThread.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 24/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__WorkerThread__
#define __DataElaborator__WorkerThread__

#include <iostream>
#include "ExperimentSimulatorBase.h"

class WorkerThread
{
public:
    WorkerThread(int id);
    ~WorkerThread();
    void Terminate();
    void WorkerLoop();

    void AssignSimulator(ExperimentSimulatorBase * sim);
    void ClearSimulator();

    bool IsFinished();
    
    int id;
    
protected:
    ExperimentSimulatorBase * simulator;
    bool gotSimulator = false;
    bool terminate = false;
};

#endif /* defined(__DataElaborator__WorkerThread__) */