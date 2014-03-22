//
//  Elaborator.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__Elaborator__
#define __DataElaborator__Elaborator__

#include <iostream>
#include <fstream>

#include "DataSet.h"

class Elaborator
{
public:
	Elaborator(DataSet * input);
	~Elaborator();
    
	void EnableDebug();
	void DisableDebug();
    
	virtual void Elaborate() =0;
	virtual void Apply() =0;
    
protected:
	DataSet * inputData;
	
	// Debug Variables
	bool debug;
	std::ofstream debugStream;
};

#endif /* defined(__DataElaborator__Elaborator__) */
