// track class 1
//

#include <drawstuff/drawstuff.h>
#include "twheel.h"
#include "twheeldesign.h"
#include "mydraw.h"
#include "track.h"

/************************************************************************
 *			Toothed	Wheel					*
 ************************************************************************/

void TWheel::Draw()
{
	dsSetColor(1, 1, 0);	// yellow
	//dsDrawCylinder (dBodyGetPosition(sprocket), dBodyGetRotation(sprocket)
//                , (float) height// *sin(dStopwatchTime(&timer)/2.0)
//                , (float) radius);
	// Sprocket Teeth
	if (g_num > 0) {
		dsSetColor(2.0, 0.5, 0.5);
		drawGeom(geom[0]);
	};
	if (g_num > 1) {
		dsSetColor(0.5, 2.0, 0.5);
		drawGeom(geom[1]);
	}
	if (g_num > 2) {
		dsSetColor(0.5, 0.5, 2.0);
		drawGeom(geom[2]);
	}
	dsSetColor(1, 0.5, 0);
	for (int j = 3; j < g_num; j++) {
		drawGeom(geom[j]);
	}
}

TWheel::TWheel(dWorldID world, dSpaceID space, TWheelDesignID d)
:  Wheel(world, space)
{
	create(d->getTeethNum(),
	       d->getRadius(), d->getTeethSides(), d->getTeethHFuzz());
}

TWheel::TWheel(dWorldID world, dSpaceID space, int tnum, dReal radius,
	       dReal * t_sides, dReal teeth_h_fuzz)
:Wheel(world, space)
{
	create(tnum, radius, t_sides, teeth_h_fuzz);
}

void TWheel::create(int tnum, dReal radius,
		    dReal * t_sides, dReal teeth_h_fuzz)
{
	int i, j;
	int g_num = tnum * 2;	// each tooth is two geometries.
	// tooth i = geom[i], geom[i+tnum]
#ifdef CENTRAL_WHEEL
	g_num++;
#endif

	Init(1, 0, g_num);

	dBodyID sprocket;

	dMass m;

	body[0] = sprocket = dBodyCreate(world);
	dBodySetPosition(sprocket, 0, 0, 0);
	dMassSetBox(&m, 1, 1, 1, 1);	// will give mass of 1
	dMassAdjust(&m, SPROCKET_MASS);
	dBodySetMass(sprocket, &m);

	// create central wheel geom

	dGeomID g2, g3;		// "child" geom
#ifdef CENTRAL_WHEEL
	geom[num * 2] = dCreateGeomTransform(space);
	g2 = dCreateSphere(0, radius);	//, height); // TODO: dCreateCylinder
	dGeomTransformSetCleanup(geom[num * 2], 1);
	dGeomTransformSetGeom(geom[num * 2], g2);
	dGeomSetBody(geom[num * 2], sprocket);
#endif
	// create teeth (meanwhile, neglecting their mass)
	for (i = 0; i < tnum; ++i) {
		// create real geom
		for (j = 0; j <= tnum; j += tnum) {
			geom[i + j] = dCreateGeomTransform(space);
			dGeomTransformSetCleanup(geom[i + j], 1);
		}
		// Create The Tooth
#ifndef CYLINDER
		g2 = dCreateBox(0, t_sides[XX], t_sides[YY], t_sides[ZZ]);	// why the 0 and not space?
#else
		g2 = dCreateCCylinder(0, /*radius*/t_sides[ZZ], MAX(t_sides[ZZ]/10,t_sides[YY]-2*t_sides[ZZ])/*length*/);
#endif
		// Create The Bracer
		g3 = dCreateBox(0, t_sides[ZZ], t_sides[YY],
				t_sides[XX] * 2);
		// attach "child" to real
		dGeomTransformSetGeom(geom[i], g2);
		dGeomTransformSetGeom(geom[i + tnum], g3);
		// transform tooth
		dReal angle = (M_PI * 2 / (dReal) tnum) * ((dReal) i);
		dReal link_r = radius - teeth_h_fuzz + t_sides[XX] / 2;
		dGeomSetPosition(g2, link_r * cos(angle), 0,
				 link_r * sin(angle));
		dQuaternion quat;
		dQFromAxisAndAngle(quat, 0, 1, 0, -angle);
		dMatrix3 Rtx;
		dQtoR(quat, Rtx);
#ifdef CYLINDER
		// add rotation of x and y because of way the ccylinder is created
		dReal t[3];
		t[0] = -Rtx[0]; t[1] = -Rtx[4]; t[2] = -Rtx[8];
		Rtx[0] = Rtx[1]; Rtx[4] = Rtx[5]; Rtx[8] = Rtx[9];
		Rtx[1] = t[0]; Rtx[5] = t[1]; Rtx[9] = t[2];
#endif
		dGeomSetRotation(g2, Rtx);
		// transform bracer
		link_r += teeth_h_fuzz - (t_sides[ZZ] + t_sides[XX]) / 2;
		dGeomSetPosition(g3, link_r * cos(angle), 0,
				 link_r * sin(angle));
		dGeomSetRotation(g3, Rtx);
		// attach real geom to body
		for (j = 0; j <= tnum; j += tnum)
			dGeomSetBody(geom[i + j], sprocket);
	}

	// calculate angle of first tooth
	//const dReal* pos = dGeomGetPosition(dGeomTransformGetGeom(geom[0]));
	//return atan2(pos[XX], pos[ZZ]);
}
