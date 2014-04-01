//
//  ElaboratorMaxMin.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "CalculatorMaxMin.h"
#include "GlobalSettings.h"

#include <cmath>

using namespace std;

const
CalculatorMaxMin::CalculatorMaxMin(DataSet * input) : Calculator(input)
{
}

CalculatorMaxMin::~CalculatorMaxMin()
{
    delete[] xMax;
    delete [] xMin;
    delete [] yMax;
    delete [] yMin;
    maxIndex.clear();
    minIndex.clear();
}

//--------------- Elaboration Functions -------------------------
void CalculatorMaxMin::Elaborate()
{
	FindMaxMin();
    
	xMin = new long double[minN];
	yMin = new long double[minN];
	for (int i = 0; i < minN; ++i)
	{
		xMin[i] = data->SplineDerivate1Zero(minIndex[i]);
		yMin[i] = data->SplineValue(xMin[i]);
	}
    
	xMax = new long double[maxN];
	yMax = new long double[maxN];
	for (int i = 0; i < maxN; ++i)
	{
		xMax[i] = data->SplineDerivate1Zero(maxIndex[i]);
		yMax[i] = data->SplineValue(xMax[i]);
	}
}


void CalculatorMaxMin::FindMaxMin()
{
	//int spanLength = 4;
    int spanLength = GlobalSettings::get_instance().maxMinSearchSpan;
	minN = 0; maxN = 0;
    
	for (int i = spanLength+1 ; i < data->n; ++i)
	{
		bool acceptMinimum = true;
		for (int j = 1; j <= spanLength; j++)
		{
			if ( !(data->y(i) <= data->y(i+j)) || !(data->y(i) <= data->y(i-j)) )
			{
				acceptMinimum = false;
				break;
			}
		}
		if (acceptMinimum)
		{
			minIndex.push_back(i);
			i++;
			minN++;
			//cout << "Min at x=" << data->xp[i] << endl;
            
		}
		else
		{
			bool acceptMaximum = true;
			for (int j = 1; j <= spanLength; j++)
			{
				if ( !(data->y(i) >= data->y(i+j)) || !(data->y(i) >= data->y(i-j)) )
				{
					acceptMaximum = false;
					break;
				}
			}
			if (acceptMaximum)
			{
				maxIndex.push_back(i);
				i++;
				maxN++;
				//cout << "Max at x=" << data->xp[i] << endl;
			}
		}
	}
}

void CalculatorMaxMin::Apply()
{
    
}

int CalculatorMaxMin::GetMaxNumber()
{
	return maxIndex.size();
}

int CalculatorMaxMin::GetMinNumber()
{
	return minIndex.size();
}

long double CalculatorMaxMin::GetMaxXPosition(int number)
{
	return data->x(maxIndex[number]);
}

long double CalculatorMaxMin::GetMinXPosition(int number)
{
	return data->x(minIndex[number]);
}

long double CalculatorMaxMin::GetMaxXSplinePosition(int number)
{
	return xMax[number];
}

long double CalculatorMaxMin::GetMinXSplinePosition(int number)
{
	return xMin[number];
}

long double CalculatorMaxMin::GetMaxYPosition(int number)
{
	return data->y(maxIndex[number]);
}

long double CalculatorMaxMin::GetMinYPosition(int number)
{
	return data->y(minIndex[number]);
}

long double CalculatorMaxMin::GetMaxYSplinePosition(int number)
{
	return yMax[number];
}

long double CalculatorMaxMin::GetMinYSplinePosition(int number)
{
	return yMin[number];
}


void CalculatorMaxMin::PrintData(ostream& myout)
{
	for (int i = 0; i < maxN; ++i)
	{
		myout << xMax[i] << "\t" << yMax[i] << endl;
	}
	for (int i = 0; i < minN; ++i)
	{
		myout << xMin[i] << "\t" << yMin[i] << endl;
	}
	myout << endl;
}

void CalculatorMaxMin::PrintMinPoints(ostream& myout)
{
	for (int i = 0; i < minN; ++i)
	{
		myout << xMin[i] << "\t" << yMin[i] << endl;
	}
	myout << endl;
}

void CalculatorMaxMin::PrintMaxPoints(ostream& myout)
{
	for (int i = 0; i < maxN; ++i)
	{
		myout << xMax[i] << "\t" << yMax[i] << endl;
	}
	myout << endl;
}
