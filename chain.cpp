// track class 1
//

#include <drawstuff/drawstuff.h>
#include "chain.h"
#include "trackdesign.h"
#include "mydraw.h"
#include "track.h"

// internal defines
#define LINK_MASS 0.1f

int Chain::showbit[LINK_PARTS];	// used to selectively display parts of each link


/********************************************************************************
 * 										*
 *					Chain					*
 * 										*
 ********************************************************************************/

// creating a chain

Chain::Chain(dWorldID _world, dSpaceID _space, TrackDesignID design,
	     LinkDesignID n_link_design)
:  MyObject(_world, _space), link_design(n_link_design)
{
	int i;

	// misc init
	for (i = 0; i < LINK_PARTS; ++i)
		showbit[i] = 1;

	// extract design parameters

	const dReal *angles = design->getAngles();
	// link connections are symmetric about center of mass, which 
	dReal l_2 = design->getJJLength() / 2.0;	// distance from joint to joint
	int n = design->getNumOfLinks();
	dReal init_pos[3];	// position of first link
	design->getLinkPosition(init_pos, 0);
	const dReal *init_R = design->getLinkZeroMatrix();
	int geom_in_link = link_design->getNG();

	// init body, geom and joint arrays
	Init(n, n, n * geom_in_link);

	link_design->create(world, space, geom, body);	// allocates body[0], geom[0..LINK_PARTS-1]
	dBodySetPosition(body[0], init_pos[0], init_pos[1], init_pos[2]);
	dBodySetRotation(body[0], init_R);

	// repeat for n-1 links: create link. change its position.
	for (i = 1; i < n; ++i) {
		dGeomID *link_geom = &geom[geom_in_link * i];	// very dangerous
		dBodyID *link_body = &body[i];
		link_design->create(world, space, link_geom, link_body);

		dBodyID last_link = body[i - 1];
		dBodyID link = body[i];
		// get position of last link
		const dReal *pos = dBodyGetPosition(last_link);
		const dReal *R = dBodyGetRotation(last_link);
		dReal x_axis[3] = { R[0], R[4], R[8] };
		dReal z_axis[3] = { R[2], R[6], R[10] };
		// set position of next link
		dReal alpha = angles[i - 1];
		dReal ca = cos(alpha);
		dReal sa = sin(alpha);
		dBodySetPosition(link,
				 pos[XX] + l_2 * (x_axis[XX] * (1 + ca) +
						  z_axis[XX] * sa),
				 pos[YY] + l_2 * (x_axis[YY] * (1 + ca) +
						  z_axis[YY] * sa),
				 pos[ZZ] + l_2 * (x_axis[ZZ] * (1 + ca) +
						  z_axis[ZZ] * sa)
		    );
		// set orientation of next link
		dMatrix3 rot, NewR;
		dRFromAxisAndAngle(rot, R[1], R[5], R[9], -alpha);
		dMultiply0(NewR, rot, R, 3, 3, 3);
		dBodySetRotation(link, NewR);
	}

	// add joints
	for (i = 0; i < n - 1; ++i) {
		joint[i] = CreateLinkHinge(world, body[i], body[i + 1], l_2);
	}

	joint[n - 1] = CreateLinkHinge(world, body[n - 1], body[0], l_2);

	// misc variables
	joint_arrow = link_design->calcJointArrow();


	//Display();
}

void Chain::Draw()
{
	int k, i, g_i;
	dReal axis[3], end[3], start[3];
	int geom_in_link = link_design->getNG();
	// Draw Chain Links
	for (i = 0; i < b_num; i++) {
		g_i = i * geom_in_link;
		dBodyID one_link = dGeomGetBody(geom[g_i]);
		int color = (int)dBodyGetData(one_link);
		dsSetColor(color, 1, 1); // color == 0 is light blue
		for (k = 0; k < geom_in_link; k++, g_i++)
			if (showbit[k])
				drawGeom(geom[g_i]);
		//const dReal* pos = dBodyGetPosition(body[i]);
		//const dReal* R = dBodyGetRotation(body[i]);
		//for ( k = 0 ; k < 3 ; ++k )
		//      end[k] = pos[k] + R[1+k*4] * joint_arrow * 2; // R -> y axis
		//dsDrawLine(pos, end);
		// Draw forces
		//drawForces(body[i]);
	}
	dsSetColor(0, 1, 1);	// light blue
	// Draw Chain Joints
	for (i = 0; i < j_num; i++) {
		dJointGetHingeAnchor(joint[i], start);
		dJointGetHingeAxis(joint[i], axis);	// hope this is normalized
		for (k = 0; k < 3; ++k)
			end[k] = start[k] + axis[k] * joint_arrow;
		dsDrawLine(start, end);
	}
}

// Create a joint to connect two links. Their length is l_2 * 2
dJointID Chain::CreateLinkHinge(dWorldID world, dBodyID one, dBodyID two, dReal l_2) 
{
	dJointID joint = dJointCreateHinge(world, 0);
	const dReal *R = dBodyGetRotation(one);
	dJointAttach(joint, one, two);
	dJointSetHingeAxis(joint, R[1], R[5], R[9]);
	const dReal *pos = dBodyGetPosition(one);
	dJointSetHingeAnchor(joint, pos[XX] + l_2 * R[0],
			      pos[YY] + l_2 * R[4], pos[ZZ] + l_2 * R[8]);
	
	//dJointSetHingeParam(joint, dParamLoStop, -M_PI/9);
	//dJointSetHingeParam(joint, dParamHiStop, M_PI/9);
	return joint;
}
