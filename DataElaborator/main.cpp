//
//  main.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <libgen.h>
#include <unistd.h>

#include "DataSet.h"
#include "CalculatorMaxMin.h"
#include "CalculatorMax.h"
#include "TransformerSimple.h"

#include "ResultSimulatorFissures.h"
#include "GlobalSettings.h"

void usage(const char * pname);

using namespace std;

int MAX_FEND = 2;
int MIN_FEND = 2;
int SIM_PER_FEND = 100000;
int NUM_THREADS = 7;
int DEFAULT_SEED = 5;

void ElaborateFile(string inputName, bool addXls = false)
{
    string inputPath = inputName;
    string fondoPath = "fondo";
    if (addXls)
    {
        inputPath = inputName + ".xls";
        fondoPath = fondoPath + ".xls";
    }
    
    string nuoviDatiPath = inputName + ".dat";
    
    DataSet * data = new DataSet(inputPath);
    data->ComputeSplineCoefficients();
    
    //Find the center and shift
    CalculatorMax * cMax = new CalculatorMax(data);
    TransformerSimple::ShiftX(data, -cMax->GetMaxXPosition());
    
    // Rumuove il Fondo
    DataSet * dataFondo = new DataSet("fondo.xls");
    TransformerSimple::ShiftY(data, -dataFondo->MeanY());
    TransformerSimple::ScaleY(data, 1.0/cMax->GetMaxYPosition());
    TransformerSimple::ScaleX(data, 1.0/1000.0);
    data->ComputeSplineCoefficients();
    delete cMax;
    
    //Stampa i dati magicati
    ofstream datFile(nuoviDatiPath);
    data->PrintData(datFile);
    datFile.close();

    
    vector<ResultSimulatorFissures*> sims;
    
    for (int i = 0; i < (MAX_FEND-MIN_FEND+1); i++)
    {
		GlobalSettings::get_instance().currentIteration = i+1;
        sims.push_back( new ResultSimulatorFissures(data, i+MIN_FEND, SIM_PER_FEND, NUM_THREADS));
        sims[i]->Simulate();
        ofstream simFile("Simulation-f"+to_string(i+MIN_FEND));
        sims[i]->Print(simFile);
        simFile.close();
    }
    
    long double minErr = 10000.0;
    int minErrIndex= 0;
    for (int i = 0; i < (MAX_FEND-MIN_FEND+1); i++)
    {
        if (minErr > sims[i]->bestLikelihoods[0])
        {
            minErr = sims[i]->bestLikelihoods[0];
            minErrIndex = i;
        }
    }
    
    long double minNewErr = 10000.0;
    int minNewErrIndex= 0;
    for (int i = 0; i < (MAX_FEND-MIN_FEND+1); i++)
    {
        if (minNewErr > sims[i]->bestLikelihoods[2])
        {
            minNewErr = sims[i]->bestLikelihoods[2];
            minNewErrIndex = i;
        }
    }
    
    long double minTopErr = 10000.0;
    int minTopErrIndex= 0;
    for (int i = 0; i < (MAX_FEND-MIN_FEND+1); i++)
    {
        if (minTopErr > sims[i]->bestLikelihoods[1])
        {
            minTopErr = sims[i]->bestLikelihoods[1];
            minTopErrIndex = i;
        }
    }
    
    cout << "----------------------------------------" << endl;
    cout << "The global best guess is for " << minErrIndex+1<< " fenditures" << endl;
    cout << "The global best guess WITH NEW is for " << minNewErrIndex+1<< " fenditures" << endl;
    cout << "The global best guess WITH TOP is for " << minTopErrIndex+1<< " fenditures" << endl;
    sims[minErrIndex]->PrintLikelihoodEvaluation(cout, 0);
    sims[minTopErrIndex]->PrintLikelihoodEvaluation(cout, 1);
    sims[minNewErrIndex]->PrintLikelihoodEvaluation(cout, 2);

    for (int i = 0; i < (MAX_FEND-MIN_FEND+1); i++)
    {
        delete sims[i];
    }
    sims.clear();
}


int main(int argc, char * argv[])
{
	int opt;
    string inputPath;
    bool addXls = false;
	bool ideLaunch = false;
	GlobalSettings::get_instance().f1Range = 2.5;
    GlobalSettings::get_instance().maxMinSearchSpan = 3;
    GlobalSettings::get_instance().showProgress = true;

	// Handle command line options
    while((opt = getopt(argc, argv, "if:n:m:j:s:pr:dxh?")) != -1)
    {
        switch (opt)
        {
            case 'i':
				//NUM_THREADS = 1;
				break;
				inputPath = "data";
				addXls = true;
				ideLaunch = true;
                break;
            case 'x':
                addXls=true;
                break;
            case 'f':
                MAX_FEND = stoi(optarg);
                break;
            case 'm':
                MIN_FEND = stoi(optarg);
                break;
            case 'n':
                SIM_PER_FEND = stoi(optarg);
                break;
            case 'j':
                NUM_THREADS = stoi(optarg);
                break;
            case 'p':
                GlobalSettings::get_instance().showProgress = true;
                break;
			case 'r':
                GlobalSettings::get_instance().f1Range = stof(optarg);
                break;
			case 's':
                DEFAULT_SEED = stoi(optarg);
                break;
            case 'h':
            case '?':
            default:
                usage( basename( argv[0] ) );
                return 1;
        }
    }
    GlobalSettings::get_instance().max_fend= MAX_FEND;
    GlobalSettings::get_instance().min_fend= MIN_FEND;
	GlobalSettings::get_instance().seed = DEFAULT_SEED;
	if (!ideLaunch)
	{
		cout << "Inserire nome file [se si usa -x, senza estensione]: "; cin >> inputPath;
	}
    ElaborateFile(inputPath, addXls);
	return 0;
}


void usage(const char * pname)
{
    std::cerr << "Usage: " << pname << std::endl;
    std::cerr << "[-x] se il nome del file contiene \".xls\"" << std::endl;
    std::cerr << "[-p] se si vuole la barra di avanzamento/progresso" << std::endl;
    std::cerr << "[-n int] per il numero di Iterazioni per gruppo di fenditure. Default: " << SIM_PER_FEND << std::endl;
    std::cerr << "[-j int] numero di job paralleli (suggerito: threads -1). Default: "<< NUM_THREADS <<std::endl;
    std::cerr << "[-m int] numero di fenditure minime da simulare. Default: "<< MIN_FEND << std::endl;
    std::cerr << "[-f int] numero di fenditure massime da simulare. Default: "<< MAX_FEND << std::endl;
	std::cerr << "[-s int] Seed. Default: "<< DEFAULT_SEED << std::endl;
}
