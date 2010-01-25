#ifndef __MYDRAW_H__
#define __MYDRAW_H__

void drawGeom(dGeomID g, const dReal * pos = 0, const dReal * R = 0);
void drawCylinder(dGeomID g, const dReal * pos = 0, const dReal * R = 0);
void drawCCS(dBodyID bod, dReal ax_len);
void drawForces(dBodyID bod);
void aimCamera(dBodyID body);
void aimCamera(dBodyID body, dReal min, dReal max);

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCappedCylinder dsDrawCappedCylinderD
#define dsDrawLine dsDrawLineD
#endif

#endif				// __MYDRAW_H__
