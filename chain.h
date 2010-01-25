// track class 1
//

#ifndef __CHAIN_H__
#define __CHAIN_H__

#include "myobj.h"
#include "trackdesign.h"
#include "chain.h"
#include "track.h"

class Chain:public MyObject {
private:

	///////////// Control //////////////

	///////////// Display //////////////

	dReal joint_arrow;	// length of joint arrow drawen at joint locations
	LinkDesignID link_design;
	int buf;

public:
	static int showbit[LINK_PARTS];	// used to selectively display parts of each link
	// NOTE: static

	Chain(dWorldID _world, dSpaceID _space, TrackDesignID design,
		LinkDesignID n_link_design);
	void CalculateForces();
	void Draw();
	dJointID CreateLinkHinge(dWorldID world, dBodyID one, dBodyID two, dReal l_2);
};

typedef Chain *ChainID;

#endif				//  __CHAIN_H__
