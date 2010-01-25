#ifndef __LINKBOXDESIGN_H__
#define __LINKBOXDESIGN_H__

#include <ode/ode.h>
#include "common.h"
#include "myobj.h"

#include "linkdesign.h"

class LinkBoxDesign:public LinkDesign {
      private:
	dReal sides[3];
      public:
	LinkBoxDesign(dReal x, dReal y, dReal z) {
		sides[XX] = x;
		sides[YY] = y;
		sides[ZZ] = z;
	};
	dReal getJJLength() {
		return sides[XX] * 1.2;
	};
	dReal getWidth() {
		return sides[YY];
	};
	dReal getDepth() {
		return sides[ZZ];
	};
	dReal calcSprocketRadius(int num);
	dReal calcJointArrow();
	int getNB() {
		return 1;
	};
	int getNG() {
		return 1;
	};
	// Modifiers
	void setJJLength(dReal new_jj);

	// Factory
	void create(dWorldID world, dSpaceID space, dGeomID * geom,
		    dBodyID * body);
};

#endif				// __LINKBOXDESIGN_H__
