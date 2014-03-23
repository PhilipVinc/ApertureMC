//
//  TransformerSimple.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "TransformerSimple.h"

void TransformerSimple::ShiftX(DataSet * data, long double value)
{
    for (int i = 0; i < data->n; ++i)
	{
		data->xp[i] += value;
	}
    data->SetDirty();
}

void TransformerSimple::ShiftY(DataSet * data, long double value)
{
    for (int i = 0; i < data->n; ++i)
	{
		data->yp[i] += value;
	}
    data->SetDirty();
}

void TransformerSimple::ScaleX(DataSet * data, long double value)
{
    for (int i = 0; i < data->n; ++i)
	{
		data->xp[i] *= value;
	}
    data->SetDirty();
}

void TransformerSimple::ScaleY(DataSet * data, long double value)
{
    for (int i = 0; i < data->n; ++i)
	{
		data->yp[i] *= value;
	}
    data->SetDirty();
}