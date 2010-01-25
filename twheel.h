// track class 1
//

#ifndef __TWHEEL_H__
#define __TWHEEL_H__

#include "wheel.h"
//#include "twheeldesign.h"
#include "track.h"

class TWheelDesign;
typedef TWheelDesign* TWheelDesignID;

class TWheel:public Wheel {
      public:
	TWheel(dWorldID world, dSpaceID space, int tnum, dReal radius,
	       dReal * t_sides, dReal teeth_h_fuzz);
	TWheel(dWorldID world, dSpaceID space, TWheelDesignID d);
	void create(int tnum, dReal radius, dReal * t_sides,
		    dReal teeth_h_fuzz);
	void Draw();
};

typedef TWheel *TWheelID;

#endif				//  __TWHEEL_H__
