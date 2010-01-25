#ifndef __BOX_OBJ_H__
#define __BOX_OBJ_H__

#include "myobj.h"

class BoxObject:public MyObject {
      public:
	BoxObject(dWorldID world, dSpaceID space, const dReal sides[3],
		  dReal mass);
	void Draw();
};

#endif				// __BOX_OBJ_H__
