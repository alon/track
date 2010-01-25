#include "apphalf.h"

////////////////////////////////////////////////////////////////////////////////
// Left/Right Half of appendage - beam+2 pistons+piston holder creation
// Creator

AppHalf::AppHalf(dWorldID world, dSpaceID space, AppDesignID app_design,
		 dBodyID main_body, dBodyID blade)
:  ebMain(main_body), ebBlade(blade)
{
	// NOTE: app_design must have a side called before this, so that A..E and K..Q are one of left or right

	// create bodies with geometries

	// create links and connect them
	jA = dJointCreateHinge(world, 0);
	dJointSetHingeAxis(jA, 0, 1, 0);	// is -1 or 1 a matter? only for angle and anglerate sign, i think.
	dReal *A = app_design->getA();
	dJointSetHingeAnchor(jA, A[0], A[1], A[2]);
	dJointConnect(jA, ebMain, bBeam);
}
