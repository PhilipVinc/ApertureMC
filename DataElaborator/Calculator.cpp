//
//  DataCruncher.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "Calculator.h"

using namespace std;

const
Calculator::Calculator(DataSet * input)
{
	debug = false;
	data = input;
}

Calculator::~Calculator()
{
    
}

//--------------- Debug Functions -------------------------
void Calculator::EnableDebug()
{
	debug = true;
	debugStream.open("debugLog-Crucher.txt");
}
void Calculator::DisableDebug()
{
	debug = false;
	debugStream.close();
}
