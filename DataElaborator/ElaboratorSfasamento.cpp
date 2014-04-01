//
//  ElaboratorSfasamento.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 29/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "ElaboratorSfasamento.h"

ElaboratorSfasamento::ElaboratorSfasamento(DataSet * data1, DataSet * data2) : Elaborator(data1, data2)
{
	
}

void ElaboratorSfasamento::Elaborate()
{
	int n1 = inputData1->n;
	int n2 = inputData1->n;
	
	// Determine wich curve to start from
	long double yStart1 = inputData1->y(0);
	long double yStart2 = inputData2->y(0);
	
	DataSet * backCurve;
	DataSet * frontCurve;
	
	if (yStart1 > yStart2)
	{
		backCurve = inputData1;
		frontCurve = inputData2;
	}
	else
	{
		backCurve = inputData2;
		frontCurve = inputData1;
	}
		
	int nEnd;
	for (int i = frontCurve->n-1; i>0; i--)
	{
		if (backCurve->y(i) < frontCurve->y(frontCurve->n-1))
		{
			nEnd = i;
			break;
		}
	}
	
	long double xBack;
	long double xFront;
	long double yBack;
	long double yFront;
	long double totalDiff = 0.0;
	
	int n = 0;
	do {
		xBack = backCurve->x(n);
		yBack = backCurve->y(n);
		
		
		n++;
	} while (n != nEnd);
	
	
	
	long double x = backCurve->x(0);
	
	
}
