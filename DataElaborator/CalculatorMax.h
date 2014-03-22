//
//  CalculatorMax.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__CalculatorMax__
#define __DataElaborator__CalculatorMax__

#include <iostream>
#include "Calculator.h"

class CalculatorMax : public Calculator
{
public:
	CalculatorMax(DataSet * input);
	~CalculatorMax();
    
    double GetMaxXPosition();
    double GetMaxYPosition();
    int GetMaxIndex();

    
private:
    void Elaborate();

    bool isDataComputed = false;
    int maxIndex;
    double maxXPosition;
    double maxYPosition;
    
};

#endif /* defined(__DataElaborator__CalculatorMax__) */
