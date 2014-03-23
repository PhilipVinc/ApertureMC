//
//  ThreadedResultSimulator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 20/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ThreadedResultSimulator__
#define __DataElaborator__ThreadedResultSimulator__

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <random>

#include "ExperimentSimulator.h"

class ThreadedResultSimulator
{
public:
    /* ------------------------   Init Functions ---------------------- */
    ThreadedResultSimulator(DataSet * _experimentalData, int iterations, int nThreads = 1);
    ~ThreadedResultSimulator();
    
    void Simulate();
    void Print(std::ostream& myout);
    void PrintEvaluation(std::ostream& myout);
    
    void Start();

protected:
    ExperimentSimulator * CreateSim();
    void SetupRandomNumberGenerator();
    void PrintSingleSimulation(int id, std::ostream& myout = std::cout);
    
    int num_threads;
    int simulationsN;
    std::thread * threads;
    
    DataSet * experimentalData;
    std::vector<ExperimentSimulator*> simulators;
    
    std::mt19937 rng;
	std::uniform_real_distribution<long double>* f1PosRangeDistribution;
    std::uniform_real_distribution<long double>* f2DistanceDistribution;
    std::uniform_real_distribution<long double>* f1apertureDistribution;
    std::uniform_real_distribution<long double>* f2apertureDistribution;
    
    int simDataN = 4;
    int resultsN = 2;
    
    /* ------------------------  Simulation setup data ---------------------- */
    long double f1PosRange = 0.02;
    long double f2DistRange = 0.5;
    long double aperture = 7;
    int fenditureN=2;
    
    void SetupArrayResults();
    long double * results;
    int lastId;

};

#endif /* defined(__DataElaborator__ThreadedResultSimulator__) */
