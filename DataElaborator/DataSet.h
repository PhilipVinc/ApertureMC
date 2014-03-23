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
	long double position;
	long double intensity;
};

class DataSet
{
    friend class TransformerSimple;
public:
	DataSet();
	DataSet(std::string inputPath, bool enableDebug = true);
	~DataSet();
    
	void EnableDebug();
	void DisableDebug();
    
	int AddMeasure(long double position, long double intensity);
	void RemoveMeasure(int id);
    void OrderAlongXAxis();
    void EraseData();
    void SetDirty();
    
    long double x(int i);
    long double y(int i);
    long double s2(int i);
    
    long double setX(int i, double x);
    long double setY(int i, double y);
    long double setS2(int i, double s2);
    
	void ComputeSplineCoefficients();
    
	long double SplineValue(long double x);
	long double SplineDerivate1(long double x);
	void PrintSpline(std::ostream& myout);
	void PrintSplineWithDerivate1(std::ostream& myout);
	long double SplineDerivate1Zero(int index, int recursion = 0);
    
	long double MeanX();
	long double MeanY();
    
	void PrintData(std::ostream& myout);

 	int n;
    
private:
 	bool meanXDirty;
	bool meanYDirty;
    
	long double meanX;
	long double meanY;
    
    bool splineDirty;
    
    std::vector<long double>  xp;
    std::vector<long double>  yp;
    std::vector<long double> zs; // spline
    
	// Debug Variables
	bool debug;
	std::ofstream debugStream;
};

#endif /* defined(__DataElaborator__DataSet__) */
