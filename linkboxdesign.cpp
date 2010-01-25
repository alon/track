#include "linkboxdesign.h"
#include "error.h"
#include "myobj.h"
#include "track.h"

void LinkBoxDesign::create(dWorldID world, dSpaceID space, dGeomID * geom,
			   dBodyID * body)
{
	body[0] = dBodyCreate(world);
	geom[0] = dCreateBox(space, sides[XX], sides[YY], sides[ZZ]);
	dGeomSetBody(geom[0], body[0]);
	dMass m;
	dMassSetBox(&m, 1, sides[XX], sides[YY], sides[ZZ]);
	dMassAdjust(&m, LINK_MASS);
	dBodySetMass(body[0], &m);
}

dReal LinkBoxDesign::calcSprocketRadius(int num)
{
	dReal rad = getJJLength() / 2 / tan(M_PI / num);
	return rad - sides[ZZ] / 2;	// this is without " - spacer"
};

dReal LinkBoxDesign::calcJointArrow()
{
	return sides[YY];
}

void LinkBoxDesign::setJJLength(dReal new_jj)
{
	dReal cur_jj = getJJLength();
	sides[XX] *= new_jj / cur_jj;
}
