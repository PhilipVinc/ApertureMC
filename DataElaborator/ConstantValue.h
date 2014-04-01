//
//  ConstantValue.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 31/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ConstantValue__
#define __DataElaborator__ConstantValue__

#include <iostream>
#include "PhysicalObject.h"

class ConstantValue : public PhysicalObject
{
public:
    ConstantValue(long double value);
    ~ConstantValue();
    
    long double operator()(long double x);
    
    void PrintFormula(std::ostream& myout);
    
    long double value;
    
protected:
	
};
#endif /* defined(__DataElaborator__ConstantValue__) */
