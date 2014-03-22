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
#include "Fissure.h"

class PhysicalScene2 : public Function
{
public:
    PhysicalScene2();
    ~PhysicalScene2();
    
    /* ------------------------   Scene Management  ---------------------- */
    int AddFissure(double position, double intensity, double halfAperture);
    Function* GetFunction(int id);
    void RemoveFunction(int id);
    int FissureNumber();
    
    void SetFissurePosition(int id, double position);
    void SetFissureIntensity(int id, double intensity);
    
    /* ------------------------   Calculation Functions  ---------------------- */
    double operator()(double value);
    
    /* ------------------------   Print Functions  ---------------------- */
    void PrintFormula(std::ostream& myout);
    
protected:
    
    std::vector<Function*> components;
    int lastId;
};

#endif /* defined(__DataElaborator__PhysicalScene2__) */
