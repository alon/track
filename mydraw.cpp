#include <ode/ode.h>
#include "drawstuff/drawstuff.h"

#include "common.h"
#include "mydraw.h"
#include <iostream>

// select correct drawing functions (Alon: this should not be here)

// draw a geom (from test_boxstack)

void drawGeom(dGeomID g, const dReal * pos, const dReal * R)
{
	if (!g)
		return;
	if (!pos)
		pos = dGeomGetPosition(g);
	if (!R)
		R = dGeomGetRotation(g);

	int type = dGeomGetClass(g);
	if (type == dBoxClass) {
		dVector3 sides;
		dGeomBoxGetLengths(g, sides);
		dsDrawBox(pos, R, sides);
	} else if (type == dSphereClass) {
		dsDrawSphere(pos, R, dGeomSphereGetRadius(g));
	} else if (type == dCCylinderClass) {
		dReal radius, length;
		dGeomCCylinderGetParams(g, &radius, &length);
		dsDrawCappedCylinder(pos, R, length, radius);
	} else if (type == dGeomTransformClass) {
		dGeomID g2 = dGeomTransformGetGeom(g);
		const dReal *pos2 = dGeomGetPosition(g2);
		const dReal *R2 = dGeomGetRotation(g2);
		dVector3 actual_pos;
		dMatrix3 actual_R;
		dMULTIPLY0_331(actual_pos, R, pos2);
		actual_pos[0] += pos[0];
		actual_pos[1] += pos[1];
		actual_pos[2] += pos[2];
		dMULTIPLY0_333(actual_R, R, R2);
		drawGeom(g2, actual_pos, actual_R);
	}
}

void drawCylinder(dGeomID g, const dReal * pos, const dReal * R)
{
	if (!g)
		return;
	if (!pos)
		pos = dGeomGetPosition(g);
	if (!R)
		R = dGeomGetRotation(g);

	int type = dGeomGetClass(g);
	if (type == dBoxClass) {
	} else if (type == dSphereClass) {
		float radius = dGeomSphereGetRadius(g);
		float length = radius / 10.0;
		dsDrawCylinder (pos, R, length, radius);
	} else if (type == dCCylinderClass) {
		dReal radius, length;
		dGeomCCylinderGetParams(g, &radius, &length);
		dsDrawCylinder(pos, R, length, radius);
	} else if (type == dGeomTransformClass) {
		dGeomID g2 = dGeomTransformGetGeom(g);
		const dReal *pos2 = dGeomGetPosition(g2);
		const dReal *R2 = dGeomGetRotation(g2);
		dVector3 actual_pos;
		dMatrix3 actual_R;
		dMULTIPLY0_331(actual_pos, R, pos2);
		actual_pos[0] += pos[0];
		actual_pos[1] += pos[1];
		actual_pos[2] += pos[2];
		dMULTIPLY0_333(actual_R, R, R2);
		drawCylinder(g2, actual_pos, actual_R);
	}
}

void drawCCS(dBodyID bod, dReal ax_len)
{
	int i;
	const dReal *pos = dBodyGetPosition(bod);
	const dReal *R = dBodyGetRotation(bod);
	// draw X, Y, Z axes
	dReal end[3];
	for (i = 0; i < 3; ++i)
		end[i] = pos[i] + ax_len * R[4 * i];
	dsDrawLine(pos, end);
	for (i = 0; i < 3; ++i)
		end[i] = pos[i] + ax_len * R[1 + 4 * i];
	dsDrawLine(pos, end);
	for (i = 0; i < 3; ++i)
		end[i] = pos[i] + ax_len * R[2 + 4 * i];
	dsDrawLine(pos, end);
}

void drawForces(dBodyID bod)
{
	/*
	   const dReal* pos = dBodyGetPosition(bod);
	   const dReal* force = dBodyGetOldForce(bod);
	   dReal end[3];
	   end[XX] = pos[XX] + force[XX];
	   end[YY] = pos[YY] + force[YY];
	   end[ZZ] = pos[ZZ] + force[ZZ];
	   dsDrawLine(pos, end);
	 */
}


// creates a level view at the center of body. meaning h and p are calculated,
// and r = 0.
void aimCamera(dBodyID body)
{
	const dReal *pos = dBodyGetPosition(body);
	float xyz[3], hpr[3], delta[3];
	int i;

	dsGetViewpoint(xyz, hpr);
	for (i = 0; i < 3; ++i)
		delta[i] = pos[i] - xyz[i];

	hpr[0] = 180.0 / M_PI * atan2(delta[YY], delta[XX]);	// heading
	hpr[1] = 180.0 / M_PI * atan2(delta[ZZ], sqrt(delta[XX] * delta[XX] + delta[YY] * delta[YY]));	// pitch
	//std::cout << "debug: heading: " << hpr[0] << "\n";
	//std::cout << "debug: pitch: " << hpr[1] << "\n";
	hpr[2] = 0;
	dsSetViewpoint(xyz, hpr);
}

// creates a level view at the center of body. meaning h and p are calculated,
// and r = 0.
// also follow to a defined distance away - between to numbers
void aimCamera(dBodyID body, dReal min, dReal max)
{
	const dReal *pos = dBodyGetPosition(body);
	float xyz[3], hpr[3], delta[3];
	int i;

	dsGetViewpoint(xyz, hpr);
	for (i = 0; i < 3; ++i)
		delta[i] = pos[i] - xyz[i];

	hpr[0] = 180.0 / M_PI * atan2(delta[YY], delta[XX]);	// heading
	hpr[1] = 180.0 / M_PI * atan2(delta[ZZ], sqrt(delta[XX] * delta[XX] + delta[YY] * delta[YY]));	// pitch
	//std::cout << "debug: heading: " << hpr[0] << "\n";
	//std::cout << "debug: pitch: " << hpr[1] << "\n";
	hpr[2] = 0;

	dReal dist =
	    sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
	if (dist > max) {
		dReal part =
		    (dist - max) / sqrt(delta[0] * delta[0] + delta[1] +
					delta[1] + delta[2] * delta[2]);
		for (i = 0; i < 3; ++i)
			xyz[i] += part * delta[i];
	}

	dsSetViewpoint(xyz, hpr);
}
