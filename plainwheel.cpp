#include <drawstuff/drawstuff.h>
#include "plainwheel.h"
#include "plainwheeldesign.h"
#include "mydraw.h"
#include "track.h"

#include "error.h"

/************************************************************************
 *				PlainWheel				*
 ************************************************************************/

void PlainWheel::Draw()
{
	if (drawFlag) {
		// to draw the (real) sphere
		//drawGeom(geom[0]);
		// to draw a prettier looking (though false) cylinder
		drawCylinder(geom[0]);
	}
}

PlainWheel::PlainWheel(dWorldID world, dSpaceID space, PlainWheelDesignID d)
:  Wheel(world, space)
{
	create(d->getRadius());
}

void PlainWheel::create(dReal radius)
{
	dBodyID sprocket;

	dMass m;

	Init(1, 0, 1);
	
	body[0] = sprocket = dBodyCreate(world);
	dBodySetPosition(sprocket, 0, 0, 0);
	dMassSetBox(&m, 1, 2*radius, 1, 1);	// will give mass of 1
	dMassAdjust(&m, SPROCKET_MASS);
	dBodySetMass(sprocket, &m);

	// create central wheel geom

	dGeomID g2;
	geom[0] = dCreateGeomTransform(space);
	dGeomTransformSetCleanup(geom[0], 1);
	g2 = dCreateSphere(0, radius);//, height); // TODO: dCreateCylinder
	dReal R[12] = {	1,0,0,0,
			0,0,-1,0,
			0,1,0,0};
	dGeomSetRotation(g2, R);
	dGeomTransformSetGeom(geom[0], g2);
	dGeomSetBody(geom[0], sprocket);
}
