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
    
    simulators = *new vector<ExperimentSimulator*>();
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
    results = new double [(simulationsN+num_threads)*4]; // longer so we just do some more simulations and there is no overflow risk.
}

void ThreadedResultSimulator::SetupRandomNumberGenerator()
{
	rng.seed(5);
    
    f1PosRangeDistribution = new uniform_real_distribution<double> (-f1PosRange, f1PosRange);
    f2DistanceDistribution = new uniform_real_distribution<double> (-f2DistRange, f2DistRange);
    apertureDistribution = new uniform_real_distribution<double> (aperture-1, aperture+1 );
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
            results[(simulators[i]->uniqueID)*4 +3] = simulators[i]->error;
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
    double variables[fenditureN];
    double _f1Pos = (*f1PosRangeDistribution)(rng);
    double _f2Dis = (*f2DistanceDistribution)(rng);
    double _aper = (*apertureDistribution)(rng);
    variables[0] = _f1Pos;
    variables[1] = 0.5;
    variables[2] = _aper;
    variables[3] = variables[0] + _f2Dis;
    variables[4] = 0.5;
    variables[5] = _aper/2;
    
    lastId++;
    results[lastId*4]=_f1Pos;
    results[lastId*4+1]=_f2Dis;
    results[lastId*4+2]=_aper;
    
    ExperimentSimulator * sim = new ExperimentSimulator(experimentalData);
    sim->uniqueID = lastId;
    sim->Setup(fenditureN, variables, 1);
    
    return sim;
}

using namespace std;

void ThreadedResultSimulator::PrintSingleSimulation(int id, ostream& myout)
{
    double _f1Pos = results[id*4];
    double _f2Dis = results[id*4+1];
    double _aper = results[id*4+2];
    
    myout << "Single Simulation with parameters for id "<< id<<endl;
    myout << "_f1Pos= " << _f1Pos << endl;
    myout << "_f2Dis= " << _f2Dis << endl;
    myout << "_aper= " << _aper << endl;
    
    double variables[fenditureN];
    variables[0] = _f1Pos;
    variables[1] = 0.5;
    variables[2] = _aper;
    variables[3] = variables[0] + _f2Dis;
    variables[4] = 0.5;
    variables[5] = _aper/2;
    ExperimentSimulator * sim = new ExperimentSimulator(experimentalData);
    sim->uniqueID = id;
    sim->Setup(fenditureN, variables, 1);
    sim->Work();
    sim->PrintSimulatedDataToFile();
}


void ThreadedResultSimulator::Print(ostream& myout)
{
	for (int i = 0; i < lastId; ++i)
	{
        for (int j = 0; j != 4; j++) {
            myout << results[i*4+j] << "\t";
        }
        myout << endl;
	}
	myout << endl;
}

void ThreadedResultSimulator::PrintEvaluation(ostream& myout)
{
    double minErr = results[3];
    int minIndex = 0;
    for (int i = 0; i < lastId; ++i)
    {
        if (minErr > results[i*4 +3] )
        {
            minErr = results[i*4+3];
            minIndex = i;
        }
    }
    myout << "-----------------------------------" << endl;
    myout << "the minimal difference is: "<<minErr << endl;
    PrintSingleSimulation(minIndex);
}
