#include "appdesign.h"

////////////////////////////////////////////////////////////////////////////////
// Appendage
// Designer

AppDesign::AppDesign(CarDesignID n_car_design,
		     BladeDesignID n_blade_design)
:  car_design(n_car_design), blade_design(n_blade_design)
{
//      dReal beam_pos[2][3], beam_size[3];
//      dReal app_piston_pos[2][3], app_piston_size[3];
//      dReal body_piston_pos[2][3], body_piston_size[3];
//      dReal beam_dy, app_piston_dy, body_piston_dy;
	TrackDesignID track_design = car_design->left_track_design;
	LinkDesignID link_design = track_design->link_design;
	const dReal *car_center = car_design->getCenterPosition();
	const dReal *R = car_design->getCenterRotation();

	dReal b[2][3];		// vector from center to beam_pos

	const dReal *car_size = car_design->getBodySides();

	dReal track2track_2 = car_design->getTrackToTrack() / 2.0;

	// calculate A, B, Bstar, C, D, E
	A[XX] = car_center[XX] - 0.4 * car_size[XX];
	A[YY] = car_center[YY] - 0. * track2track_2;
	A[ZZ] = car_center[ZZ] - 0.25 * car_size[ZZ];
};
