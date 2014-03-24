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
    simulators.clear();
}

