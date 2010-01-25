#ifndef __CARDESIGN_H__
#define __CARDESIGN_H__

#include <ode/ode.h>
#include "common.h"
#include "myobj.h"

#include "movabledesign.h"
#include "trackdesign.h"
#include "wheeldesign.h"

class CarDesign:public MovableDesign {
private:
	dReal body_mass;	// mass of main body
	dReal center[3];
	dReal chassis[3];	// center of chassis
	dReal body_sides[3];	// size of main body
	dReal back_vec[3], back_pos[3];	// place of right back wheel relative to center of body
	dReal length;
	int sprocket_teeth;
	dReal spacer;
	dMatrix3 init_R;
public:
	TrackDesignID left_track_design;	// design of left track (position of links)
	WheelDesignID wheel_design;	// design of wheel (teeth size)

	dReal getBodyMass() {
		return body_mass;
	}
	const dReal *getBodySides() {
		return body_sides;
	}
	const dReal *getCenterPosition() {
		return center;
	}
	const dReal *getCenterRotation() {
		return init_R;
	}
	const dReal *getChassisPosition() {
		return chassis;
	}
	dReal getTrackToTrack() {
		return body_sides[YY] + left_track_design->getLinkWidth();
	}

	dReal getLength() {
		return length;
	}
	int getTeethNum() {
		return sprocket_teeth;
	};
	dReal getSpacer() {
		return spacer;
	};
	dReal centerButtomZ();	// return distance from buttom most point to center in zero condition

	// Constructors
	
	CarDesign(dReal pos[3], dMatrix3 R, TrackDesignID n_track_design,
		  WheelDesignID n_wheel_design, dReal spacer,
		  int n_sprocket_teeth, dReal n_body_mass);

	// Modifiers

	void moveDesign(dReal x, dReal y, dReal z);
};

typedef CarDesign *CarDesignID;

#endif				// __DESIGN_H__
