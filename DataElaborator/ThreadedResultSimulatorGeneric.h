//
//  ThreadedResultSimulatorGeneric.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 22/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ThreadedResultSimulatorGeneric__
#define __DataElaborator__ThreadedResultSimulatorGeneric__

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <random>

#include "ExperimentSimulator.h"

class ThreadedResultSimulatorGeneric
{
public:
    /* ------------------------   Init Functions ---------------------- */
    ThreadedResultSimulatorGeneric(DataSet * _experimentalData,int fenditure, int iterations, int nThreads = 1);
    ~ThreadedResultSimulatorGeneric();
    
    void Simulate();
    void Print(std::ostream& myout);
    void PrintEvaluation(std::ostream& myout);
    //void PrintTopEvaluation(std::ostream& myout);
    void PrintNewEvaluation(std::ostream& myout);
    
    double minError;
    //double minTopError;
    double minNewError;
    
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
	std::uniform_real_distribution<double>* posRangeDistribution;
    std::uniform_real_distribution<double>* intensityDistribution;
    std::uniform_real_distribution<double>* apertureDistribution;
    
    int dataPerFend = 3;
    int simDataN = 4;
    int resultsN = 2;
    
    /* ------------------------  Simulation setup data ---------------------- */
    double f1PosRange = 0.2;
    double aperture = 7;
    int fenditureN=2;
    
    void SetupArrayResults();
    double * results;
    int lastId;
    
    void DrawProgressBar(int len, double percent);
};


#endif /* defined(__DataElaborator__ThreadedResultSimulatorGeneric__) */
