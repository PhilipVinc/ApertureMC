//
//  ThreadedResultSimulatorBase.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 24/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ThreadedResultSimulator.h"

using namespace std;

ThreadedResultSimulator::ThreadedResultSimulator(DataSet * _experimentalData, int _simulationsN, int _nThreads)
{
    num_threads = _nThreads;
    simulationsN = _simulationsN;
    experimentalData = _experimentalData;
    settings = &GlobalSettings::get_instance();
    lastId = -1;

    //SetupArrayResults();
    //SetupRandomNumberGenerator();
}

ThreadedResultSimulator::~ThreadedResultSimulator()
{
    cycleResults.clear();
    simulators.clear();
}

void ThreadedResultSimulator::Simulate()
{
    OnPreSimulation();
    int cyclesN = ceil(double(simulationsN)/double(num_threads));
    long double lastPercent = 0.0;
    
    threads = new thread[num_threads];
    std::vector<WorkerThread*> workers;
    for (int i = 0; i<num_threads; i++)
    {
        workers.push_back(new WorkerThread(i));
        threads[i] = std::thread(&WorkerThread::WorkerLoop, workers[i]);
    }
    
    for (int cycle = 0; cycle < cyclesN; cycle++)
    {
        for (int i = 0; i != num_threads; i++)
        {
            simulators.push_back(CreateSim(i));
            workers[i]->AssignSimulator(simulators[i]);
        }
        
        for (int i = 0; i != num_threads; i++)
        {
            while (!workers[i]->IsFinished())
            {
            }
            for (int j = 0; j < likelihoodsN; j++)
            {
                cycleResults[(i)*(simDataN+likelihoodsN)+j] = simulators[i]->GetLikelihood(j);
            }
            delete simulators[i];
        }
        
        simulators.clear();
        CheckBestSim();
        
        long double percent = double(cycle)/double(cyclesN);
        if (settings->showProgress)
        {
            if ( percent > lastPercent )
            {
                settings->DrawSimulationProgressBar(40, percent, 4);
                lastPercent = percent + 0.01;
            }
        }
    }
    for (int i = 0; i<num_threads; i++)
    {
        workers[i]->Terminate();
    }
    OnPostSimulation();
}