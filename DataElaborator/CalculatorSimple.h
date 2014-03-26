//
//  CalculatorSimple.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__CalculatorSimple__
#define __DataElaborator__CalculatorSimple__

#include "DataSet.h"

class CalculatorSimple
{
public:
    static long double SplineDiff(DataSet * data1, DataSet * data2, long double xMin, long double xMax, long double step);
};
#endif /* defined(__DataElaborator__CalculatorSimple__) */
