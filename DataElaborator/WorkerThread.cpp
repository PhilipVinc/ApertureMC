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
}

void WorkerThread::AssignSimulator(ExperimentSimulator * sim)
{
	finished = false;
	gotSimulator = true;
    simulator = sim;
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
        if (gotSimulator && (simulator != nullptr))
        {
            simulator->Work();
			finished = true;
            ClearSimulator();
        }

    }
    delete this;
}

bool WorkerThread::IsFinished() { return (finished); }

void WorkerThread::Terminate()
{
    terminate = true;
}