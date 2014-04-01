//
//  ConstantValue.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 31/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ConstantValue.h"
#include <cmath>

/* ----------   Init Functions -------- */
ConstantValue::ConstantValue(long double _value )
{
    value = _value;
}

ConstantValue::~ConstantValue()
{
    
}

/* ----------   Operative Functions -------- */

long double ConstantValue::operator()(long double x)
{
    return value;
}

void ConstantValue::PrintFormula(std::ostream& myout)
{
    myout << value;
}

