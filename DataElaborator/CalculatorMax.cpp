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
	//EnableDebug();
}

CalculatorMax::~CalculatorMax()
{
}

int CalculatorMax::GetMaxIndex()
{
    if ( !isDataComputed) Elaborate();
    return maxIndex;
}

long double CalculatorMax::GetMaxXPosition()
{
    if ( !isDataComputed) Elaborate();
    return maxXPosition;
}

long double CalculatorMax::GetMaxYPosition()
{
    if ( !isDataComputed) Elaborate();
    return maxYPosition;
}

void CalculatorMax::Elaborate()
{
    maxIndex = 0;
	{
		long double max = data->y(0);
		// Trovo il massimo centrale
		for (int i = 0; i < data->n; ++i)
		{
			if( max < data->y(i) )
			{
				maxIndex = i;
				max = data->y(i);
			}
			if (debug) debugStream << "iter " << i << " \t max= " << max << " \t data-y(i)= "<< data->y(i)<< std::endl;

		}
	}
	if (debug) debugStream << "Il dato piu alto è a x= " << data->x(maxIndex) << "\t y= " << data->y(maxIndex) << std::endl;
    
	maxXPosition = data->SplineDerivate1Zero(maxIndex);
	
	//Calcolo lo zero del polinomio derivata
	if (debug) debugStream << "#Il centro dei dati è:";
    if (debug) debugStream << maxXPosition << std::endl;
	
    maxYPosition = data->SplineValue(maxXPosition);

}