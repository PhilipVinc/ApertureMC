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
	meanXDirty = true;
	meanYDirty = true;
    splineDirty = true;
    n=0;
    
}

DataSet::DataSet(string inputPath, bool enableDebug)
{
	debug = enableDebug;
	meanXDirty = true;
	meanYDirty = true;
    splineDirty = true;
    n=0;
    
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
    long double x, y;
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
                    long double temp = xp[i];
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

int DataSet::AddMeasure(long double position, long double intensity)
{
    splineDirty = true;
	n++;
    xp.push_back(position);
    yp.push_back(intensity);
    zs.push_back(0.0);
	//if (DEBUG)	debugStream << "Added new Measure with ID= " << lastId << " position= " << newMeasure.position << " speed= " << newMeasure.intensity << endl;
    
	return (n-1);
}

void DataSet::RemoveMeasure(int id)
{
	xp.erase(xp.begin()+id);
    yp.erase(yp.begin()+id);
    SetDirty();
}

long double DataSet::x(int i) { return xp[i]; }
long double DataSet::y(int i) { return yp[i]; }
long double DataSet::s2(int i) { return zs[i]; }

long double DataSet::setX(int i, double _x)
{
    SetDirty();
    xp[i] = _x; return xp[i];
}
long double DataSet::setY(int i, double _y)
{
    SetDirty();
    yp[i] = _y; return yp[i];
}
long double DataSet::setS2(int i, double _s2)
{
    SetDirty();
    zs[i] = _s2; return zs[i];
}

void DataSet::SetDirty()
{
    meanXDirty = true;
    meanYDirty = true;
    splineDirty = true;
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
	long double * h = new long double[n];
	long double * b = new long double[n];
	long double * u = new long double[n];
	long double * v = new long double[n];
    
	for (int i = 0; i < n-1; i++)
	{
		h[i] = xp[i+1]-xp[i];
		b[i] = (yp[i+1] - yp[i])/h[i];
        //cout  << i << " \t h=" << h[i] << " \t b="<< b[i] << " \t " << endl;
	}
    
	u[1] = 2.0 * ( h[0] + h[1] );
	v[1] = 6.0 * ( b[1] - b[0] );
    
	for (int i = 2; i < n-1; i++)
	{
		u[i] = 2.0 * ( h[i] + h[i-1] ) - h[i-1]*h[i-1]/u[i-1];
		v[i] = 6.0 * ( b[i] - b[i-1] ) - h[i-1]*v[i-1]/u[i-1];
        //cout  << i <<"\t u=" << u[i] << " \t v="<< v[i] << " \t "<<endl;
	}
    
	zs[n-1]=0.0;
	for (int i = n-2; i > 0 ; i--)
	{
		zs[i] = (v[i] - h[i] * zs[i+1]) / u[i];
		//if (DEBUG)	debugStream << "SplineParameter z["<<i<<"]=" << zs[i] << endl;
	}
	zs[0]=0.0;
    
	delete[] h; delete[] b; delete[] u; delete[] v;
    splineDirty = false;
}

//--------------- Value Functions -------------------------
long double DataSet::SplineValue(long double x)
{
    if(splineDirty) ComputeSplineCoefficients();
    
	// Select the interval
	int i = n-2;
    i = abs((x-xp[0])/(xp[1]-xp[0]))+1; // This only works for constant X-spaced data!
    
	while ( i>0 )
	{
		//if ((( x > xp[i] ) && ( x < xp[i+1] )) || (( x < xp[i] ) && ( x > xp[i+1] )))
 		if  ((x - xp[i]) > 0 )
		{
			break;
		}
		i--;
	}
	long double h = xp[i+1] - xp[i];
	long double temp = zs[i]/2.0 + (x-xp[i]) * (zs[i+1] - zs[i]) / (6.0*h);
	temp = -(h/6.0) * (zs[i+1] + 2.0*zs[i]) + (yp[i+1] - yp[i]) / h + (x - xp[i])*temp;
	long double result = (yp[i] + (x - xp[i]) * temp);
    
	return result;
}

long double DataSet::SplineDerivate1(long double x)
{
    if(splineDirty) ComputeSplineCoefficients();
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
	long double h = xp[i+1] - xp[i];
	long double temp = zs[i]+ (x-xp[i]) * (zs[i+1] - zs[i]) / (2.0*h);
	long double result = -(h/6.0) * (zs[i+1] + 2.0*zs[i]) + (yp[i+1] - yp[i]) / h + (x - xp[i])*temp;
    
	return result;
}

long double DataSet::SplineDerivate1Zero(int index, int recursion)
{
    if(splineDirty) ComputeSplineCoefficients();
    if (index==0) index++;
	long double a = xp[index-1];
	long double b = xp[index+1];
	//cout << "Investigating x-= " << xp[index-1] << "\t x+= " << xp[index+1] << endl;
    
	if( SplineDerivate1(a)*SplineDerivate1(b) < 0.0 )
	{
		long double c ;
		long double err = 100;
		while( err > 0.0001)
		{
			c = (b+a)/2;
			(SplineDerivate1(a)*SplineDerivate1(c)) < 0 ? b=c : a=c;
			err = abs((b+a)/2 - c);
		}
	}
	return (b+a)/2;
}

long double DataSet::MeanX()
{
	if (meanXDirty)
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

long double DataSet::MeanY()
{
	if (meanYDirty)
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
	long double h = abs((xp[21]-xp[20])/12.0);
    
	myout << "#Spline data values for h= "<< h << endl;
    
	for ( long double x=xp[0]; x < xp[n-1]; )
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
	long double h = abs((xp[21]-xp[20])/12.0);
    
	myout << "#Spline data values for h= "<< h << endl;
    
	for ( long double x=xp[0]; x < xp[n-1]; )
	{
		//if (DEBUG)	debugStream << "SplineOut at x=" << x << endl;
		myout << x << "\t";
		myout << SplineValue(x) << " \t " << SplineDerivate1(x) << " \t "<<endl;
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