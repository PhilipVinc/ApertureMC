//
//  Fenditura.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "Fissure.h"
#include <cmath>

/* ----------   Init Functions -------- */
Fissure::Fissure(long double _position, long double _intensity, long double _halfAperture)
{
    position = _position;
    relativeIntensity = _intensity;
    halfAperture = _halfAperture;
}

Fissure::~Fissure()
{
    
}

/* ----------   Operative Functions -------- */

long double Fissure::operator()(long double x)
{
    long double value = 4*relativeIntensity*relativeIntensity*sin((x-position)*halfAperture)*sin((x-position)*halfAperture)/(x-position)/(x-position);
    //double value = 4*relativeIntensity*sin((x-position)*halfAperture)/(x-position);
    return value;
}

void Fissure::PrintFormula(std::ostream& myout)
{
    myout << "4*"<< relativeIntensity*relativeIntensity << "*(sin( (x - " << position << " )*" <<halfAperture << "))**2 /(( x-"<< position <<")**2)";
}

