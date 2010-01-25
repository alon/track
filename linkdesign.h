#ifndef __LINKDESIGN_H__
#define __LINKDESIGN_H__

#include <ode/ode.h>
#include "common.h"
#include "myobj.h"

#include "design.h"

class LinkDesign : public Design { // sloth is a sin
public:
	// Accesors
	virtual dReal getJJLength() = 0; // distance between joints
	virtual dReal getWidth() = 0;
	virtual dReal getDepth() = 0;
	virtual dReal calcSprocketRadius(int num) = 0;	// radius of sprocket without a spacer
	virtual dReal calcJointArrow() = 0;	// length of arrow representing a joint
	virtual int getNB() = 0;
	virtual int getNG() = 0;
	// Modifiers
	virtual void setJJLength(dReal new_jj) = 0;
	// Factory methods
	virtual void create(dWorldID world, dSpaceID space, dGeomID * geom,
			    dBodyID * body) = 0;
	// Constructors
	LinkDesign();
};

typedef LinkDesign *LinkDesignID;

#endif				// __LINKDESIGN_H__
