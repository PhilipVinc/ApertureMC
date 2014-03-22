//
//  Transformer.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "Transformer.h"

using namespace std;

const
Transformer::Transformer(DataSet * input, bool _debug)
{
	debug = _debug;
	data = input;
}

Transformer::~Transformer()
{
}

//--------------- Debug Functions -------------------------
void Transformer::EnableDebug()
{
	debug = true;
	debugStream.open("debugLog-Transformer.txt");
}
void Transformer::DisableDebug()
{
	debug = false;
	debugStream.close();
}
