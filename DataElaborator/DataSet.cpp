//
//  DataSet.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "DataSet.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <libgen.h>
#include <unistd.h>
#include <unistd.h>

using namespace std;

const
DataSet::DataSet()
{
	debug = false;
	meanXDirty = false;
	meanYDirty = false;
    n=0;
    
    xp = *new vector<double>();
    yp = *new vector<double>();
    zs = *new vector<double>();
}

DataSet::DataSet(string inputPath, bool enableDebug)
{
	debug = enableDebug;
	meanXDirty = false;
	meanYDirty = false;
    n=0;
    
    xp = *new vector<double>();
    yp = *new vector<double>();
    
	int lines=0;
    {
        string line;
        ifstream inputFile(inputPath);
        if(inputFile.is_open()){
            while(!inputFile.eof()){
                getline(inputFile,line);
                lines++;
            }
            inputFile.close();
        }
        lines--;
    }
    double x, y;
    ifstream inputFile(inputPath);
    {
        for (int i = 0; i < lines; ++i)
        {
            inputFile>> x;
            inputFile>> y;
            AddMeasure(x, y);
            if (debug) debugStream << "xp["<<i<<"]= " << xp[i]<<"\t";
            if (debug) debugStream << "yp["<<i<<"]= " << yp[i]<<endl;
        }
    }
    OrderAlongXAxis();
}

DataSet::~DataSet()
{
    xp.clear();
    yp.clear();
    zs.clear();
    
    free xp;
}

void DataSet::OrderAlongXAxis()
{
    bool mustReorder = false;
    for (int i = 1; i < n; i++)
    {
        if (xp[i] < xp[i-1])
        {
            mustReorder = true;
            break;
        }
    }
    if (mustReorder)
    {
        for (int j = 0; j != n; j++)
        {
            for (int i = n-1; i != 0; i--)
            {
                if (xp[i] < xp[i-1])
                {
                    double temp = xp[i];
                    xp[i] = xp[i-1];
                    xp[i-1] = temp;
                    temp = yp[i];
                    yp[i] = yp[i-1];
                    yp[i-1] = temp;
                }
            }
        }
    }
}

void DataSet::EraseData()
{
    xp.clear();
    yp.clear();
    zs.clear();
    n=0;
}

/*Measure& DataSet::GetMeasure(int id)
{
	//if (DEBUG)	debugStream << "Accessed Measure with ID= " << id << " : has position= " << pool[id].position << " speed= " << pool[id].intensity << endl;
    Measure newMeasure;
    newMeasure.position = xp[id];
    newMeasure.intensity = yp[id];
    
    return newMeasure;
}*/

int DataSet::AddMeasure(double position, double intensity)
{
	n++;
    xp.push_back(position);
    yp.push_back(intensity);
    zs.push_back(0.0);
	//if (DEBUG)	debugStream << "Added new Measure with ID= " << lastId << " position= " << newMeasure.position << " speed= " << newMeasure.intensity << endl;
    
	return (n-1);
}

void DataSet::RemoveMeasure(int id)
{
	//if (DEBUG)	debugStream << "Destroyed Measure with ID= " << id << " : had position= " << pool[id].position << " speed= " << pool[id].intensity << endl;
    
	xp.erase(xp.begin()+id);
    yp.erase(yp.begin()+id);
}

//--------------- Debug Functions -------------------------
void DataSet::EnableDebug()
{
	debug = true;
	debugStream.open("debugLog-Pool.txt");
}
void DataSet::DisableDebug()
{
	debug = false;
	debugStream.close();
}

//--------------- Compute Functions -------------------------
void DataSet::ComputeSplineCoefficients()
{
	double * h = new double[n];
	double * b = new double[n];
	double * u = new double[n];
	double * v = new double[n];
    
	for (int i = 0; i < n-1; i++)
	{
		h[i] = xp[i+1]-xp[i];
		b[i] = (yp[i+1] - yp[i])/h[i];
	}
    
	u[1] = 2.0 * ( h[0] + h[1] );
	v[1] = 6.0 * ( b[1] - b[0] );
    
	for (int i = 2; i < n-1; i++)
	{
		u[i] = 2.0 * ( h[i] + h[i-1] ) - h[i-1]*h[i-1]/u[i-1];
		v[i] = 6.0 * ( b[i] - b[i-1] ) - h[i-1]*v[i-1]/u[i-1];
	}
    
	zs[n-1]=0.0;
	for (int i = n-2; i > 0 ; i--)
	{
		zs[i] = (v[i] - h[i] * zs[i+1]) / u[i];
		//if (DEBUG)	debugStream << "SplineParameter z["<<i<<"]=" << zs[i] << endl;
	}
	zs[0]=0.0;
    
	delete[] h; delete[] b; delete[] u; delete[] v;
}

//--------------- Value Functions -------------------------
double DataSet::SplineValue(double x)
{
	// Select the interval
	int i = n-2;
	while ( i>0 )
	{
		//if ((( x > xp[i] ) && ( x < xp[i+1] )) || (( x < xp[i] ) && ( x > xp[i+1] )))
 		if  ((x - xp[i]) > 0 )
		{
			break;
		}
		i--;
	}
	double h = xp[i+1] - xp[i];
	double temp = zs[i]/2.0 + (x-xp[i]) * (zs[i+1] - zs[i]) / (6.0*h);
	temp = -(h/6.0) * (zs[i+1] + 2.0*zs[i]) + (yp[i+1] - yp[i]) / h + (x - xp[i])*temp;
	double result = (yp[i] + (x - xp[i]) * temp);
    
	return result;
}

double DataSet::SplineDerivate1(double x)
{
	// Select the interval
	int i = n-2;
	while ( i>0 )
	{
		//if ((( x > xp[i] ) && ( x < xp[i+1] )) || (( x < xp[i] ) && ( x > xp[i+1] )))
 		if  ((x - xp[i]) > 0 )
		{
			break;
		}
		i--;
	}
	double h = xp[i+1] - xp[i];
	double temp = zs[i]+ (x-xp[i]) * (zs[i+1] - zs[i]) / (2.0*h);
	double result = -(h/6.0) * (zs[i+1] + 2.0*zs[i]) + (yp[i+1] - yp[i]) / h + (x - xp[i])*temp;
    
	return result;
}

double DataSet::SplineDerivate1Zero(int index, int recursion)
{
	double a = xp[index-1];
	double b = xp[index+1];
	//cout << "Investigating x-= " << xp[index-1] << "\t x+= " << xp[index+1] << endl;
    
	if( SplineDerivate1(a)*SplineDerivate1(b) < 0.0 )
	{
		double c ;
		double err = 100;
		while( err > 0.0001)
		{
			c = (b+a)/2;
			(SplineDerivate1(a)*SplineDerivate1(c)) < 0 ? b=c : a=c;
			err = abs((b+a)/2 - c);
		}
	}
	return (b+a)/2;
}

double DataSet::MeanX()
{
	if (!meanXDirty)
	{
		meanX = 0;
		for (int i = 0; i < n; ++i)
		{
			meanX += xp[i];
		}
		meanX /= n;
	}
	return meanX;
}

double DataSet::MeanY()
{
	if (!meanYDirty)
	{
		meanY = 0;
		for (int i = 0; i < n; ++i)
		{
			meanY += yp[i];
		}
		meanY /= n;
	}
	return meanY;
}

//--------------- Print Functions -------------------------
void DataSet::PrintSpline(ostream& myout)
{
	double h = abs((xp[21]-xp[20])/12.0);
    
	myout << "#Spline data values for h= "<< h << endl;
    
	for ( double x=xp[0]; x < xp[n-1]; )
	{
		//if (DEBUG)	debugStream << "SplineOut at x=" << x << endl;
		myout << x << "\t";
		myout << SplineValue(x) << endl;
		x += h;
	}
	myout << endl;
}

void DataSet::PrintSplineWithDerivate1(ostream& myout)
{
	double h = abs((xp[21]-xp[20])/12.0);
    
	myout << "#Spline data values for h= "<< h << endl;
    
	for ( double x=xp[0]; x < xp[n-1]; )
	{
		//if (DEBUG)	debugStream << "SplineOut at x=" << x << endl;
		myout << x << "\t";
		myout << SplineValue(x) << "\t" << SplineDerivate1(x) << endl;
		x += h;
	}
	myout << endl;
}

void DataSet::PrintData(ostream& myout)
{
	for (int i = 0; i < n; ++i)
	{
		myout << xp[i] << "\t" << yp[i] << endl;
	}
	myout << endl;
}