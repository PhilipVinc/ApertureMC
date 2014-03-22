//
//  PhysicalScene.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "PhysicalScene.h"

using namespace std;

/* ------------------------   Init Functions ---------------------- */
PhysicalScene::PhysicalScene()
{
    //fissures = *new vector<Fissure*>();
    lastId = 0;
}

PhysicalScene::~PhysicalScene()
{
    for (int i = 0; i != fissures.size(); i++)
    {
        delete fissures[i];
    }
    fissures.clear();
}

/* ------------------------   Scene Management  ---------------------- */
int PhysicalScene::AddFissure(double position, double intensity, double halfAperture)
{
    Fissure * newFissure = new Fissure(position, intensity, halfAperture);
    fissures.push_back(newFissure);
    return fissures.size();
}

Fissure* PhysicalScene::GetFissure(int id)
{
    return fissures[id];
}

void PhysicalScene::RemoveFissure(int id)
{
    if (id < fissures.size())
    {
        fissures.erase(fissures.begin()+id);
    }
    else
    {
        std::cerr << "erase failed"<< std::endl;
    }
}

int PhysicalScene::FissureNumber()
{
    return fissures.size();
}

void PhysicalScene::SetFissurePosition(int id, double position)
{
    if (id < fissures.size())
    {
        fissures[id]->position = position;
    }
    else
    {
        std::cerr << "edit failed"<< std::endl;
    }
}

void PhysicalScene::SetFissureIntensity(int id, double intensity)
{
    if (id < fissures.size())
    {
        fissures[id]->relativeIntensity = intensity;
    }
    else
    {
        std::cerr << "edit failed"<< std::endl;
    }
}

/* ------------------------   Calculation Functions  ---------------------- */
double PhysicalScene::operator()(double x)
{
    double result = 0.0;
    for (vector<Fissure*>::size_type i=0; i != fissures.size(); i++)
    {
        result += (*fissures[i])(x);
    }
    return result;
}

/* ------------------------   Print Functions  ---------------------- */
void PhysicalScene::PrintFormula(std::ostream& myout)
{
    for (vector<Fissure*>::size_type i=0; i != fissures.size(); i++)
    {
        fissures[i]->PrintFormula(myout);
        if (i!=(fissures.size()-1)) myout << " + "; // If this is not the last element in the row, print +
    }
}


