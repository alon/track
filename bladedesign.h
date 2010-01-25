#ifndef __BLADEDESIGN_H__
#define __BLADEDESIGN_H__

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

class BladeDesign : public Design {
      private:
	dReal height, depth, width;
      public:
	dReal getHeight() {
		return height;
	};
	dReal getDepth() {
		return depth;
	};
	dReal getWidth() {
		return width;
	};
	dGeomID createGeom(dSpaceID space);	// actual work to implement required
      BladeDesign(dReal _h, dReal _d, dReal _w):height(_h), depth(_d),
	    width(_w) {
	};
};

typedef BladeDesign *BladeDesignID;

#endif				// __BLADEDESIGN_H__
