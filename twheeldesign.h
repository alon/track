#ifndef __TWHEELDESIGN_H__
#define __TWHEELDESIGN_H__

#include <ode/ode.h>
#include "common.h"
#include "myobj.h"
#include "wheeldesign.h"
#include "twheel.h"

class TWheelDesign : public WheelDesign {
      private:
	int sprocket_num;
	dReal teeth_h_fuzz;
	dReal t_sides[3];
      public:
	TWheelDesign(dReal _teeth_h_fuzz, dReal _t_sides[3], int _sprocket_num);
	void setTeethNum(int _num) {
		sprocket_num = _num;
	};
	int getTeethNum() {
		return sprocket_num;
	};
	dReal getRadius() {
		return R;
	};
	dReal getTeethHFuzz() {
		return teeth_h_fuzz;
	};
	dReal *getTeethSides() {
		return t_sides;
	};
	WheelID create(dWorldID world, dSpaceID space) {
		return new TWheel(world, space, this);
	};
};

typedef TWheelDesign *TWheelDesignID;

#endif				// __TWHEELDESIGN_H__
