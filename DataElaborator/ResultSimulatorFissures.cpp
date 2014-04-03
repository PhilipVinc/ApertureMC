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
    bestResults.clear(); //delete[] bestResults;
    bestLikelihoods.clear(); //delete[] bestErrors;
}

void ResultSimulatorFissures::SetupArrayResults()
{
    simDataN = dataPerFend*fenditureN +1+1;
    //results = new long double [(simulationsN+num_threads)*(simDataN+likelihoodsN)]; // longer so we just do some more simulations and there is no overflow risk.
    //cycleResults = new long double [(num_threads)*(simDataN+likelihoodsN)];
    for (int i = 0; i !=(num_threads)*(simDataN+likelihoodsN); i++) {
        cycleResults.push_back(10.0);
    }

    //bestResults = new long double [likelihoodsN*(simDataN+likelihoodsN)];
    for (int i = 0; i !=likelihoodsN*(simDataN+likelihoodsN); i++) {
        bestResults.push_back(10.0);
    }
    //bestLikelihoods = new long double [likelihoodsN];
    //for (int i = 0; i < likelihoodsN; i++) { bestLikelihoods[i] = 10000000; }
    for (int i = 0; i != likelihoodsN; i++) {
        bestLikelihoods.push_back(10000000);
    }
}

void ResultSimulatorFissures::SetupRandomNumberGenerator()
{
	rng.seed(GlobalSettings::get_instance().seed);
	f1PosRange = GlobalSettings::get_instance().f1Range;
    
    posRangeDistribution = new uniform_real_distribution<long double> (-f1PosRange, f1PosRange);
    intensityDistribution = new uniform_real_distribution<long double> (0.0, 100.0 );
    apertureDistribution = new uniform_real_distribution<long double> (0.0, aperture+4 );
	constantIntensityDistribution = new uniform_real_distribution<long double>(0.0, 20.0*double(fenditureN));
}

void ResultSimulatorFissures::CheckBestSim()
{
    for (int i = 0; i < likelihoodsN ; i++)
    {
        double minErr = cycleResults[i];
        int minIndex = 0;
        for (int j=0; j < num_threads; j++)
        {
            if (minErr > cycleResults[j*(simDataN+likelihoodsN)+i])
            {
                minIndex = j;
                minErr = cycleResults[j*(simDataN+likelihoodsN)+i];
            }
        }
        if (minErr < bestLikelihoods[i])
        {
            bestLikelihoods[i] = minErr;
            for (int j = 0; j < simDataN+likelihoodsN; j++)
            {
                bestResults[(simDataN+likelihoodsN)*i +j] = cycleResults[(minIndex)*(simDataN+likelihoodsN)+j];
            }
        }
    }
}

void ResultSimulatorFissures::OnPostSimulation()
{
    std::cout << std::endl;
    PrintAllLikelihoodEvaluations(std::cout);
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
	//variables[fenditureN*dataPerFend]= (*constantIntensityDistribution)(rng);
	variables[fenditureN*dataPerFend]= 0.0;
    variables[1] = 100.0;
    
    for (int i = 0; i < simDataN; i++)
    {
        cycleResults[threadN*(simDataN+likelihoodsN) +(likelihoodsN+i)] = variables[i];
    }
    
    ExperimentSimulatorFissures * sim = new ExperimentSimulatorFissures(experimentalData);
    sim->uniqueID = lastId;
    sim->Setup(fenditureN, variables, sim_range);
    
    return sim;
}

void ResultSimulatorFissures::PrintSingleSimulation(int bestId, ostream& myout)
{
    long double variables[simDataN];
    for (int i = 0; i < simDataN; i++)
    {
        variables[i] = bestResults[bestId*(simDataN+likelihoodsN) +(likelihoodsN+i)];
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
	cout << "I_0 cost = "<< variables[fenditureN*dataPerFend]<<endl;
    
    cout << endl;
    
    ExperimentSimulatorFissures * sim = new ExperimentSimulatorFissures(experimentalData);
    sim->uniqueID = variables[simDataN-1];
    sim->Setup(fenditureN, variables, sim_range);
    sim->Work();
    sim->PrintSimulatedDataToFile();
	sim->PrintSimulationFourierSpace();
    
    delete sim;
}

void ResultSimulatorFissures::Print(ostream& myout)
{
	for (int i = 0; i < likelihoodsN; ++i)
	{
        myout << i << " \t ";
        for (int j = 0; j != simDataN+likelihoodsN; j++) {
            myout << bestResults[i*(simDataN+likelihoodsN)+j] << " \t ";
        }
        myout << endl;
	}
	myout << endl;
}

void ResultSimulatorFissures::PrintLikelihoodEvaluation(ostream& myout, int likelihoodId)
{
    string likelihoodName;
    if (likelihoodId == 0) likelihoodName = "Data-Point-Evaluated Likelihood";
    else if (likelihoodId == 1) likelihoodName = "Small-Interval-around-origin Likelihood";
    else if (likelihoodId == 2) likelihoodName = "Spline-Evaluated Likelihood";
    else likelihoodName = "Unknown Likelihood";
    
    myout << "the maximum likelihood for "<< fenditureN << " fenditures with " << likelihoodName <<" is: "<<bestLikelihoods[likelihoodId] << endl;
    PrintSingleSimulation(likelihoodId);
}

void ResultSimulatorFissures::PrintAllLikelihoodEvaluations(ostream &myout)
{
    for (int i = 0; i < likelihoodsN; i++)
    {
        PrintLikelihoodEvaluation(myout, i);
    }
}