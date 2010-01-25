#ifndef __LINKBALLDESIGN_H__
#define __LINKBALLDESIGN_H__

#include <ode/ode.h>
#include "common.h"
#include "myobj.h"

#include "linkdesign.h"

class LinkBallDesign:public LinkDesign {
      private:
      public:
	LinkBallDesign() {
	};
	void create(dWorldID world, dSpaceID space, dGeomID * geom,
		    dBodyID * body);
};

#endif				// __LINKBALLDESIGN_H__
