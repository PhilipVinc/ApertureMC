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
#include "PhysicalObject.h"

class Fissure : public PhysicalObject
{
public:
    Fissure(long double position, long double relativeIntensity, long double halfAperture);
    ~Fissure();
    
    long double operator()(long double x);
    
    void PrintFormula(std::ostream& myout);
    
    long double position;
    long double relativeIntensity;
    long double halfAperture;
    
protected:

};

#endif /* defined(__DataElaborator__Fissure__) */
