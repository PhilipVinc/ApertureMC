//
//  ElaboratorSfasamento.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 29/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__ElaboratorSfasamento__
#define __DataElaborator__ElaboratorSfasamento__

#include <iostream>
#include "Elaborator.h"

class ElaboratorSfasamento : public Elaborator
{
	ElaboratorSfasamento(DataSet * data1, DataSet * data2);
	~ElaboratorSfasamento();
	
	void Elaborate();
};

#endif /* defined(__DataElaborator__ElaboratorSfasamento__) */
