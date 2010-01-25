#include "app.h"

////////////////////////////////////////////////////////////////////////////////
// Appendage
// Creator (uses sub contractor - AppHalf)

App::App(dWorldID world, dSpaceID space, AppDesignID app_design,
	 BladeDesignID blade_design, dBodyID main_body)
:  MyObject(world, space), ebMain(main_body)
{
	// body count:  9 = blade + 2 * ( piston(2) + piston(2) )
	// geom count:  10 = body_num + 1
	// joint count: 12 = 2 * (A,B,C,D,E + piston )
	Init(9, 12, 10);	// b j g

	bBlade = dBodyCreate(world);
	gBlade = blade_design->createGeom(space);

	int i;

	for (i = 0; i < 2; ++i) {
		app_design->setSide(i);	// tell it which side i want
		half[i] = new AppHalf(app_design);
		
		// connect the half to the body and to the blade
		dBodyID B = half[i]->getBBody();
		dBodyID B_star = half[i]->getBstarBody();
		dBodyID E = half[i]->getEBody();

		BJoint = dCreate
	}

	
	return;
}
