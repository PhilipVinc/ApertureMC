//
//  CalculatorMax.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "CalculatorMax.h"

CalculatorMax::CalculatorMax(DataSet * input) : Calculator(input)
{
}

CalculatorMax::~CalculatorMax()
{
}

int CalculatorMax::GetMaxIndex()
{
    if ( !isDataComputed) Elaborate();
    return maxIndex;
}

double CalculatorMax::GetMaxXPosition()
{
    if ( !isDataComputed) Elaborate();
    return maxXPosition;
}

double CalculatorMax::GetMaxYPosition()
{
    if ( !isDataComputed) Elaborate();
    return maxYPosition;
}

void CalculatorMax::Elaborate()
{
    maxIndex = 0;
	{
		double max = data->yp[0];
		// Trovo il massimo centrale
		for (int i = 0; i < data->n; ++i)
		{
			if( max < data->yp[i] )
			{
				maxIndex = i;
				max = data->yp[i];
			}
		}
	}
    
	maxXPosition = data->SplineDerivate1Zero(maxIndex);
    maxYPosition = data->SplineValue(maxXPosition);
    
	if (debug) debugStream << "Il dato piu alto è a x= " << data->xp[maxIndex] << "\t y= " << data->yp[maxIndex] << std::endl;
    
	//Calcolo lo zero del polinomio derivata
	if (debug) debugStream << "#Il centro dei dati è:";
    if (debug) debugStream << maxXPosition << std::endl;
}