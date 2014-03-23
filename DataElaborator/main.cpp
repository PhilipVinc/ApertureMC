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
#include <unistd.h>
#include <vector>

#include "DataSet.h"
#include "CalculatorMaxMin.h"
#include "CalculatorMax.h"
#include "TransformerSimple.h"

#include "ThreadedResultSimulatorGeneric2.h"
#include "GlobalSettings.h"

void usage(const char * pname);

using namespace std;

int MAX_FEND = 6;
int MIN_FEND = 1;
int SIM_PER_FEND = 20;
int NUM_THREADS = 1;

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
    TransformerSimple::ScaleX(data, 1.0/1500.0);
    data->ComputeSplineCoefficients();
    delete cMax;
    
    //Stampa i dati magicati
    ofstream datFile(nuoviDatiPath);
    data->PrintData(datFile);
    datFile.close();

    
    vector<ThreadedResultSimulatorGeneric2*> sims;
    
    for (int i = 0; i < (MAX_FEND-MIN_FEND+1); i++)
    {
        sims.push_back( new ThreadedResultSimulatorGeneric2(data, i+MIN_FEND, SIM_PER_FEND, NUM_THREADS));
        sims[i]->Simulate();
        ofstream simFile("Simulation-f"+to_string(i+MIN_FEND));
        sims[i]->Print(simFile);
        simFile.close();
    }
    
    long double minErr = 10000.0;
    int minErrIndex= 0;
    for (int i = 0; i < (MAX_FEND-MIN_FEND+1); i++)
    {
        if (minErr > sims[i]->minError)
        {
            minErr = sims[i]->minError;
            minErrIndex = i;
        }
    }
    
    long double minNewErr = 10000.0;
    int minNewErrIndex= 0;
    for (int i = 0; i < (MAX_FEND-MIN_FEND+1); i++)
    {
        if (minNewErr > sims[i]->minNewError)
        {
            minNewErr = sims[i]->minNewError;
            minNewErrIndex = i;
        }
    }
    
    cout << "----------------------------------------" << endl;
    cout << "The global best guess is for " << minErrIndex+1<< " fenditures" << endl;
    cout << "The global best guess WITH NEW is for " << minNewErrIndex+1<< " fenditures" << endl;
    sims[minErrIndex]->PrintEvaluation(cout);
    sims[minNewErrIndex]->PrintNewEvaluation(cout);
    
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
    GlobalSettings::get_instance().maxMinSearchSpan = 4;
	// Handle command line options
    while((opt = getopt(argc, argv, "i:f:n:m:j:dh?")) != -1)
    {
        switch (opt)
        {
            case 'i':
                //inputFile = optarg;
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
            
            case 'h':
            case '?':
            default:
                usage( basename( argv[0] ) );
                return 1;
        }
    }
    addXls=true;
    //cout << "Inserire nome file [se si usa -x, senza estensione]: "; cin >> inputPath;
    inputPath = "data";
    ElaborateFile(inputPath, addXls);
	return 0;
}


void usage(const char * pname)
{
    std::cerr << "Usage: " << pname << " [-x] se il file è xls" << std::endl;
    std::cerr << " Il programma funziona ed è figo! In particolare, prende in input il nome del file" << std::endl;
    std::cerr << ", lo elabora, e ti sputa fuori altri file con lo stesso nome + altre terminazioni." << std::endl;
    std::cerr << "es: se vuoi elaborare cicciobello.xls, esegui il programma con l'opzione -x e poi dagli il nome"<<std::endl;
    std::cerr << "cicciobello come nome file da elaborare " << std::endl;
}
