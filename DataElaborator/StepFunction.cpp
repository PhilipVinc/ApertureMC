//
//  StepFunction.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 26/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "StepFunction.h"

StepFunction::StepFunction(long double _center, long double _length, long double _height)
{
	center = _center;
	length = _length;
	height = _height;
}

StepFunction::~StepFunction()
{
	
}

long double StepFunction::operator()(long double x)
{
	if ((x >= center-length)&&(x <= center + length))
	{
		return height;
	}
	else
	{
		return 0.0;
	}
}

void StepFunction::PrintFormula(std::ostream &myout)
{
	return;
}