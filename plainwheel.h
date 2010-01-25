#ifndef __PLAINWHEEL_H__
#define __PLAINWHEEL_H__

#include "wheel.h"
#include "track.h"

class PlainWheelDesign;
typedef PlainWheelDesign* PlainWheelDesignID;

class PlainWheel:public Wheel {
private:
	dReal radius;
public:
	PlainWheel(dWorldID world, dSpaceID space, PlainWheelDesignID d);
	void create(dReal radius);
	void Draw();
};

typedef PlainWheel *PlainWheelID;

#endif	//  __PLAINWHEEL_H__
