//
//  CalculatorMaxSimple.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 26/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__CalculatorMaxSimple__
#define __DataElaborator__CalculatorMaxSimple__

#include "Calculator.h"

class CalculatorMaxSimple : public Calculator
{
public:
	CalculatorMaxSimple(DataSet * input);
	~CalculatorMaxSimple();
    
    long double GetMaxXPosition();
    long double GetMaxYPosition();
    int GetMaxIndex();
    
private:
    void Elaborate();
	
    bool isDataComputed = false;
    int maxIndex;
    long double maxXPosition;
    long double maxYPosition;
    
};

#endif /* defined(__DataElaborator__CalculatorMaxSimple__) */
