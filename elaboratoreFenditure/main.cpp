//
//  main.cpp
//  elaboratoreFenditure
//
//  Created by Filippo Vicentini on 23/03/14.
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

#include "GlobalSettings.h"

void usage(const char * pname);
void Automate(std::string automateFilePath, bool addXls);

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
	
	// Riscalo i dati
    TransformerSimple::ScaleY(data, 1.0/cMax->GetMaxYPosition());
	TransformerSimple::ScaleX(data, 1.9/100000);
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
    extremerer->PrintMaxPoints(maxFile);
    maxFile.close();
    
    // Stampo i Minimi
    ofstream minFile(minPath);
    extremerer->PrintMinPoints(minFile);
    minFile.close();
    delete extremerer;

    // Stampo i Comandi gnuplot
    ofstream plotFile(plotPath);
    plotFile << "#Gnuplot command:" << endl;
	plotFile << "set terminal x11" << endl;
	plotFile << "set title \"Grafico per " << inputName << "\"" << endl;
	plotFile << "set xlabel \"Angolo (mStep)\"" << endl;
	plotFile << "set ylabel \"Intensità Relativa I\\I0\"" << endl;
    plotFile << "plot \""<<splinePath<<"\" u 1:2 w l lc rgb \"red\", \"";
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
    
}


int main(int argc, char * argv[])
{
	int opt;
    string inputPath;
    bool addXls = false;
	bool automate = false;
    GlobalSettings::get_instance().maxMinSearchSpan = 3;

	// Handle command line options
    while((opt = getopt(argc, argv, "af:n:m:j:s:dxh?")) != -1)
    {
        switch (opt)
        {
            case 'a':
				automate = true;
                break;
            case 'x':
                addXls=true;
                break;
            case 's':
                GlobalSettings::get_instance().maxMinSearchSpan = stoi(optarg);
                break;
            case 'h':
            case '?':
            default:
                usage( basename( argv[0] ) );
                return 1;
        }
    }
	if(automate)
	{
		cout << "Inserire nome file con i nomi degli altri file: "; cin >> inputPath;
		Automate(inputPath, addXls);
	}
    else
	{
		cout << "Inserire nome file [se si usa -x, senza estensione]: "; cin >> inputPath;
		ElaborateFile(inputPath, addXls);

	}
	
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

void Automate(string automateFilePath, bool addXls)
{
	int lines=0;
    {
        string line;
        ifstream inputFile(automateFilePath);
        if(inputFile.is_open()){
            while(!inputFile.eof()){
                getline(inputFile,line);
                lines++;
            }
            inputFile.close();
        }
        lines--;
    }
	string filePath;
	int span;
    ifstream inputFile(automateFilePath);
    {
        for (int i = 0; i < lines; ++i)
        {
			inputFile >> filePath;
			inputFile >> span;
			cout << "---------Elaborating File " << filePath << " with maxmin search span of " << span << " ---------"<<endl;
			GlobalSettings::get_instance().maxMinSearchSpan = span;
			ElaborateFile(filePath, addXls);
        }
    }
	cout << "---------Done!-----------" << endl;

}
