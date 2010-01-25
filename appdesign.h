#ifndef __APPDESIGN_H__
#define __APPDESIGN_H__

/*  ^
 * /|\
 *  |          C
 *  | Z
 *  |
 *               Ag
 *
 *  /---\    /-------------------------------\
 *  |   |    |                               |
 *  | E |    |                               |
 *  |   |    |                               |
 *  |   |    |                               |
 *  |   |    |                               |
 *  |   |    |                               |
 *  |   |    |                               |
 *  |   |    |                               |
 *  |   |    |                               |
 *  | B*|    |                               |
 *  | B |    | D                  A          |
 *  |   |    |                               |
 *  \---/    \-------------------------------/
 *
 *     X
 *   <------
 *
 *  body 0: beam from A to B
 *  body 1: blade with E and B
 *  bodies 2,3: piston from C to B_star (large part first)
 *  bodies 4,5: piston from D to E (" " ")
 *
 *  geom without body 0: centered on Ag, from Ag to C
 *
 *  assuming length of cube is 1, and so is its side (normalize later) and width
 *  and assuming that track distance is one_track_distance + track_width.
 *
 *  assuming that shovel height is 1 (normalize E[z] accordingly)
 *  
 *  one_track_distance: distance from center of cube to center of track, y axis wise.
 *  one_track_width: show width, y axis wise (normally. Always local central body y axis wise).
 *  beam_width: width of beam from A to B.
 *  
 *  and relative to center of cube (meaning that the back end middle is [ -0.5, 0, 0 ]
 *  
 *      [ x , y , z ]
 *  A = [ -0.2, one_track_distance + track_width + beam_width, -0.2 ]
 *  		notice that i added an extra track_width/2 to the y, so it will be farther.
 *  B = [ 0.5 + x_end_to_blade + blade_depth / 2, A[y], A[z] ]
 *  B_star = [ B[x], 0.3, A[z] ]
 *  C = [ 0.4, B_star[y], 0.7 ]
 *  D = [ C[x], A[y], A[z] ]
 *  E = [ B[x], A[y], 0.4 ]
 *  
 */

#include "design.h"
#include "cardesign.h"
#include "bladedesign.h"

class AppDesign : public Design {
private:
	// all values are stored relative to center. that way i can easily change left to right

	dReal A[3], B[3], B_star[3], C[3], D[3], E[3];	// anchor positions - hinge joints all

	dReal K[3], L[3], M[3], N[3], O[3], P[3], Q[3];

	int side;		// left = 0, right = 1; should this be -1, 1?
public:
	CarDesignID car_design;
	BladeDesignID blade_design;
	AppDesign(CarDesignID n_car_design, BladeDesignID n_blade_design);

	void setSide(int _side) {
		ASSERT(_side == 0 || _side == 1);
		side = _side;
	};			// determine left or right side
};

typedef AppDesign *AppDesignID;

#endif				// __APPDESIGN_H__
