#ifndef __APPHALF_H__
#define __APPHALF_H__

#include "myobj.h"
#include "appdesign.h"

class AppHalf:public MyObject {
      private:
	// external - not created here
	dBodyID ebMain, ebBlade;
	// body geom pairs. 
	dBodyID bBeam, bCBPistonA, bCBPistonB, bDEPistonA, bDEPistonB;
	dGeomID gBeam, gCBPistonA, gCBPistonB, gDEPistonA, gDEPistonB;
	// sole geom
	dGeomID gPistonHolder[2];
	// links
	dJointID jA, jB, jB_star, jC, jD, jE;
      public:
	 AppHalf(dWorldID world,
		 dSpaceID space,
		 AppDesignID app_design, dBodyID main_body, dBodyID blade);
};

#endif				// __APPHALF_H__
