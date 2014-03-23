//
//  ThreadedResultSimulatorGeneric.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 22/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ThreadedResultSimulatorGeneric.h"

using namespace std;

/* ------------------------   Init Functions ---------------------- */
ThreadedResultSimulatorGeneric::ThreadedResultSimulatorGeneric(DataSet * _experimentalData, int fenditure, int _simulationsN, int _nThreads)
{
    num_threads = _nThreads;
    simulationsN = _simulationsN;
    experimentalData = _experimentalData;
    
    fenditureN = fenditure;
    SetupArrayResults();
    SetupRandomNumberGenerator();
    lastId = -1;
}

ThreadedResultSimulatorGeneric::~ThreadedResultSimulatorGeneric()
{
    simulators.clear();
    delete[] results;
}

void ThreadedResultSimulatorGeneric::SetupArrayResults()
{
    simDataN = 3*fenditureN;
    results = new double [(simulationsN+num_threads)*(simDataN+resultsN)]; // longer so we just do some more simulations and there is no overflow risk.
}

void ThreadedResultSimulatorGeneric::SetupRandomNumberGenerator()
{
	rng.seed(5);
    
    posRangeDistribution = new uniform_real_distribution<double> (-f1PosRange, f1PosRange);
    intensityDistribution = new uniform_real_distribution<double> (0.0, 100.0 );
    apertureDistribution = new uniform_real_distribution<double> (aperture-4, aperture+4 );
}


void ThreadedResultSimulatorGeneric::Simulate()
{
    int cyclesN = ceil(double(simulationsN)/double(num_threads));
    double lastPercent = 0.0;
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
            results[(simulators[i]->uniqueID)*(simDataN+resultsN)+2] = simulators[i]->newError;
            delete simulators[i];
        }
        simulators.clear();
        double percent = double(cycle)/double(cyclesN);
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

ExperimentSimulator * ThreadedResultSimulatorGeneric::CreateSim()
{
    double variables[simDataN];
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
        results[lastId*(simDataN+resultsN) +(resultsN+i)] = variables[i];
      //  cout << i<<"= "<<variables[i] << " \t ";
    }
    //cout << endl;
    
    ExperimentSimulator * sim = new ExperimentSimulator(experimentalData);
    sim->uniqueID = lastId;
    sim->Setup(fenditureN, variables, 1);
    
    return sim;
}

void ThreadedResultSimulatorGeneric::PrintSingleSimulation(int id, ostream& myout)
{
    double variables[fenditureN*3];
    for (int i = 0; i < simDataN; i++)
    {
        variables[i] = results[id*(simDataN+resultsN) +(resultsN+i)];
    }
    
    
    myout << "Single Simulation with parameters for id "<< id<<endl;    
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
    sim->uniqueID = id;
    sim->Setup(fenditureN, variables, 1);
    sim->Work();
    sim->PrintSimulatedDataToFile();
    
    delete sim;
}

void ThreadedResultSimulatorGeneric::Print(ostream& myout)
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

void ThreadedResultSimulatorGeneric::PrintEvaluation(ostream& myout)
{
    double minErr = results[0];
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
    myout << "the minimal difference for "<< fenditureN << " fenditures is: "<<minErr << endl;
    PrintSingleSimulation(minIndex);
    
    minError = minErr;
}

/*void ThreadedResultSimulatorGeneric::PrintTopEvaluation(ostream& myout)
{
    myout << "-----------------------------------" << endl;
    double minErr = results[1];
    int minIndex = 0;
    for (int i = 0; i < lastId; ++i)
    {
        if (minErr > results[i*(simDataN+resultsN)+1] )
        {
            minErr = results[i*(simDataN+resultsN)+1];
            minIndex = i;
        }
    }
    minTopError = minErr;
    
    myout << "the minimal difference for "<< fenditureN << " fenditures on the top is: "<<minErr << endl;
    PrintSingleSimulation(minIndex);
}*/

void ThreadedResultSimulatorGeneric::PrintNewEvaluation(ostream& myout)
{
    myout << "-----------------------------------" << endl;
    double minErr = results[1];
    int minIndex = 0;
    for (int i = 0; i < lastId; ++i)
    {
        if (minErr > results[i*(simDataN+resultsN)+1] )
        {
            minErr = results[i*(simDataN+resultsN)+1];
            minIndex = i;
        }
    }
    minNewError = minErr;
    
    myout << "the minimal difference for "<< fenditureN << " fenditures with the new eval. is: "<<minErr << endl;
    PrintSingleSimulation(minIndex);
}

void ThreadedResultSimulatorGeneric::DrawProgressBar(int len, double percent)
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