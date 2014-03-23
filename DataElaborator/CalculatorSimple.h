//
//  CalculatorSimple.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__CalculatorSimple__
#define __DataElaborator__CalculatorSimple__

#include <iostream>
#include "DataSet.h"

class CalculatorSimple
{
public:
    static double SplineDiff(DataSet * data1, DataSet * data2, double xMin, double xMax, double step);
};
#endif /* defined(__DataElaborator__CalculatorSimple__) */
