//
//  ElaboratorMaxMin.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ElaboratorMaxMin__
#define __DataElaborator__ElaboratorMaxMin__

#include <iostream>
#include <vector>

#include "Calculator.h"

class CalculatorMaxMin : public Calculator
{
public:
	CalculatorMaxMin(DataSet * input);
	~CalculatorMaxMin();
    
	void Elaborate();
	void Apply();
    
	void PrintData(std::ostream& myout);
	void PrintMinPoints(std::ostream& myout);
	void PrintMaxPoints(std::ostream& myout);
	
	int GetMaxNumber();
	int GetMinNumber();
	
	long double GetMaxXPosition(int number);
	long double GetMinXPosition(int number);
	long double GetMaxXSplinePosition(int number);
	long double GetMinXSplinePosition(int number);
	
	long double GetMaxYPosition(int number);
	long double GetMinYPosition(int number);
	long double GetMaxYSplinePosition(int number);
	long double GetMinYSplinePosition(int number);
	
    
private:
	void FindMaxMin();
    
	std::vector<int> maxIndex;
	std::vector<int> minIndex;
	int minN;
	int maxN;
    
	long double * xMin;
	long double * yMin;
	long double * xMax;
	long double * yMax;
};

#endif /* defined(__DataElaborator__ElaboratorMaxMin__) */
