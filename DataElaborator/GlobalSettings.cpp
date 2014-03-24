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

void GlobalSettings::DrawProgressBar(int len, long double percent, int iter, int total)
{
    percent = percent + 0.01;
    std::cout << "\x1B[2K"; // Erase the entire current line.
    std::cout << "\x1B[0E"; // Move to the beginning of the current line.
    std::string progress;
    for (int i = 0; i < len; ++i) {
        if (i < static_cast<int>(len * percent)) {
            progress += "=";
        } else {
            progress += " ";
        }
    }
    std::cout << "[" << progress << "] " << (static_cast<int>(100 * percent)) << "%";
    std::cout << " \t\t Simulation - " << iter << " / " << total;
    
    std::flush(std::cout); // Required.
}

void GlobalSettings::DrawSimulationProgressBar(int len, long double percent, int iter)
{
    DrawProgressBar(len, percent, iter-min_fend+1, max_fend-min_fend+1);
}