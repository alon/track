#ifndef __TRACK_H__
#define __TRACK_H__

#define LINK_PARTS 8
#define LINK_MASS 0.1f
#define SPROCKET_MASS 1

// Link, Chain
//void CreateLink(dWorldID world, dSpaceID space,
//		dGeomID * geom, dBodyID * body,
//		const dReal(*link_sides)[3]);

//dJointID CreateLinkHinge(dWorldID world, dBodyID one, dBodyID two,
//			 dReal l_2);

enum { ITYPE = 0, INUM };	// used for the orders parameter
enum { ILINEAR = 0, ICIRCULAR };	// to CreateComplexChain
#endif				// __TRACK_H__
