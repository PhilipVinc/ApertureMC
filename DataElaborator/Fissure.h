//
//  Fenditura.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__Fissure__
#define __DataElaborator__Fissure__

#include <iostream>
#include "Function.h"

class Fissure : public Function
{
public:
    Fissure(double position, double relativeIntensity, double halfAperture);
    ~Fissure();
    
    double operator()(double x);
    
    void PrintFormula(std::ostream& myout);
    
    double position;
    double relativeIntensity;
    double halfAperture;
    
protected:

};

#endif /* defined(__DataElaborator__Fissure__) */
