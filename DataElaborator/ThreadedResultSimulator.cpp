//
//  ThreadedResultSimulator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 20/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ThreadedResultSimulator.h"

using namespace std;

/* ------------------------   Init Functions ---------------------- */
ThreadedResultSimulator::ThreadedResultSimulator(DataSet * _experimentalData, int _simulationsN, int _nThreads)
{
    num_threads = _nThreads;
    simulationsN = _simulationsN;
    experimentalData = _experimentalData;
    
    //simulators = *new vector<ExperimentSimulator*>();
    SetupArrayResults();
    SetupRandomNumberGenerator();
    lastId = -1;
}

ThreadedResultSimulator::~ThreadedResultSimulator()
{
    delete [] threads;
    simulators.clear();
    delete[] results;
}

void ThreadedResultSimulator::SetupArrayResults()
{
    results = new long double [(simulationsN+num_threads)*(simDataN+resultsN)]; // longer so we just do some more simulations and there is no overflow risk.
}

void ThreadedResultSimulator::SetupRandomNumberGenerator()
{
	rng.seed(5);
    
    f1PosRangeDistribution = new uniform_real_distribution<long double> (-f1PosRange, f1PosRange);
    f2DistanceDistribution = new uniform_real_distribution<long double> (-f2DistRange, f2DistRange);
    f1apertureDistribution = new uniform_real_distribution<long double> (aperture-4, aperture+4 );
    f2apertureDistribution = new uniform_real_distribution<long double> (0.25, 0.75 );
}


void ThreadedResultSimulator::Start()
{
    Simulate();
}

void ThreadedResultSimulator::Simulate()
{
    int cyclesN = ceil(double(simulationsN)/double(num_threads));
    for (int cycle = 0; cycle < cyclesN; cycle++)
    {
        threads = new thread[num_threads];
        
        for (int i = 0; i != num_threads; i++)
        {
            //Setup The Simulator
            simulators.push_back(CreateSim());
        }
        
        for (int i = 0; i != num_threads; i++)
        {
            threads[i] = std::thread(&ExperimentSimulator::Work, simulators[i]);
        }
        
        for (int i = 0; i != num_threads; i++)
        {
            threads[i].join();
        }
        delete[] threads;
        
        for (int i = 0; i != num_threads; i++)
        {
            results[(simulators[i]->uniqueID)*(simDataN+resultsN)] = simulators[i]->error;
            results[(simulators[i]->uniqueID)*(simDataN+resultsN)+1] = simulators[i]->errorSmall;
            delete simulators[i];
        }
        simulators.clear();
        double percent = double(cycle)/double(cyclesN)*100;
        cout << percent << "\%" << endl;
    }
    PrintEvaluation(std::cout);
}

ExperimentSimulator * ThreadedResultSimulator::CreateSim()
{
    long double variables[fenditureN*3];
    long double _f1Pos = (*f1PosRangeDistribution)(rng);
    long double _f2Dis = (*f2DistanceDistribution)(rng);
    long double _f1aper = (*f1apertureDistribution)(rng);
    long double _f2aper = (*f2apertureDistribution)(rng);
    
    variables[0] = _f1Pos;
    variables[1] = 0.5;
    variables[2] = _f1aper;
    variables[3] = variables[0] + _f2Dis;
    variables[4] = 0.5;
    variables[5] = _f2aper*_f1aper;
    
    /* for (int i = 0; i < 6; i++)
    {
        cout << i<<"= "<<variables[i] << "\t";
    }
    cout << endl; */
    
    lastId++;
    results[lastId*(simDataN+resultsN)+(resultsN)]= _f1Pos;
    results[lastId*(simDataN+resultsN)+(resultsN+1)]= _f2Dis;
    results[lastId*(simDataN+resultsN)+(resultsN+2)]= _f1aper;
    results[lastId*(simDataN+resultsN)+(resultsN+3)]= _f2aper;

    ExperimentSimulator * sim = new ExperimentSimulator(experimentalData);
    sim->uniqueID = lastId;
    sim->Setup(fenditureN, variables, 1);
    
    return sim;
}

void ThreadedResultSimulator::PrintSingleSimulation(int id, ostream& myout)
{
    long double _f1Pos = results[id*(simDataN+resultsN)+(resultsN)];
    long double _f2Dis = results[id*(simDataN+resultsN)+(resultsN+1)];
    long double _f1aper = results[id*(simDataN+resultsN)+(resultsN+2)];
    long double _f2aper = results[id*(simDataN+resultsN)+(resultsN+3)];
    
    myout << "Single Simulation with parameters for id "<< id<<endl;
    myout << "_f1Pos= " << _f1Pos << endl;
    myout << "_f2Dis= " << _f2Dis << endl;
    myout << "_f1aper= " << _f1aper << endl;
    myout << "_f2aper= " << _f2aper << endl;
    
    long double variables[fenditureN*3];
    variables[0] = _f1Pos;
    variables[1] = 0.5;
    variables[2] = _f1aper;
    variables[3] = variables[0] + _f2Dis;
    variables[4] = 0.5;
    variables[5] = _f2aper*_f1aper;
    ExperimentSimulator * sim = new ExperimentSimulator(experimentalData);
    sim->uniqueID = id;
    sim->Setup(fenditureN, variables, 1);
    sim->Work();
    sim->PrintSimulatedDataToFile();
    
    delete sim;
}


void ThreadedResultSimulator::Print(ostream& myout)
{
	for (int i = 0; i < lastId; ++i)
	{
        myout << i << " \t ";
        for (int j = 0; j != simDataN+resultsN; j++) {
            myout << results[i*(simDataN+resultsN)+j] << " \t ";
        }
        myout << endl;
	}
	myout << endl;
}

void ThreadedResultSimulator::PrintEvaluation(ostream& myout)
{
    long double minErr = results[0];
    int minIndex = 0;
    for (int i = 0; i < lastId; ++i)
    {
        if (minErr > results[i*(simDataN+resultsN)] )
        {
            minErr = results[i*(simDataN+resultsN)];
            minIndex = i;
        }
    }
    myout << "-----------------------------------" << endl;
    myout << "the minimal difference is: "<<minErr << endl;
    PrintSingleSimulation(minIndex);
    
    
    myout << "-----------------------------------" << endl;
    minErr = results[1];
    minIndex = 0;
    for (int i = 0; i < lastId; ++i)
    {
        if (minErr > results[i*(simDataN+resultsN)+1] )
        {
            minErr = results[i*(simDataN+resultsN)+1];
            minIndex = i;
        }
    }
    myout << "the minimal difference on the top is: "<<minErr << endl;
    PrintSingleSimulation(minIndex);
}
