//
//  CalculatorSimple.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "CalculatorSimple.h"

#include <cmath>
using namespace std;

long double CalculatorSimple::SplineDiff(DataSet * data1, DataSet * data2, long double xMin, long double xMax, long double step)
{
    long double x = xMin;
    long double diff;
    long double totalDiff = 0.0;
    int stepN = 0;
    do
    {

        diff = fabs(data1->SplineValue(x) - data2->SplineValue(x));

        totalDiff += diff*diff;
        x += step;
        stepN ++;
    } while (x < xMax);
    
    totalDiff /= double(stepN);
    
    return totalDiff;
}