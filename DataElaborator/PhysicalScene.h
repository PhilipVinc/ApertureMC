//
//  PhysicalScene.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__PhysicalScene__
#define __DataElaborator__PhysicalScene__

#include <iostream>
#include <vector>
#include "Fissure.h"

class PhysicalScene : public PhysicalObject
{
public:
    PhysicalScene();
    ~PhysicalScene();
    
    /* ------------------------   Scene Management  ---------------------- */
    int AddFissure(double position, double intensity, double halfAperture);
    Fissure* GetFissure(int id);
    void RemoveFissure(int id);
    int FissureNumber();
    
    void SetFissurePosition(int id, double position);
    void SetFissureIntensity(int id, double intensity);

    /* ------------------------   Calculation Functions  ---------------------- */
    double operator()(double value);
    
    /* ------------------------   Print Functions  ---------------------- */
    void PrintFormula(std::ostream& myout);
    
protected:

    std::vector<Fissure*> fissures;
    int lastId;
};

#endif /* defined(__DataElaborator__PhysicalScene__) */
