//
//  PhysicalScene.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__PhysicalScene2__
#define __DataElaborator__PhysicalScene2__

#include <iostream>
#include <vector>
#include "PhysicalObject.h"
#include "Fissure.h"

class PhysicalScene2 : public PhysicalObject
{
public:
    PhysicalScene2();
    ~PhysicalScene2();
    
    /* ------------------------   Scene Management  ---------------------- */
    int AddFunction(PhysicalObject * object);
    PhysicalObject* GetObject(int id);
    void RemoveFunction(int id);
    int NumberOfObjects();
    
    int AddFissure(long double position, long double intensity, long double halfAperture);
    
    /* ------------------------   Calculation Functions  ---------------------- */
    long double operator()(long double value);
    
    /* ------------------------   Print Functions  ---------------------- */
    void PrintFormula(std::ostream& myout);
    
protected:
    std::vector<PhysicalObject*> components;
    int lastId;
};

#endif /* defined(__DataElaborator__PhysicalScene2__) */
