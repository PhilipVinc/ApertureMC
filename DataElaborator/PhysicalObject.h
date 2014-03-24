//
//  PhysicalObject.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__PhysicalObject__
#define __DataElaborator__PhysicalObject__

#include <iostream>

class PhysicalObject {
public:
    virtual ~PhysicalObject();
    
    virtual long double operator()(long double value)=0;
    virtual void PrintFormula(std::ostream& myout)=0;
protected:
    
    
};

#endif /* defined(__DataElaborator__PhysicalObject__) */
