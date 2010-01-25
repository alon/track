#ifndef __MBOXLINKDESIGN_H__
#define __MBOXLINKDESIGN_H__

#include <ode/ode.h>
#include "common.h"
#include "myobj.h"

#include "linkdesign.h"

// a very dumb name - it has 8 geometries now (was 7)
class MBoxLinkDesign:public LinkDesign {
      private:
	dReal(*link_sides)[3];
      public:
	MBoxLinkDesign(dReal(*the_link_sides)[3]) {
		link_sides = the_link_sides;
	};
	dReal getJJLength() {
		return link_sides[1][XX] + link_sides[2][XX] * 9 / 8;
	};
	dReal getWidth() {
		return link_sides[7][YY];
	};
	dReal getDepth() {
		return link_sides[0][ZZ];
	};
	dReal calcSprocketRadius(int num);
	dReal calcJointArrow();
	int getNB() {
		return 1;
	};
	int getNG();		// returns LINK_NUM, but i don't want to define that here (even with an include)
	// Modifiers
	void setJJLength(dReal new_jj);
	
	void create(dWorldID world,
		    dSpaceID space, dGeomID * geom, dBodyID * body);
};

#endif	// __MBOXLINKDESIGN_H__
