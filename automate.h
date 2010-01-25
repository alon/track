#ifndef __AUTOMAT_H__
#define __AUTOMAT_H__

#include <ode/ode.h>
#include "Simulation.h"

// from this the use of the automat creates a useful sub class, where Event does something,
// like setting the speed of one of the tracks
class Machine {
protected:
    Simulation* m_sim;
public:
    Machine(Simulation* sim) : m_sim(sim) {}
	virtual void Event() = 0;
};

typedef Machine* MachineID;

class Automat {
protected:
	dReal startTime;
	int state;
	int finished;
	MachineID machine;	// machine that is automated
public:
	 virtual void Action(dReal curTime);
	int Finished();
	 Automat(dReal _startTime, Machine * machine);
};

class AutoEventFinish:public Automat {
      private:
	dReal event_time;
	dReal finish_time;
	dBodyID cameraBody;
	MachineID finish_machine;
      public:
	 AutoEventFinish(double _startTime, double _event_time, MachineID _machine,
			 double _finish_time, MachineID _finish_machine,
			 dBodyID _cameraBody)
	:Automat(_startTime, _machine), event_time(_event_time),
	    finish_time(_finish_time), cameraBody(_cameraBody), finish_machine(_finish_machine){
	};

	void Action(dReal curTime);
};

#endif				// __AUTOMAT_H__
