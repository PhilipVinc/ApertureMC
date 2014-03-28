//
//  main.cpp
//  elaboratoreCoseni
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
#include <cmath>

#include "DataSet.h"
#include "CalculatorMaxMin.h"
#include "CalculatorMax.h"
#include "CalculatorMaxSimple.h"
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
    
    string nuoviDatiPath = inputName + ".dat";
	string linearizzatiPath = inputName + "-lineari.dat";
    string plotPath = "plot-" + inputName + ".gnu";
    
    DataSet * data = new DataSet(inputPath);
    //data->ComputeSplineCoefficients();
    
    // Rumuove il Fondo
    DataSet * dataFondo = new DataSet("fondo.xls");
    TransformerSimple::ShiftY(data, -dataFondo->MeanY());
	
	//Find the center and shift
    CalculatorMaxSimple * cMax = new CalculatorMaxSimple(data);
	TransformerSimple::ScaleY(data, 1.0/cMax->GetMaxYPosition());
	TransformerSimple::ShiftX(data, -cMax->GetMaxXPosition());
	delete cMax;
	TransformerSimple::ScaleX(data, M_PI/180.0);
	
	//Stampa i dati magicati
    ofstream datFile(nuoviDatiPath);
	datFile << "# Theta (rad) \t I/I0" << endl;
    data->PrintData(datFile);
    datFile.close();
	
	TransformerSimple::ScaleXCos2(data);
	
	//Stampa i dati linearizzati
    ofstream linFile(linearizzatiPath);
	linFile << "# Cos(theta)^2 \t I/I0" << endl;
    data->PrintData(linFile);
    linFile.close();
}

int main(int argc, char * argv[])
{
	int opt;
    string inputPath;
    bool addXls = false;
	bool automate = false;
    GlobalSettings::get_instance().maxMinSearchSpan = 3;
	
	// Handle command line options
    while((opt = getopt(argc, argv, "as:dxh?")) != -1)
    {
        switch (opt)
        {
            case 'a':
				automate = true;
                break;
			case 'd':
				addXls = true;
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
    std::cerr << "[-p] se si vuole la barra di avanzamento/progresso" << std::endl;
    std::cerr << "[-s int] MaxMin Search Span. Default= "<< GlobalSettings::get_instance().maxMinSearchSpan << std::endl;
    std::cerr << "[-a string] Automatizza il funzionamento leggendo i file da elaborare da un altro file" << std::endl;
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
