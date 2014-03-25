//
//  PhysicalScene.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 25/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "PhysicalScene.h"

using namespace std;

/* ------------------------   Init Functions ---------------------- */
PhysicalScene::PhysicalScene()
{
    lastId = 0;
}

PhysicalScene::~PhysicalScene()
{
    for (int i = 0; i != components.size(); i++)
    {
        delete components[i];
    }
    components.clear();
}

/* ------------------------   Scene Management  ---------------------- */

int PhysicalScene::AddFunction(PhysicalObject * _object)
{
    components.push_back(_object);
    return components.size();
}

PhysicalObject* PhysicalScene::GetObject(int id)
{
    return components[id];
}

void PhysicalScene::RemoveFunction(int id)
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

int PhysicalScene::NumberOfObjects()
{
    return components.size();
}

/* ------------------------   Calculation Functions  ---------------------- */
long double PhysicalScene::operator()(long double x)
{
    long double result = 0.0;
    for (vector<PhysicalObject*>::size_type i=0; i != components.size(); i++)
    {
        result += (*components[i])(x);
    }
    return result;
}

/* ------------------------   Print Functions  ---------------------- */
void PhysicalScene::PrintFormula(std::ostream& myout)
{
    myout << "( ";
    for (vector<PhysicalObject*>::size_type i=0; i != components.size(); i++)
    {
        components[i]->PrintFormula(myout);
        if (i!=(components.size()-1)) myout << " + "; // If this is not the last element in the row, print +
    }
    myout << ") ";
}
