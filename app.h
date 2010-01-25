#ifndef __APP_H__
#define __APP_H__

#include "appdesign.h"
#include "apphalf.h"

class App:public MyObject {
      private:
	// external
	dBodyID ebMain;
	// blade body + geom
	dBodyID bBlade;
	dGeomID gBlade;
	AppHalf half[2];
	dJointID EJoint[2], BJoint[2], BstarJoint[2];

      public:
	 App(dWorldID world, dSpaceID space, AppDesignID app_design,
	     dBodyID main_body);
};

typedef App *AppID;

#endif				// __APP_H__
