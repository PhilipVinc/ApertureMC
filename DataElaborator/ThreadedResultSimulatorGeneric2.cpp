//
//  ThreadedResultSimulatorGeneric2.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ThreadedResultSimulatorGeneric2.h"

using namespace std;

/* ------------------------   Init Functions ---------------------- */
ThreadedResultSimulatorGeneric2::ThreadedResultSimulatorGeneric2(DataSet * _experimentalData, int fenditure, int _simulationsN, int _nThreads)
{
    num_threads = _nThreads;
    simulationsN = _simulationsN;
    experimentalData = _experimentalData;
    
    fenditureN = fenditure;
    SetupArrayResults();
    SetupRandomNumberGenerator();
    lastId = -1;
}

ThreadedResultSimulatorGeneric2::~ThreadedResultSimulatorGeneric2()
{
    simulators.clear();
    delete[] cycleResults;
    delete[] bestResults;
}

void ThreadedResultSimulatorGeneric2::SetupArrayResults()
{
    simDataN = dataPerFend*fenditureN;
    //results = new long double [(simulationsN+num_threads)*(simDataN+resultsN)]; // longer so we just do some more simulations and there is no overflow risk.
    cycleResults = new long double [(num_threads)*(simDataN+resultsN)];
    bestResults = new long double [2*(simDataN+resultsN)];
    minNewError = 1000000;
    minError = 1000000;
}

void ThreadedResultSimulatorGeneric2::SetupRandomNumberGenerator()
{
	rng.seed(5);
    
    posRangeDistribution = new uniform_real_distribution<long double> (-f1PosRange, f1PosRange);
    intensityDistribution = new uniform_real_distribution<long double> (0.0, 100.0 );
    apertureDistribution = new uniform_real_distribution<long double> (aperture-4, aperture+4 );
}


void ThreadedResultSimulatorGeneric2::Simulate()
{
    int cyclesN = ceil(double(simulationsN)/double(num_threads));
    long double lastPercent = 0.0;
    for (int cycle = 0; cycle < cyclesN; cycle++)
    {
        threads = new thread[num_threads];
        
        for (int i = 0; i != num_threads; i++)
        {
            //Setup The Simulator
            simulators.push_back(CreateSim(i));
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
            cycleResults[(i)*(simDataN+resultsN)] = simulators[i]->error;
            cycleResults[(i)*(simDataN+resultsN)+1] = simulators[i]->newError;
            //cout << "Iter: " << cycle*num_threads+i << " with error " << cycleResults[(i)*(simDataN+resultsN)] << " \t and new error " << cycleResults[(i)*(simDataN+resultsN)+1]<<endl;
            delete simulators[i];
        }
        simulators.clear();
        CheckBestSim();
        
        long double percent = double(cycle)/double(cyclesN);
        if ( percent > lastPercent )
        {
            DrawProgressBar(100, percent);
            lastPercent = percent + 0.01;
        }
    }
    std::cout << std::endl;
    PrintEvaluation(std::cout);
    //PrintTopEvaluation(std::cout);
    PrintNewEvaluation(std::cout);
}

void ThreadedResultSimulatorGeneric2::CheckBestSim()
{
    long double minErrOld = cycleResults[0];
    int minIndexOld = 0;
    for (int i = 0; i < num_threads; i++)
    {
        if (minErrOld > cycleResults[(i)*(simDataN+resultsN)] )
        {
            minIndexOld = i;
            minErrOld = cycleResults[(i)*(simDataN+resultsN)];
        }
    }
    if (minErrOld < minError)
    {
        minError = minErrOld;
        //cout << "OLD best: ";
        for (int i = 0; i < simDataN+resultsN; i++)
        {
            bestResults[i] = cycleResults[(minIndexOld)*(simDataN+resultsN)+i];
            //cout << i<< "= "<< bestResults[i] <<" \t ";
        }
        //cout << endl;
    }
    
    long double minErrNew = cycleResults[0];
    int minIndexNew = 0;
    for (int i = 0; i < num_threads; i++)
    {
        if (minErrNew > cycleResults[(i)*(simDataN+resultsN)+1] )
        {
            minIndexNew = i;
            minErrNew = cycleResults[(i)*(simDataN+resultsN)+1];
        }
    }
    if (minErrNew < minNewError)
    {
        minNewError = minErrNew;
        //cout << "NEW best: ";
        for (int i = 0; i < simDataN+resultsN; i++)
        {
            bestResults[ (simDataN+resultsN) +i] = cycleResults[(minIndexNew)*(simDataN+resultsN)+i];
            //cout << i<< "= "<< bestResults[i] <<" \t ";
        }
        //cout << endl;
    }
}

ExperimentSimulator * ThreadedResultSimulatorGeneric2::CreateSim(int threadN)
{
    long double variables[simDataN];
    for (int i = 0; i < fenditureN; i++)
    {
        variables[i*dataPerFend] = (*posRangeDistribution)(rng);
        variables[i*dataPerFend + 1 ] = (*intensityDistribution)(rng);
        variables[i*dataPerFend + 2 ] = (*apertureDistribution)(rng)/(i+1);
    }
    variables[1] = 1.0;
    
    lastId++;
    //cout << "Single Simulation with parameters for id "<< lastId <<endl;
    for (int i = 0; i < simDataN; i++)
    {
        cycleResults[threadN*(simDataN+resultsN) +(resultsN+i)] = variables[i];
        //  cout << i<<"= "<<variables[i] << " \t ";
    }
    //cout << endl;
    
    ExperimentSimulator * sim = new ExperimentSimulator(experimentalData);
    sim->uniqueID = lastId;
    sim->Setup(fenditureN, variables, 1);
    
    return sim;
}

void ThreadedResultSimulatorGeneric2::PrintSingleSimulation(int bestId, ostream& myout)
{
    long double variables[fenditureN*3];
    for (int i = 0; i < simDataN; i++)
    {
        variables[i] = bestResults[bestId*(simDataN+resultsN) +(resultsN+i)];
    }
    
    myout << "Single Simulation with parameters for bestId "<< bestId <<endl;
    cout << "#Fend \t posizione \t intensità \t apertura" << endl;
    for (int i = 0; i < fenditureN; i++)
    {
        cout << "F-"<< i+1 << " \t ";
        for (int j = 0; j < dataPerFend; j++)
        {
            cout << variables[i*dataPerFend+j] << " \t ";
        }
        cout << endl;
    }
    
    cout << endl;
    
    ExperimentSimulator * sim = new ExperimentSimulator(experimentalData);
    sim->uniqueID = bestId;
    sim->Setup(fenditureN, variables, 1);
    sim->Work();
    sim->PrintSimulatedDataToFile();
    
    delete sim;
}

void ThreadedResultSimulatorGeneric2::Print(ostream& myout)
{
	for (int i = 0; i < resultsN; ++i)
	{
        myout << i << " \t ";
        for (int j = 0; j != simDataN+resultsN; j++) {
            myout << bestResults[i*(simDataN+resultsN)+j] << " \t ";
        }
        myout << endl;
	}
	myout << endl;
}

void ThreadedResultSimulatorGeneric2::PrintEvaluation(ostream& myout)
{
    myout << "-----------------------------------" << endl;
    myout << "the minimal difference for "<< fenditureN << " fenditures is: "<< minError << endl;
    PrintSingleSimulation(0);
}

void ThreadedResultSimulatorGeneric2::PrintNewEvaluation(ostream& myout)
{
    myout << "the minimal difference for "<< fenditureN << " fenditures with the new eval. is: "<<minNewError << endl;
    PrintSingleSimulation(1);
}

void ThreadedResultSimulatorGeneric2::DrawProgressBar(int len, long double percent)
{
    percent = percent + 0.01;
    cout << "\x1B[2K"; // Erase the entire current line.
    cout << "\x1B[0E"; // Move to the beginning of the current line.
    string progress;
    for (int i = 0; i < len; ++i) {
        if (i < static_cast<int>(len * percent)) {
            progress += "=";
        } else {
            progress += " ";
        }
    }
    cout << "[" << progress << "] " << (static_cast<int>(100 * percent)) << "%";
    cout << " \t\t Simulation - " << fenditureN << " Fenditures";
    
    flush(cout); // Required.
}