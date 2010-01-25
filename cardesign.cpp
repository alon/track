#include "cardesign.h"
#include "error.h"
#include "myobj.h"
#include "track.h"

dReal CarDesign::centerButtomZ()
{
	return back_pos[ZZ] + left_track_design->getMinimumZ();
}

CarDesign::CarDesign(dReal pos[3], dMatrix3 Rot,
			TrackDesignID n_track_design,
			WheelDesignID n_wheel_design, dReal n_spacer,
			int n_sprocket_teeth, dReal n_body_mass)
:  body_mass(n_body_mass), sprocket_teeth(n_sprocket_teeth),
spacer(n_spacer), left_track_design(n_track_design), wheel_design(n_wheel_design)
{
	// NOTE: left_track_design is initially not at the correct position. otherwise it can be used
	// (for getWidth for instance)
	PEXP(body_mass);
	center[0] = pos[0];
	center[1] = pos[1];
	center[2] = pos[2];
	for (int i = 0; i < 12; ++i)
		init_R[i] = Rot[i];

	dReal L, R;
	L = n_track_design->getJJLength();
	R = n_track_design->calcSprocketRadius(sprocket_teeth);

	// body size
	body_sides[XX] = R + 4 * L;	//R + 10 * L;
	body_sides[YY] = body_sides[XX] / 2.0;
	body_sides[ZZ] = body_sides[XX] / 3.0;

	// center of back wheels relative to block center (right)
	back_vec[XX] = -2 * L;	//- 5 * L;
	back_vec[YY] = -getTrackToTrack() / 2.0;
	back_vec[ZZ] = -0.25 * R;	//- 5 * L + 0.25 * R;

	back_pos[XX] = center[XX] + back_vec[XX];
	back_pos[YY] = center[YY] + back_vec[YY];
	back_pos[ZZ] = center[ZZ] + back_vec[ZZ];

	PEXP(R);
	PEXP(spacer);

	wheel_design->setRadius(R - spacer);
	//wheel_design->setTeethNum(sprocket_teeth);

	left_track_design->moveDesign(back_pos[XX], back_pos[YY], back_pos[ZZ]);
	
	// finally set the body position higher then the center - so its bottom is a little below axis,
	chassis[XX] = center[XX];
	chassis[YY] = center[YY];
	chassis[ZZ] = center[ZZ];// + left_track_design->getHeight() / 2.0;
	PEXP(left_track_design->getHeight());
}

void CarDesign::moveDesign(dReal x, dReal y, dReal z)
{
	center[XX] += x;
	center[YY] += y;
	center[ZZ] += z;
	back_pos[XX] += x;
	back_pos[YY] += y;
	back_pos[ZZ] += z;
	left_track_design->moveDesign(x, y, z);
}
