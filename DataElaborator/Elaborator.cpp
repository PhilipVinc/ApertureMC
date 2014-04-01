//
//  Elaborator.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "Elaborator.h"

using namespace std;

const
Elaborator::Elaborator(DataSet * input1, DataSet * input2)
{
	debug = false;
	inputData1 = input1;
	inputData2 = input2;
}

Elaborator::~Elaborator()
{
    
}

//--------------- Debug Functions -------------------------
void Elaborator::EnableDebug()
{
	debug = true;
	debugStream.open("debugLog-ElaboratorShiftCenter.txt");
}
void Elaborator::DisableDebug()
{
	debug = false;
	debugStream.close();
}
