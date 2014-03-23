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
    bool showProgress = false;
    
private:
    // ecco il costruttore privato in modo che l'utente non possa istanziare direttamante
	GlobalSettings() { };
};

#endif /* defined(__DataElaborator__GlobalSettings__) */
