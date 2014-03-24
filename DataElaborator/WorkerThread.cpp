//
//  WorkerThread.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 24/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "WorkerThread.h"

WorkerThread::WorkerThread(int _id)
{
    id = _id;
}

WorkerThread::~WorkerThread()
{
    Terminate();
}

void WorkerThread::AssignSimulator(ExperimentSimulatorBase * sim)
{
    simulator = sim;
    gotSimulator = true;
}

void WorkerThread::ClearSimulator()
{
    gotSimulator = false;
    simulator = nullptr;
}

void WorkerThread::WorkerLoop()
{
    while (!terminate)
    {
        if (gotSimulator)
        {
            simulator->Work();
            ClearSimulator();
        }
    }
    delete this;
}

bool WorkerThread::IsFinished() { return (!gotSimulator); }

void WorkerThread::Terminate()
{
    terminate = true;
}