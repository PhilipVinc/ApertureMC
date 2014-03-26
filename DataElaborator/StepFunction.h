//
//  StepFunction.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 26/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__StepFunction__
#define __DataElaborator__StepFunction__

#include "Function.h"
#include "PhysicalObject.h"

#include <iostream>

class StepFunction : public PhysicalObject
{
public:
	StepFunction(long double center, long double length, long double height);
	~StepFunction();
	
	long double operator () (long double x);
	void PrintFormula(std::ostream& myout);
	
protected:
	long double center;
	long double length;
	long double height;
};


#endif /* defined(__DataElaborator__StepFunction__) */
