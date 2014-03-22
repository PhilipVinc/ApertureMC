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
	std::uniform_real_distribution<double>* f1PosRangeDistribution;
    std::uniform_real_distribution<double>* f2DistanceDistribution;
    std::uniform_real_distribution<double>* apertureDistribution;
    
    /* ------------------------  Simulation setup data ---------------------- */
    double f1PosRange = 0.2;
    double f2DistRange = 0.5;
    double aperture = 5;
    int fenditureN=2;
    
    void SetupArrayResults();
    double * results;
    int lastId;

};

#endif /* defined(__DataElaborator__ThreadedResultSimulator__) */
