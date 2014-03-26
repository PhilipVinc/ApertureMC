//
//  PhysicalScene.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__PhysicalSceneFissures__
#define __DataElaborator__PhysicalSceneFissures__

#include "PhysicalObject.h"
#include "PhysicalScene.h"
#include "Fissure.h"

class PhysicalSceneFissures : public PhysicalScene
{
public:
    PhysicalSceneFissures();
    ~PhysicalSceneFissures();
    
    /* ------------------------   Scene Management  ---------------------- */
    int AddFissure(long double position, long double intensity, long double halfAperture);

};

#endif /* defined(__DataElaborator__PhysicalSceneFissures__) */
