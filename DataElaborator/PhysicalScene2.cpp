//
//  PhysicalScene2.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "PhysicalScene2.h"

using namespace std;

/* ------------------------   Init Functions ---------------------- */
PhysicalScene2::PhysicalScene2()
{
    //components = *new vector<Function*>();
    lastId = 0;
}

PhysicalScene2::~PhysicalScene2()
{
    for (int i = 0; i != components.size(); i++)
    {
        delete components[i];
    }
    components.clear();
}

/* ------------------------   Scene Management  ---------------------- */

int PhysicalScene2::AddFunction(PhysicalObject * _object)
{
    components.push_back(_object);
    return components.size();
}

PhysicalObject* PhysicalScene2::GetObject(int id)
{
    return components[id];
}

void PhysicalScene2::RemoveFunction(int id)
{
    if (id < components.size())
    {
        components.erase(components.begin()+id);
    }
    else
    {
        std::cerr << "erase failed"<< std::endl;
    }
}

int PhysicalScene2::NumberOfObjects()
{
    return components.size();
}

/* ------------------------>   Fissures  <---------------------- */

int PhysicalScene2::AddFissure(long double position, long double intensity, long double halfAperture)
{
    Fissure * newFissure = new Fissure(position, intensity, halfAperture);
    components.push_back(newFissure);
    return components.size();
}

/* ------------------------   Calculation Functions  ---------------------- */
long double PhysicalScene2::operator()(long double x)
{
    long double result = 0.0;
    for (vector<PhysicalObject*>::size_type i=0; i != components.size(); i++)
    {
        result += (*components[i])(x);
    }
    return result;
}

/* ------------------------   Print Functions  ---------------------- */
void PhysicalScene2::PrintFormula(std::ostream& myout)
{
    myout << "( ";
    for (vector<PhysicalObject*>::size_type i=0; i != components.size(); i++)
    {
        components[i]->PrintFormula(myout);
        if (i!=(components.size()-1)) myout << " + "; // If this is not the last element in the row, print +
    }
    myout << ") ";
}


