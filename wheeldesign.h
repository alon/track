#ifndef __WHEELDESIGN_H__
#define __WHEELDESIGN_H__

#include <ode/ode.h>
#include "design.h"
#include "wheel.h"

class WheelDesign : public Design {
protected:
	dReal R;
public:
	virtual WheelID create(dWorldID world, dSpaceID space);
	void setRadius(dReal _rad) { R = _rad; };
	dReal getRadius() { return R; };
};

typedef WheelDesign *WheelDesignID;

#endif	// __WHEELDESIGN_H__
