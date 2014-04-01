//
//  PhysicalScene2.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "PhysicalSceneFissures.h"

using namespace std;

/* ------------------------   Init Functions ---------------------- */
PhysicalSceneFissures::PhysicalSceneFissures()
{
}

PhysicalSceneFissures::~PhysicalSceneFissures()
{
}

/* ------------------------>   Fissures  <---------------------- */

int PhysicalSceneFissures::AddFissure(long double position, long double intensity, long double halfAperture)
{
    Fissure * newFissure = new Fissure(position, intensity, halfAperture);
    components.push_back(newFissure);
    return components.size();
}

int PhysicalSceneFissures::AddConstant(long double intensity)
{
	ConstantValue * newConstantValue = new ConstantValue(intensity);
	components.push_back(newConstantValue);
	return components.size();
}