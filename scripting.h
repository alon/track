#ifndef __SCRIPTING_H__
#define __SCRIPTING_H__

#include <drawstuff/drawstuff.h>
#include "automate.h"

//////////////////////////////////////////////////////////////////////////
//      Machine Interface to automat                                    //
//////////////////////////////////////////////////////////////////////////
class LeftTrackMachine : public Machine {
public:
    LeftTrackMachine(Simulation* sim) : Machine(sim) {}
	void Event() {
		m_sim->setSpeed(1, m_sim->max_speed);
	};
};

class LeftRightHalfMachine:public Machine {
public:
    LeftRightHalfMachine(Simulation* sim) : Machine(sim) {}
	void Event() {
		m_sim->setSpeed(1, m_sim->max_speed);
		m_sim->setSpeed(0,  m_sim->max_speed / 2);
	};
};

class FullSpeedAheadMachine:public Machine {
public:
    FullSpeedAheadMachine(Simulation* sim) : Machine(sim) {}
	void Event() {
		m_sim->setSpeed(0, m_sim->max_speed);
		m_sim->setSpeed(1, m_sim->max_speed);
	};
};

class DoNothingMachine : public Machine {
public:
    DoNothingMachine(Simulation* sim) : Machine(sim) {}
	void Event() {		// hahaha
	};
};

class ExitMachine: public Machine {
public:
    ExitMachine(Simulation* sim) : Machine(sim) {}
	void Event() {
		dsStop();
	};
};

#endif // __SCRIPTING_H__

