#ifndef __COMMON_H__
#define __COMMON_H__

// coordinate system standard (for vectors, e.g. speed[XX])
enum VEC_INDEX { XX = 0, YY, ZZ };
enum DIR_INDEX { DIR_X = 1, DIR_Y, DIR_Z };
enum ROT_INDEX {
	RXX = 0, RYX, RZX,
	RXY = 4, RYY, RZY,
	RXZ = 8, RYZ, RZZ
};

#define EVEN(j) (1 - 2 * ((j)%2))
#define MAX(a,b) ((a)>(b)? (a) : (b))
#define MIN(a,b) ((a)>(b)? (b) : (a))

//#include <ode/config.h>

#endif	// __COMMON_H__
