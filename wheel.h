#ifndef __WHEEL_H__
#define __WHEEL_H__

#include "myobj.h"

class Wheel:public MyObject {
      public:
	Wheel(dWorldID world, dSpaceID space);
};

typedef Wheel *WheelID;

#endif	//  __WHEEL_H__
