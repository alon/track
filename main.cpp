// defines list - you can change these for global effect

//#define SIMPLE_TRACK

////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <fstream>

#include <cstring>

#include <ode/ode.h>
//#include <drawstuff/drawstuff.h>

// start my includes

#define WE_ARE_IN_MAIN

#include "keymap.h"
#include "tags.h"

#include "error.h"
#include "common.h"
#include "cconfig.h"
#include "myobj.h"
#include "mydraw.h"
#include "car.h"
#include "automate.h"

// designs for links
#include "linkboxdesign.h"
#include "mboxlinkdesign.h"
// designs for wheels
#include "plainwheeldesign.h"
#include "twheeldesign.h"

#include "Simulation.h"

using namespace std;

// globals
//


/////////////////////////////////////////////////////////////////////
//                                                                 //
//                      Other Variables                            //
//                                                                 //
/////////////////////////////////////////////////////////////////////
// Process Keyboard Input

#include "main_command.cpp"
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//                              Main                                            //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    dInitODE();
    Simulation* sim = Simulation::instance(); // make sure simulation is constructed (Simulation is a singleton)
	// check command line arguments
	if (argc >= 4 && strcmp("-auto", argv[1]) == 0) {
		/*
		if (strcmp("fallfwdexit", argv[2])) {
			dReal end = atoi(argv[3]);
			automat = new AutoFallFwdexit(world, space, end);
		}
		*/
	}

    sim->run(argc, argv);

	return 0;
}

