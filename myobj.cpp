#include <iostream>
#include <cstdio>
#include <ode/ode.h>
#include "myobj.h"
#include "mydraw.h"
#include "error.h"
#include "common.h"

using namespace std;

// Allocate

void MyObject::Init(int n_b_num, int n_j_num, int n_g_num)
{
	DestroyContents();
	show_forces = 0;
	j_num = n_j_num;
	b_num = n_b_num;
	g_num = n_g_num;
	if (g_num) {
		geom = new dGeomID[g_num];
		for (int i = 0; i < g_num; ++i)
			geom[i] = 0;
		ASSERT(geom);
	};
	if (g_num) {
		joint = new dJointID[j_num];
		for (int i = 0; i < j_num; ++i)
			joint[i] = 0;
		ASSERT(joint);
	}
	if (b_num) {
		body = new dBodyID[b_num];
		for (int i = 0; i < b_num; ++i)
			body[i] = 0;
		ASSERT(body);
	}
}

MyObject::MyObject(dWorldID n_world, dSpaceID n_space,
		   int n_b_num, int n_j_num, int n_g_num):world(n_world),
space(n_space), next(0), geom(0), body(0), joint(0), g_num(0), b_num(0),
j_num(0), camera_ind(0)
{
	Init(n_b_num, n_j_num, n_g_num);
}

// Deallocate

// Delete / Destroy (Destroy means using the dGeomDestroy and the like,
// delete means using the delete operator only, which means not affecting
// the actual geoms, joints and bodies)

void MyObject::DestroyContents()
{
	if (geom) {
		for (int i = 0; i < g_num; ++i) {
			if (geom[i]) {
				if (dGeomGetClass(geom[i]) ==
				    dGeomTransformClass)
					dGeomDestroy(dGeomTransformGetGeom
						     (geom[i]));
				dGeomDestroy(geom[i]);
				geom[i] = 0;
			}
		};
		delete geom;
	};
	if (joint) {
		for (int i = 0; i < j_num; ++i) {
			if (joint[i]) {
				dJointDestroy(joint[i]);
				joint[i] = 0;
			}
		}
		delete joint;
	};
	if (body) {
		for (int i = 0; i < b_num; ++i) {
			if (body[i]) {
				dBodyDestroy(body[i]);
				body[i] = 0;
			}
		}
		delete body;
	};
	j_num = g_num = b_num = 0;
}

MyObject::~MyObject()
{
	DestroyContents();
}

// Merge
void MyObject::Merge(MyObjectID b, int reallocate)
{
	int i, this_i;
	if (reallocate) {
		// reallocate a to have space for both
		dJointID *n_joint = new dJointID[j_num + b->j_num];
		dGeomID *n_geom = new dGeomID[g_num + b->g_num];
		dBodyID *n_body = new dBodyID[b_num + b->b_num];
		for (i = 0; i < j_num; ++i)
			n_joint[i] = joint[i];
		for (i = 0; i < g_num; ++i)
			n_geom[i] = geom[i];
		for (i = 0; i < b_num; ++i)
			n_body[i] = body[i];
		delete joint;
		delete body;
		delete geom;
		joint = n_joint;
		body = n_body;
		geom = n_geom;
	}
	// copy b to a
	for (i = 0, this_i = j_num; i < b->j_num; ++i, ++next)
		joint[this_i] = b->joint[i];
	for (i = 0, this_i = g_num; i < b->g_num; ++i, ++next)
		geom[this_i] = b->geom[i];
	for (i = 0, this_i = b_num; i < b->b_num; ++i, ++next)
		body[this_i] = b->body[i];
	// update counters
	j_num += b->j_num;
	g_num += b->g_num;
	b_num += b->b_num;
}

// Copy
/*
MyObjectID MyObject::Copy()
{
  if (j_num > ) {
	  cout << "error: can't copy an object with joints yet.\n";
	  exit(-1);
  };
  dJointID* joint;
  dBodyID* body;
  dGeomID* geom;

  MyObjectID obj = new MyObject(world, space, b_num, j_num, g_num);
  for (int i = 0 ; i < g_num ; ++i ) {

  }
  for (int i = 0 ; i < b_num ; ++i ) {
  }
}
*/


// Display

void MyObject::Display()
{
	int i;
	printf("debug: j %d g %d b %d\n", j_num, g_num, b_num);
	for (i = 0; i < g_num; ++i)
		if (geom[i]) {
			const dReal *pos = dGeomGetPosition(geom[i]);
			printf("debug: geom %d: %3.3f %3.3f %3.3f\n", i,
			       pos[0], pos[1], pos[2]);
		}
	for (i = 0; i < j_num; ++i)
		if (joint[i]) {
			dReal pos[3];
			dJointGetHingeAnchor(joint[i], pos);
			printf("debug: joint %d: %3.3f %3.3f %3.3f\n", i,
			       pos[0], pos[1], pos[2]);
		}
	for (i = 0; i < b_num; ++i)
		if (body[i]) {
			const dReal *pos = dBodyGetPosition(body[i]);
			printf("debug: body %d: %3.3f %3.3f %3.3f\n", i,
			       pos[0], pos[1], pos[2]);
		}
}

void MyObject::Draw()
{
	int i;
	//printf("debug: g_num = %d\n", g_num);
	for (i = 0; i < g_num; ++i) {
		//printf("debug: geom[%d] = %x\n", i, geom[i]);
		if (geom[i]) {
			drawGeom(geom[i]);
		}
	}
	// show forces
	if (show_forces)
		for (i = 0; i < b_num; ++i)
			if (body[i])
				drawForces(body[i]);
}

void MyObject::DrawChain()
{
	Draw();
	if (next)
		next->DrawChain();
}

void MyObject::LookHere()
{
	aimCamera(body[camera_ind]);
}

void MyObject::ZeroUserData()
{
	for (int i = 0 ; i < b_num ; ++i)
		dBodySetData(body[i], 0);
}
