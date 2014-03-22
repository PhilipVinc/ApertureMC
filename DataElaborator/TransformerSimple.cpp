//
//  TransformerSimple.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "TransformerSimple.h"

void TransformerSimple::ShiftX(DataSet * data, double value)
{
    for (int i = 0; i < data->n; ++i)
	{
		data->xp[i] += value;
	}
}

void TransformerSimple::ShiftY(DataSet * data, double value)
{
    for (int i = 0; i < data->n; ++i)
	{
		data->yp[i] += value;
	}
}

void TransformerSimple::ScaleX(DataSet * data, double value)
{
    for (int i = 0; i < data->n; ++i)
	{
		data->xp[i] *= value;
	}
}

void TransformerSimple::ScaleY(DataSet * data, double value)
{
    for (int i = 0; i < data->n; ++i)
	{
		data->yp[i] *= value;
	}
}