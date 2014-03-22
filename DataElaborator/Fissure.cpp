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
Fissure::Fissure(double _position, double _intensity, double _halfAperture)
{
    position = _position;
    relativeIntensity = _intensity;
    halfAperture = _halfAperture;
}

Fissure::~Fissure()
{
    
}

/* ----------   Operative Functions -------- */

double Fissure::operator()(double x)
{
    double value = 4*relativeIntensity*relativeIntensity*sin((x-position)*halfAperture)*sin((x-position)*halfAperture)/(x-position)/(x-position);
    return value;
}

void Fissure::PrintFormula(std::ostream& myout)
{
    myout << "4*"<< relativeIntensity*relativeIntensity << "*sin( (x - " << position << " )*" <<halfAperture << ")**2 /(( x-"<< position <<")**2)" <<std::endl;
}

