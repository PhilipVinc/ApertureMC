//
//  ThreadedResultSimulatorGeneric2.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ThreadedResultSimulatorGeneric2__
#define __DataElaborator__ThreadedResultSimulatorGeneric2__

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <random>

#include "ThreadedResultSimulatorBase.h"
#include "ExperimentSimulator.h"
#include "GlobalSettings.h"

class ThreadedResultSimulatorGeneric2 : public ThreadedResultSimulatorBase
{
public:
    /* ------------------------   Init Functions ---------------------- */
    ThreadedResultSimulatorGeneric2(DataSet * _experimentalData,int fenditure, int iterations, int nThreads = 1);
    ~ThreadedResultSimulatorGeneric2();
    
    void Simulate();
    void Print(std::ostream& myout);
    void PrintEvaluation(std::ostream& myout);
    void PrintNewEvaluation(std::ostream& myout);
    void PrintTopEvaluation(std::ostream& myout);
    
    long double minError;
    long double minNewError;
    long double minTopError;
    std::vector<long double> bestErrors;//long double * bestErrors;
    
protected:
    ExperimentSimulator * CreateSim(int threadN);
    void SetupRandomNumberGenerator();
    void PrintSingleSimulation(int bestId, std::ostream& myout = std::cout);
    
    void CheckBestSim();
        
    std::mt19937 rng;
	std::uniform_real_distribution<long double>* posRangeDistribution;
    std::uniform_real_distribution<long double>* intensityDistribution;
    std::uniform_real_distribution<long double>* apertureDistribution;
    
    int dataPerFend = 3;
    int simDataN;
    int resultsN = 3;
    
    /* ------------------------  Simulation setup data ---------------------- */
    long double f1PosRange = 0.5;
    long double aperture = 14;
    int fenditureN=2;
    
    void SetupArrayResults();
    std::vector <long double> cycleResults;//long double * cycleResults;
    std::vector <long double> bestResults;//long double * bestResults;
};


#endif /* defined(__DataElaborator__ThreadedResultSimulatorGeneric2__) */
