//
//  DataSet.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__DataSet__
#define __DataElaborator__DataSet__

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
struct Measure
{
	double position;
	double intensity;
};

class DataSet
{
public:
	DataSet();
	DataSet(std::string inputPath, bool enableDebug = true);
	~DataSet();
    
	void EnableDebug();
	void DisableDebug();
    
	int AddMeasure(double position, double intensity);
	void RemoveMeasure(int id);
    void OrderAlongXAxis();
    void EraseData();
    
	void ComputeSplineCoefficients();
    
	double SplineValue(double x);
	double SplineDerivate1(double x);
	void PrintSpline(std::ostream& myout);
	void PrintSplineWithDerivate1(std::ostream& myout);
	double SplineDerivate1Zero(int index, int recursion = 0);
    
	double MeanX();
	double MeanY();
    
	void PrintData(std::ostream& myout);

    std::vector<double> xp;
    std::vector<double> yp;
    std::vector<double> zs; // spline

 	int n;
    
private:
 	bool meanXDirty;
	bool meanYDirty;
    
	double meanX;
	double meanY;
    
	// Debug Variables
	bool debug;
	std::ofstream debugStream;
};

#endif /* defined(__DataElaborator__DataSet__) */
