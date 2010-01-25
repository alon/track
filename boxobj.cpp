#include "boxobj.h"
#include "mydraw.h"

BoxObject::BoxObject(dWorldID world, dSpaceID space, const dReal sides[3], dReal mass):MyObject(world, space, 1, 0, 1)	// one body, zero joints, one geom
{
	static dMass m;
	body[0] = dBodyCreate(world);
	geom[0] = dCreateBox(space, sides[0], sides[1], sides[2]);
	dGeomSetBody(geom[0], body[0]);
    dReal temp_mass = 1.0;
	dMassSetBox(&m, temp_mass, sides[0], sides[1], sides[2]);
	dMassAdjust(&m, mass);
	dBodySetMass(body[0], &m);
}

void BoxObject::Draw()
{
	// draw body CCS (centered coordinate system)
	drawGeom(geom[0]);
	//drawCCS(body[0], 1.0);
	//drawForces(body[0]);
};
