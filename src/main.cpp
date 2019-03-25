/*******************************************************************************
 * This file is part of DENISE.
 * Copyright (c) 2019 Moritz Fischer (moritz.s.fischer@eclipso.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include <iostream>
#include <cmath>
#include <fstream>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <random>
#include <pthread.h>
#include <unistd.h>

typedef double dtype;

#include "tools.h"
#include "parameter.h"
#include "particle.h"
#include "particles.h"
#include "data.h"
#include "parallel.h"
#include "hermite.h"
#include "initial_cond.h"
#include "nbody.h"
#include "analysis.h"

#include "tools.cpp"
#include "parameter.cpp"
#include "particle.cpp"
#include "particles.cpp"
#include "data.cpp"
#include "parallel.cpp"
#include "hermite.cpp"
#include "initial_cond.cpp"
#include "nbody.cpp"
#include "analysis.cpp"


using namespace std;

int main()
{
    //cout<<"  ======================================================  "<<endl;
    cout<<"                                                         "<<endl;
    cout<<"    ____     _____    __    _    _    _____    _____  "<<endl;
    cout<<"   |  _ \\   |  ___|  |  \\  | |  | |  /  ___|  |  ___| "<<endl;
    cout<<"   | | \\ \\  | |___   | | \\ | |  | |  | |___   | |___  "<<endl;
    cout<<"   | | | |  |  ___|  | |\\ \\| |  | |  \\___  \\  |  ___| "<<endl;
    cout<<"   | |_/ /  | |___   | | \\ | |  | |   ___| |  | |___  "<<endl;
    cout<<"   |____/   |_____|  |_|  \\__|  |_|  |_____/  |_____| "<<endl;
    cout<<endl;
    cout<<" Dynamical Evolution of Numerous Interacting Stars Engine"<<endl;
    cout<<"  ______________________________________________________  "<<endl;
    cout<<endl;
    cout<<"  Version: 1.0 - parallel, shared memory"<<endl;
    cout<<endl;
    time_t stime,etime;
    nbody sim("parameters.txt");
    cout<<"  ______________________________________________________  "<<endl;
    cout<<endl;
    if(!sim.replot())
    {
        cout<<"  Set initial conditions"<<endl;
        sim.set_IC();
        cout<<"  Start simulation"<<endl;
        clock_t begin = clock();
        time(&stime);

        sim.simulate();

        time(&etime);
        clock_t end = clock();
        cout<<"  Simulation has finished"<<endl;
        double pelapsed_secs = double(end - begin)/CLOCKS_PER_SEC;
        cout<<"   Processor time: "<<pelapsed_secs<<" s"<<endl;
        double elapsed_secs = difftime(etime,stime);
        cout<<"   Elapsed time: "<<elapsed_secs<<" s"<<endl;
    }
    cout<<"  Make plots"<<endl;
    sim.do_plot();
    cout<<endl;
    //cout<<"  ______________________________________________________  "<<endl;
    cout<<"  ======================================================  "<<endl;
    //cout<<"                                                        "<<endl;
    cout<<"                         The End                          "<<endl;
    //cout<<"                                                        "<<endl;
    cout<<"  ======================================================  "<<endl;

    pthread_exit(NULL);
    return 0;
}