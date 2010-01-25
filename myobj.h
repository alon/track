#ifndef __MYOBJ_H__
#define __MYOBJ_H__

#include "error.h"
#include <ode/ode.h>

class MyObject;

typedef MyObject *MyObjectID;

// this is also a linked list
class MyObject {
      protected:
	dWorldID world;
	dSpaceID space;
	int drawFlag;
      public:
	int show_forces;	// whether Draw displays force vectors
	MyObjectID next;
	dGeomID *geom;
	dBodyID *body;
	dJointID *joint;
	int g_num;
	int b_num;
	int j_num;
	int camera_ind;		// index to body LookHere uses

	void Display();

	MyObject(dWorldID n_world, dSpaceID n_space, int b_num =
		  0, int j_num = 0, int g_num = 0);
	void Init(int b_num = 0, int j_num = 0, int g_num = 0);

	void Merge(MyObjectID b, int reallocate = 0);

	virtual void setDrawFlag(int flag) { drawFlag = flag;};
	virtual int getDrawFlag() { return drawFlag;};
	void toggleDrawFlag() { setDrawFlag(1-getDrawFlag()); }
	
	virtual void Draw();	// draw geom[*] using mydraw drawGeom
	void DrawChain();	// draw this and next if not null
	void LookHere();	// aimCamera(body[camera_body])
	void SetCameraBody(int num) {
		camera_ind = num;
		CONSTRAIN_INT(camera_ind, 0, b_num);
	};
	void ZeroUserData();	// for all_bodies dBodySetData(body[i],0)

	void DestroyContents();
	virtual ~ MyObject();	// doesn't destroy the contents = dGeomDestroy, dBodyDestroy, dJointDestroy
};

// a linked list of MyObject
class MyContainer {
      public:
	MyObjectID first;
	MyContainer() {
		first = 0;
	};
	void Add(MyObjectID obj) {
		if (!first) {
			first = obj;
		} else {
			obj->next = first;
			first = obj;
		}
	}
};

typedef MyContainer *MyContainerID;

#endif				// __MYOBJ_H__
