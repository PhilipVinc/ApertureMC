//
//  Transformer.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__Transformer__
#define __DataElaborator__Transformer__

#include <iostream>
#include <fstream>

#include "DataSet.h"

class Transformer
{
public:
	Transformer(DataSet * input, bool _debug = false);
	~Transformer();
    
	void EnableDebug();
	void DisableDebug();
    
	virtual void Apply() =0;
    
protected:
	DataSet * data;
	
	// Debug Variables
	bool debug;
	std::ofstream debugStream;
};


#endif /* defined(__DataElaborator__Transformer__) */
