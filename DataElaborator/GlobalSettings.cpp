//
//  GlobalSettings.cpp
//  DataElaborator
//
//  Created by Filippo Vicentini on 23/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#include "GlobalSettings.h"

GlobalSettings& GlobalSettings::get_instance()
{
    // l'unica istanza della classe viene creata alla prima chiamata di get_instance()
    // e verrà distrutta solo all'uscita dal programma
    static GlobalSettings instance;
    return instance;
}