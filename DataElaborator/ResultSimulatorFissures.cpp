//
//  ThreadedResultSimulatorGeneric2.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ResultSimulatorFissures.h"
#include "WorkerThread.h"
using namespace std;

/* ------------------------   Init Functions ---------------------- */
ResultSimulatorFissures::ResultSimulatorFissures(DataSet * _experimentalData, int fenditure, int _simulationsN, int _nThreads) : ThreadedResultSimulator(_experimentalData, _simulationsN, _nThreads)
{
    fenditureN = fenditure;
    SetupArrayResults();
    SetupRandomNumberGenerator();
}

ResultSimulatorFissures::~ResultSimulatorFissures()
{
    cycleResults.clear(); //delete[] cycleResults;
    bestResults.clear(); //delete[] bestResults;
    bestErrors.clear(); //delete[] bestErrors;
}

void ResultSimulatorFissures::SetupArrayResults()
{
    simDataN = dataPerFend*fenditureN +1;
    //results = new long double [(simulationsN+num_threads)*(simDataN+resultsN)]; // longer so we just do some more simulations and there is no overflow risk.
    //cycleResults = new long double [(num_threads)*(simDataN+resultsN)];
    for (int i = 0; i !=(num_threads)*(simDataN+resultsN); i++) {
        cycleResults.push_back(10.0);
    }

    //bestResults = new long double [resultsN*(simDataN+resultsN)];
    for (int i = 0; i !=resultsN*(simDataN+resultsN); i++) {
        bestResults.push_back(10.0);
    }
    //bestErrors = new long double [resultsN];
    for (int i = 0; i != resultsN; i++) {
        bestErrors.push_back(10000000);
    }
    for (int i = 0; i < resultsN; i++) { bestErrors[i] = 10000000; }
    minNewError = 1000000;
    minError = 1000000;
}

void ResultSimulatorFissures::SetupRandomNumberGenerator()
{
	rng.seed(5);
    
    posRangeDistribution = new uniform_real_distribution<long double> (-f1PosRange, f1PosRange);
    intensityDistribution = new uniform_real_distribution<long double> (0.0, 100.0 );
    apertureDistribution = new uniform_real_distribution<long double> (0.0, aperture+4 );
}

void ResultSimulatorFissures::Simulate()
{
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
            cycleResults[(i)*(simDataN+resultsN)] = simulators[i]->GetError(0);
            cycleResults[(i)*(simDataN+resultsN)+1] = simulators[i]->GetError(1);
            cycleResults[(i)*(simDataN+resultsN)+2] = simulators[i]->GetError(2);
            delete simulators[i];
        }
        
        simulators.clear();
        CheckBestSim();
        
        long double percent = double(cycle)/double(cyclesN);
        if (settings->showProgress)
        {
            if ( percent > lastPercent )
            {
                settings->DrawSimulationProgressBar(40, percent, fenditureN);
                lastPercent = percent + 0.01;
            }
        }
    }
    for (int i = 0; i<num_threads; i++)
    {
        workers[i]->Terminate();
    }

    std::cout << std::endl;
    PrintEvaluation(std::cout);
    PrintTopEvaluation(std::cout);
    PrintNewEvaluation(std::cout);
}

void ResultSimulatorFissures::CheckBestSim()
{
    for (int i = 0; i < resultsN ; i++)
    {
        double minErr = cycleResults[i];
        int minIndex = 0;
        for (int j=0; j < num_threads; j++)
        {
            if (minErr > cycleResults[j*(simDataN+resultsN)+i])
            {
                minIndex = j;
                minErr = cycleResults[j*(simDataN+resultsN)+i];
            }
        }
        if (minErr < bestErrors[i])
        {
            bestErrors[i] = minErr;
            for (int j = 0; j < simDataN+resultsN; j++)
            {
                bestResults[(simDataN+resultsN)*i +j] = cycleResults[(minIndex)*(simDataN+resultsN)+j];
            }
        }
    }
    minError = bestErrors[0];
    minTopError = bestErrors[1];
    minNewError = bestErrors[2];
}

ExperimentSimulator * ResultSimulatorFissures::CreateSim(int threadN)
{
    lastId++;
    long double variables[simDataN];
    variables[simDataN-1] = lastId;
    for (int i = 0; i < fenditureN; i++)
    {
        variables[i*dataPerFend] = (*posRangeDistribution)(rng);
        variables[i*dataPerFend + 1 ] = (*intensityDistribution)(rng);
        variables[i*dataPerFend + 2 ] = (*apertureDistribution)(rng);///(i+1);
    }
    variables[1] = 100.0;
    
    for (int i = 0; i < simDataN; i++)
    {
        cycleResults[threadN*(simDataN+resultsN) +(resultsN+i)] = variables[i];
    }
    
    ExperimentSimulator * sim = new ExperimentSimulator(experimentalData);
    sim->uniqueID = lastId;
    sim->Setup(fenditureN, variables, 1);
    
    return sim;
}

void ResultSimulatorFissures::PrintSingleSimulation(int bestId, ostream& myout)
{
    long double variables[simDataN];
    for (int i = 0; i < simDataN; i++)
    {
        variables[i] = bestResults[bestId*(simDataN+resultsN) +(resultsN+i)];
    }
    
    myout << "Single Simulation with parameters for bestId "<< bestId << " with ID" << variables[simDataN-1] << endl;
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
    sim->uniqueID = variables[simDataN-1];
    sim->Setup(fenditureN, variables, 1);
    sim->Work();
    sim->PrintSimulatedDataToFile();
    
    delete sim;
}

void ResultSimulatorFissures::Print(ostream& myout)
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

void ResultSimulatorFissures::PrintEvaluation(ostream& myout)
{
    myout << "-----------------------------------" << endl;
    myout << "the minimal difference for "<< fenditureN << " fenditures is: "<< minError << endl;
    PrintSingleSimulation(0);
}

void ResultSimulatorFissures::PrintTopEvaluation(ostream& myout)
{
    myout << "the minimal difference for "<< fenditureN << " fenditures with the top eval. is: "<<minTopError << endl;
    PrintSingleSimulation(1);
}

void ResultSimulatorFissures::PrintNewEvaluation(ostream& myout)
{
    myout << "the minimal difference for "<< fenditureN << " fenditures with the new eval. is: "<<minNewError << endl;
    PrintSingleSimulation(2);
}