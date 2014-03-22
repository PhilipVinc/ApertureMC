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
int PhysicalScene2::AddFissure(double position, double intensity, double halfAperture)
{
    Fissure * newFissure = new Fissure(position, intensity, halfAperture);
    components.push_back(newFissure);
    return components.size();
}

/*
Fissure* PhysicalScene2::GetFissure(int id)
{
    return components[id];
}*/

Function* PhysicalScene2::GetFunction(int id)
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

int PhysicalScene2::FissureNumber()
{
    return components.size();
}

void PhysicalScene2::SetFissurePosition(int id, double position)
{
    if (id < components.size())
    {
        static_cast<Fissure*>(components[id])->position = position;
    }
    else
    {
        std::cerr << "edit failed"<< std::endl;
    }
}

void PhysicalScene2::SetFissureIntensity(int id, double intensity)
{
    if (id < components.size())
    {
        static_cast<Fissure*>(components[id])->relativeIntensity = intensity;
    }
    else
    {
        std::cerr << "edit failed"<< std::endl;
    }
}

/* ------------------------   Calculation Functions  ---------------------- */
double PhysicalScene2::operator()(double x)
{
    double result = 0.0;
    for (vector<Function*>::size_type i=0; i != components.size(); i++)
    {
        result += (*components[i])(x);
    }
    return result;
}

/* ------------------------   Print Functions  ---------------------- */
void PhysicalScene2::PrintFormula(std::ostream& myout)
{
    myout << "( ";
    for (vector<Function*>::size_type i=0; i != components.size(); i++)
    {
        components[i]->PrintFormula(myout);
        if (i!=(components.size()-1)) myout << " + "; // If this is not the last element in the row, print +
    }
    myout << ") ";

}


