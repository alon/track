#include "automate.h"
#include "mydraw.h"

int Automat::Finished()
{
	return finished;
};

Automat::Automat(dReal _startTime, Machine * _machine):
startTime(_startTime), state(0), finished(0), machine(_machine)
{
};

void Automat::Action(dReal curTime)
{
}

void AutoEventFinish::Action(dReal curTime)
{
	aimCamera(cameraBody);
	switch (state) {
	case 0:
		if (curTime - startTime > event_time) {
			machine->Event();	// this should make a right turn. it can also be anything
			state = 1;
		};
		break;
	case 1:
		if (curTime - startTime > finish_time) {
			state = 2;
			finished = 1;
			if (finish_machine)
				finish_machine->Event();
		};
		break;
	};
}
