#ifndef __TRACKDESIGN_H__
#define __TRACKDESIGN_H__

#include <ode/ode.h>
#include "common.h"
#include "myobj.h"

#include "movabledesign.h"
#include "linkdesign.h"

class WheelDesign;
typedef WheelDesign* WheelDesignID;

class TrackDesign:public MovableDesign {
      private:
	dReal radius;
	int link_num;		// number of links
	dReal jj_len;		// joint-joint length
	dReal *angles;		// angles between links (starting from 0-1)
	dReal top[3], back[3], front[3];	// position of wheels (top = sprocket)
	dMatrix3 rot_0;		// orientation of link 0
	dReal(*link_pos)[3];	// link position vectors
	dReal R;		// radius of sprocket

	dReal AABB_max[3], AABB_min[3];	// Axis Aligned Bounding Box for whole track - two corners

      public:
	LinkDesignID link_design;

	// Constructors
/*
	TrackDesign(dReal n_radius,	// curvature radius (=R+spacer)
			int sprocket_teeth,	// number of teeth in sprocket
			const dReal * n_back,	// position of back wheel
			const dReal * n_rot_0=0,	// orientation of link 0
			LinkDesign * n_link_design=0);
*/
	TrackDesign(WheelDesignID wheel_design,
			LinkDesignID n_link_design,
			int links_in_circle,
			dReal spacer,
			const dReal * n_back=0,	// position of back wheel
			const dReal * n_rot_0=0);	// orientation of link 0
	virtual ~TrackDesign();

	// Accesors

	dReal getRadius() {
		return R;
	}
	dReal getJJLength() {
		return jj_len;
	}
	int getNumOfLinks() {
		return link_num;
	}
	void getLinkPosition(dReal * pos_n, int n) {
		pos_n[XX] = link_pos[n][XX];
		pos_n[YY] = link_pos[n][YY];
		pos_n[ZZ] = link_pos[n][ZZ];
	}
	const dReal *getLinkZeroMatrix() {
		return rot_0;
	}
	const dReal *getTopWheelPos() {
		return top;
	}
	const dReal *getFrontWheelPos() {
		return front;
	}
	const dReal *getBackWheelPos() {
		return back;
	}
	const dReal *getAngles() {
		return angles;
	}			// [0] = angle between first and second link

	dReal getMinimumZ() {
		return AABB_min[ZZ];
	}

	dReal getLinkWidth() { return link_design->getWidth(); }
	dReal getHeight() { return AABB_max[ZZ] - AABB_min[ZZ]; };
	
	dReal calcSprocketRadius(int teeth_num) {
		return link_design->calcSprocketRadius(teeth_num);
	}

	// Action Methods

	void moveDesign(dReal x, dReal y, dReal z);
};

typedef TrackDesign *TrackDesignID;

#endif				// __TRACKDESIGN_H__
