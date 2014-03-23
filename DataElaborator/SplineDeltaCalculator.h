//
//  SplineDeltaCalculator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__SplineDeltaCalculator__
#define __DataElaborator__SplineDeltaCalculator__

#include <iostream>
#include "DeltaCalculator.h"

class SplineDeltaCalculator : public DeltaCalculator
{
public:
    SplineDeltaCalculator(DataSet * expData, DataSet * simData, int xMinIndex, int yMinIndex);
    ~SplineDeltaCalculator();
    
    long double GetDelta();
protected:
};


#endif /* defined(__DataElaborator__SplineDeltaCalculator__) */
