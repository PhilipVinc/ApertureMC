//
//  TransformerSimple.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "TransformerSimple.h"
#include <cmath>

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

void TransformerSimple::ScaleXCos2(DataSet * data)
{
	for (int i = 0; i < data->n; ++i)
	{
		data->xp[i] = (cosl(data->xp[i]))*(cosl(data->xp[i]));
	}
    data->SetDirty();
}

void TransformerSimple::ScaleYCos2(DataSet * data)
{
	for (int i = 0; i < data->n; ++i)
	{
		data->yp[i] = (cosl(data->yp[i]))*(cosl(data->yp[i]));
	}
    data->SetDirty();
}