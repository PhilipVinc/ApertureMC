//
//  DataCruncher.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__DataCruncher__
#define __DataElaborator__DataCruncher__

#include <iostream>
#include <fstream>

#include "DataSet.h"

class Calculator
{
public:
	Calculator(DataSet * input);
	~Calculator();
    
	void EnableDebug();
	void DisableDebug();
    
	virtual void Elaborate() =0;
    
protected:
	DataSet * data;
	
	// Debug Variables
	bool debug;
	std::ofstream debugStream;
};

#endif /* defined(__DataElaborator__DataCruncher__) */
