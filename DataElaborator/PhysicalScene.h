//
//  PhysicalScene.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 25/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__PhysicalScene__
#define __DataElaborator__PhysicalScene__

#include <iostream>
#include <vector>
#include "PhysicalObject.h"

class PhysicalScene : public PhysicalObject
{
public:
    PhysicalScene();
    ~PhysicalScene();
    
    /* ------------------------   Scene Management  ---------------------- */
    int AddFunction(PhysicalObject * object);
    PhysicalObject* GetObject(int id);
    void RemoveFunction(int id);
    int NumberOfObjects();
    
    /* ------------------------   Calculation Functions  ---------------------- */
    long double operator()(long double value);
    
    /* ------------------------   Print Functions  ---------------------- */
    void PrintFormula(std::ostream& myout);
    
protected:
    std::vector<PhysicalObject*> components;
    int lastId;
};

#endif /* defined(__DataElaborator__PhysicalScene__) */
