//
//  GlobalSettings.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__GlobalSettings__
#define __DataElaborator__GlobalSettings__

#include <iostream>

class GlobalSettings {
public:
	static GlobalSettings& get_instance();

	bool method() { return true; };
    
    int maxMinSearchSpan = 4;
	int currentIteration;
    bool showProgress = false;
    int max_fend;
    int min_fend;
	long double FISSURE_POS = 0.5;
    
    void DrawSimulationProgressBar(int len, long double percent);
    void DrawProgressBar(int len, long double percent, int iter, int total);
    
private:
    // costruttore privato in modo non si possa istanziare direttamante
	GlobalSettings() { };
};

#endif /* defined(__DataElaborator__GlobalSettings__) */
