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

#include "DataSet.h"
#include "CalculatorMaxMin.h"
#include "CalculatorMax.h"
#include "TransformerSimple.h"

#include "ThreadedResultSimulator.h"

void usage(const char * pname);

using namespace std;

void ElaborateFile(string inputName, bool addXls = false)
{
    string inputPath = inputName;
    string fondoPath = "fondo";
    if (addXls)
    {
        inputPath = inputName + ".xls";
        fondoPath = fondoPath + ".xls";
    }
    
    string splinePath = inputName + "-spline.dat";
    string nuoviDatiPath = inputName + ".dat";
    string maxPath = inputName + "-max.dat";
    string minPath = inputName + "-min.dat";
    string plotPath = "plot-" + inputName + ".gnu";
    
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
    
    // Trovo gli estremi
    CalculatorMaxMin * extremerer = new CalculatorMaxMin(data);
    extremerer->Elaborate();
    
    //Stampa i dati magicati
    ofstream datFile(nuoviDatiPath);
    data->PrintData(datFile);
    datFile.close();
    
    // Stampa la spline
    ofstream splineFile(splinePath);
    data->PrintSplineWithDerivate1(splineFile);
    splineFile.close();
    
    // Stampo i massimi
    ofstream maxFile(maxPath);
    extremerer->PrintData(maxFile);
    maxFile.close();
    
    // Stampo i Minimi
    ofstream minFile(minPath);
    extremerer->PrintData(minFile);
    minFile.close();
    
    // Stampo i Comandi gnuplot
    ofstream plotFile(plotPath);
    plotFile << "#Gnuplot command:" << endl;
    plotFile << "plot \""<<splinePath<<"\" u 1:2 w l lc rgb \"red\", \""<<splinePath<<"\" u 1:3 w l lc rgb \"blue\", \"";
    plotFile << nuoviDatiPath<<"\" w p lc rgb \"black\", \"";
    plotFile << maxPath << "\" w p lc rgb \"blue\", \"";
    plotFile << minPath << "\" w p lc rgb \"blue\" " << endl;
    plotFile.close();
    
    // Stampo a schermo
    cout << "#Gnuplot command:" << endl;
    cout << "plot \""<<splinePath<<"\" u 1:2 w l lc rgb \"red\", \""<<splinePath<<"\" u 1:3 w l lc rgb \"blue\", \"";
    cout << nuoviDatiPath<<"\" w p lc rgb \"black\", \"";
    cout << maxPath << "\" w p lc rgb \"blue\", \"";
    cout << minPath << "\" w p lc rgb \"blue\" " << endl;
    
    
    
    ThreadedResultSimulator * sim = new ThreadedResultSimulator(data, 3);
    sim->Simulate();
    ofstream pincoFIle("FIGO");
    sim->Print(pincoFIle);
    pincoFIle.close();
    delete extremerer;
    
}


int main(int argc, char * argv[])
{
	int opt;
    string inputPath;
    bool addXls = false;
	// Handle command line options
    while((opt = getopt(argc, argv, "ixr:m:e:dh?")) != -1)
    {
        switch (opt)
        {
            case 'i':
                //inputFile = optarg;
                break;
            case 'x':
                addXls=true;
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
    inputPath = "data2";
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
