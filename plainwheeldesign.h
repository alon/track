#ifndef __PLAINWHEELDESIGN_H__
#define __PLAINWHEELDESIGN_H__

#include <ode/ode.h>
#include "plainwheel.h"
#include "wheeldesign.h"

class PlainWheelDesign : public WheelDesign {
public:
	PlainWheelID create(dWorldID world, dSpaceID space);
	PlainWheelDesign(dReal _radius) { setRadius(_radius); };
};

typedef PlainWheelDesign *PlainWheelDesignID;

#endif	// __WHEELDESIGN_H__
