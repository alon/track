#include "mboxlinkdesign.h"
#include "error.h"
#include "myobj.h"
#include "track.h"

// forward declaration (implementation at end of file)
void CreateLink(dWorldID world, dSpaceID space, 
		dGeomID * geom, dBodyID * body,
		const dReal(*link_sides)[3]);

int MBoxLinkDesign::getNG()
{
	return LINK_PARTS;
};

void MBoxLinkDesign::create(dWorldID world, dSpaceID space, dGeomID * geom,
			 dBodyID * body)
{
	CreateLink(world, space, geom, body, link_sides);
};

dReal MBoxLinkDesign::calcSprocketRadius(int num)
{
	dReal rad = getJJLength() / 2 / tan(M_PI / num);
	return rad - link_sides[0][ZZ] / 2;	// this is without " - spacer"
};

dReal MBoxLinkDesign::calcJointArrow()
{
	return link_sides[0][YY] + link_sides[2][YY] + link_sides[4][YY];
}

void MBoxLinkDesign::setJJLength(dReal new_jj)
{
	dReal cur_jj = getJJLength();
	for (int i = 0; i < LINK_PARTS; ++i) link_sides[i][XX] *= new_jj / cur_jj;
}

////////////////////////////////////////////////////////////
// CreateLinkGeom
//
// creates LINK_PARTS geometries
// (using CreateGeom. no "new" operator is used directly)
//
////
int CreateLinkGeom(dSpaceID space, dGeomID * link_g, dGeomID * g2,
		    const dReal(*link_sides)[3]) 
{
	if (link_g == NULL || g2 == NULL) {
		std::
		    cout <<
		    "warning: CreateLinkGeom called with null pointers\n";
		return 0;
	}
	
	// create link geometry.
	// it has some lee way, the hard coded stuff is the relation between the pieces
	// and not actual lengths.
	
	// create geoms (real and child) and give them correct size
	for (int j = 0; j < LINK_PARTS; ++j) {
		link_g[j] = dCreateGeomTransform(space);
		g2[j] = dCreateBox(0, link_sides[j][0], link_sides[j][1],
			       link_sides[j][2]);
		dGeomTransformSetGeom(link_g[j], g2[j]);
	} 
	// give correct position
	dGeomSetPosition(g2[0], -(link_sides[1][XX] + link_sides[0][XX]) / 2,
			     (2 * link_sides[1][YY] + link_sides[5][YY] -
			     link_sides[0][YY]) / 2, 0);
	dGeomSetPosition(g2[1], 0, (link_sides[1][YY] + link_sides[5][YY]) / 2, 0);
	dGeomSetPosition(g2[2], (link_sides[1][XX] + link_sides[2][XX]) / 2, 0, 0);
	dGeomSetPosition(g2[3], 0, -(link_sides[3][YY] + link_sides[5][YY]) / 2, 0);
	dGeomSetPosition(g2[4], -(link_sides[3][XX] + link_sides[4][XX]) / 2,
			  -(2 * link_sides[3][YY] + link_sides[5][YY] -
			     link_sides[4][YY]) / 2, 0);
	dGeomSetPosition(g2[5], -(link_sides[1][XX] - link_sides[5][XX]) / 2, 0, 0);
	dGeomSetPosition(g2[6], (link_sides[1][XX] - link_sides[6][XX]) / 2, 0, 0);
	
	dReal outer_part_z = (link_sides[0][ZZ] + link_sides[7][ZZ]) / 2.0;
	
	dGeomSetPosition(g2[7], 0, 0, -outer_part_z);
	return LINK_PARTS;
}


// recieves MyObjectID assuming it has preallocated body, geom, joint
void CreateLink(dWorldID world, dSpaceID space, 
		dGeomID * geom, dBodyID * body,
		const dReal(*link_sides)[3]) 
{
	int i;
	
	// PLAN
	// ====
	// create enough id's
	// allocate mass
	// allocate bodies
	// allocate joints(or none)
	// allocate geoms
	//
	//dGeomID* g2 = new dGeomID[LINK_PARTS];
	dGeomID g2[LINK_PARTS];
	
	// Body
	// ====
	dBodyID link;
	body[0] = link = dBodyCreate(world);
	
	// Geom
	// ====
	// TODO: use a group geom
	int g_num = CreateLinkGeom(space, geom, g2, link_sides);
	for (i = 0; i < g_num; ++i)
		dGeomSetBody(geom[i], link);
	
	// Mass
	// ====
	dMass m;
	dMassSetZero(&m);
	for (i = 0; i < LINK_PARTS; ++i) {
		dMass m2;
		dMassSetBox(&m2, 1, link_sides[i][XX], link_sides[i][YY],
			     link_sides[i][ZZ]);
		const dReal *pos = dGeomGetPosition(g2[i]);
		dMassTranslate(&m2, pos[0], pos[1], pos[2]);
		pos = dGeomGetRotation(g2[i]);
		dMassRotate(&m2, pos);
		dMassAdd(&m, &m2);
	} 
	//PEXP(m.c[0]);
	//PEXP(m.c[1]);
	//PEXP(m.c[2]);
	    
	// TODO: move all encapsulated objects so that the center of mass is (0,0,0)
    {
        for ( i = 0 ; i < LINK_PARTS ; i++) {
            const dReal *pos = dGeomGetPosition(g2[i]);
            dGeomSetPosition (g2[i], pos[0] - m.c[0], pos[1] - m.c[1],
                                     pos[2] - m.c[2]);
        }
        dMassTranslate (&m, -m.c[0], -m.c[1], -m.c[2]);
    }
	dMassAdjust(&m, LINK_MASS);
	dBodySetMass(link, &m);
	
	//delete g2;
} 

