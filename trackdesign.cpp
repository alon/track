#include "trackdesign.h"
#include "common.h"		// for MIN / MAX
#include "error.h"
#include "myobj.h"
#include "track.h"
#include "wheeldesign.h"

TrackDesign::TrackDesign(WheelDesignID wheel_design,
			LinkDesignID n_link_design,
			int links_in_circle,
			dReal spacer,
			const dReal * n_back,	// position of back wheel
			const dReal * n_rot_0)// orientation of link 0
:  link_design(n_link_design)
{
	int i;
	static dReal ident_rot[12] = {1,0,0,0,0,1,0,0,0,0,1,0};
	static dReal zero_vec[3] = {0,0,0};

	// move initial position to given, and initial rotation to given
	if (!n_back) n_back = zero_vec;
	if (!n_rot_0) n_rot_0 = ident_rot;

	for (i = 0; i < 3; ++i)
		back[i] = n_back[i];
	for (i = 0; i < 12; ++i)
		rot_0[i] = n_rot_0[i];
	dReal radius = wheel_design->getRadius() + spacer + link_design->getDepth() / 2.0;// + 0.2;

	dReal alpha = M_PI * 2 / links_in_circle;	// angle between two adjacent teeth / links

	jj_len = 2 * radius * sin(alpha / 2.0);		// calculate link length from wheel radius
							// and number of links in circle
	link_design->setJJLength(jj_len);		// adjust link design accordingly

	//dReal width_2 = link_design->getWidth() / 2.0;

	// calculate angle between adjacent links in clock wise direction is stored in angles.

	// num of links in each part (*units, -1 for the last one)
/*#define NUM_OF_PARTS 6
  // -180..45 degree
  // 67.5 constant for 6 units
  // 45..-22.5
  // -22.5 constant for 8 units
  // -22.5..-180
  dReal app[NUM_OF_PARTS] = {alpha, 0, alpha, 0, alpha, 0};	// angle per part
  int lpp[NUM_OF_PARTS] = {6, 6, 3, 11, 7, 15};			// links per part
  */
#define NUM_OF_PARTS 4
	dReal app[NUM_OF_PARTS] = { 0, alpha, 0, alpha };
	int lpp[NUM_OF_PARTS] = { 1, 1, 1, 1 };
	link_num = 0;
	for (i = 0; i < NUM_OF_PARTS; ++i) {
		lpp[i] *= links_in_circle / 2;
		link_num += lpp[i];
	};
	lpp[NUM_OF_PARTS - 1]--;
	int angle_num = link_num - 1;
	angles = new dReal[angle_num];

	int pn = 0;		// index into lpp (stands for part number)
	int ind = 0;
	/*
	   int sprocket_link_ind = lpp[0] + lpp[1] + 2;
	   int front_link_ind = lpp[0] + lpp[1] + lpp[2] +lpp[3] + 3;
	 */
	//int sprocket_link_ind = 8;
	//int front_link_ind = 8;

	for (pn = 0; pn < NUM_OF_PARTS; ++pn)
		for (int i = 0; i < lpp[pn]; ++i, ++ind)
			angles[ind] = app[pn];

	// an integrity check - the sum should be 360 or 2 * M_PI
	dReal sum = 0;
	for (i = 0; i < angle_num; ++i)
		sum += angles[i];
	PEXP(sum);
	// end -- integrity check

	// calculate link positions

	link_pos = new dReal[link_num][3];
	dMatrix3 rot;
	for (i = 0; i < 12; ++i)
		rot[i] = rot_0[i];
	dMatrix3 rot2;
	dReal *p = rot;
	dReal *p2 = rot2;

	dReal l_2 = jj_len / 2.0;
	// link 0 position
	for (i = 0; i < 3; ++i)
		link_pos[0][i] = back[i];
	link_pos[0][ZZ] = back[ZZ] - radius;

	for (i = 1; i < link_num; ++i) {
		dReal alpha = angles[i - 1];
		dReal ca = cos(alpha);
		dReal sa = sin(alpha);
		// pos[i] = pos[i-1] + jj_len/2 * (xhat*(1+ca) + zhat*sa)
			//	 pos[XX] + l_2 * (x_axis[XX] * (1 + ca) +
			//			  z_axis[XX] * sa),
			//	 pos[YY] + l_2 * (x_axis[YY] * (1 + ca) +
		link_pos[i][XX] = link_pos[i - 1][XX] +
		    l_2 * (p[RXX] * (1 + ca) + p[RZX] * sa);
		link_pos[i][YY] = link_pos[i - 1][YY] +
		    l_2 * (p[RXY] * (1 + ca) + p[RZY] * sa);
		link_pos[i][ZZ] = link_pos[i - 1][ZZ] +
		    l_2 * (p[RXZ] * (1 + ca) + p[RZZ] * sa);
		p2[RXX] = ca * p[RXX] + sa * p[RYX];	// xhat_new = ca*xhat + sa*yhat
		p2[RXY] = ca * p[RXY] + sa * p[RYY];
		p2[RXZ] = ca * p[RXZ] + sa * p[RYZ];
		p2[RYX] = ca * p[RYX] - sa * p[RXX];	// yhat_new = ca*yhat - sa*xhat
		p2[RYY] = ca * p[RYY] - sa * p[RXY];
		p2[RYZ] = ca * p[RYZ] - sa * p[RXZ];
		dReal *t = p;
		p = p2;
		p2 = t;

		// compute heighest and lowest links X, Y, Z - i want the AABB
		for (int k = 0; k < 3; ++k) {
			if (i == 1) {
				AABB_max[k] = link_pos[i][k];
				AABB_min[k] = link_pos[i][k];
			} else {
				AABB_max[k] =
				    MAX(AABB_max[k], link_pos[i][k]);
				AABB_min[k] =
				    MIN(AABB_min[k], link_pos[i][k]);
				//PEXP(AABB_max[k]);
				//PEXP(AABB_min[k]);
			};
		}
	}

	// compute wheel positions
	//getLinkPosition(top, sprocket_link_ind);
	//top[ZZ] -= radius;
	//getLinkPosition(front, front_link_ind);
	//front[XX] -= radius;
	top[0] = back[0];
	top[1] = back[1];
	top[2] = back[2];
	front[0] = back[0];
	front[1] = back[1];
	front[2] = back[2];
	front[XX] += jj_len * links_in_circle / 2;
}

TrackDesign::~TrackDesign()
{
	delete angles;
	delete[]link_pos;
}

void TrackDesign::moveDesign(dReal x, dReal y, dReal z)
{
	/*
	dReal top[3], back[3], front[3];	// position of wheels (top = sprocket)
	 dReal(*link_pos)[3];	// link position vectors
	dReal AABB_max[3], AABB_min[3];	// Axis Aligned Bounding Box for whole track - two corners
*/

	dReal(*pos)[3] = link_pos;
	for (int i = 0; i < link_num; ++i, ++pos) {
		(*pos)[XX] += x;
		(*pos)[YY] += y;
		(*pos)[ZZ] += z;
	}
	top[XX] += x;
	top[YY] += y;
	top[ZZ] += z;
	back[XX] += x;
	back[YY] += y;
	back[ZZ] += z;
	front[XX] += x;
	front[YY] += y;
	front[ZZ] += z;
	AABB_min[XX] += x;
	AABB_min[YY] += y;
	AABB_min[ZZ] += z;
	AABB_max[XX] += x;
	AABB_max[YY] += y;
	AABB_max[ZZ] += z;
}
