#include <ode/ode.h>
#include "cardesign.h"
#include "boxobj.h"
#include "car.h"

#include "error.h"


void Car::getSprocketMass(dMass * mass) 
{
	if (sprocket[0])
		dBodyGetMass(sprocket[0], mass);
}

void Car::breakWheel() 
{
	dJointDestroy(axle[0]);
	axle[0] = 0;
}

void Car::setSpeed(int ind, dReal speed) 
{
	ind += FIRST_SPROCKET;
	if (axle[ind] != 0) {
		dJointSetHingeParam(axle[ind], dParamFMax, max_f);	// what should this be?
		dJointSetHingeParam(axle[ind], dParamVel, speed);
	}
}

void Car::createBody(const dReal * pos) 
{
	
	// create body
	std::cout << "debug: in createBody\n";
	body_obj =
	new BoxObject(world, space, car_design->getBodySides(),
			  body_mass);
	dBodySetPosition(body_obj->body[0], pos[XX], pos[YY], pos[ZZ]);
	dBodySetRotation(body_obj->body[0],
			  car_design->getCenterRotation());
	
	//dBodySetRotation(body_obj->body[0], R);
}

Car::Car(dWorldID world, dSpaceID space, CarDesignID n_car_design, AppDesignID n_app_design) :MyObject(world, space), car_design(n_car_design),
app_design(n_app_design)
{
	int i, j, ind;
	body_obj = 0;
	for (i = 0; i < 2; ++i)
		chain_obj[i] = 0;
	for (i = 0; i < 6; ++i)
		wheel_obj[i] = 0;
	max_f = 0;		// must be set with setMaxF
	TrackDesignID track_design = car_design->left_track_design;
	LinkDesignID link_design = track_design->link_design;
	WheelDesignID wheel_design = car_design->wheel_design;
	body_mass = car_design->getBodyMass();

	// create body
	createBody(car_design->getChassisPosition());
	
	/*
	for ( int i = 0 ; i < 6 ; ++i ) {
	CreateToothedWheel(world, space,
	wheel_obj[i], sprocket_teeth, R, t_sides, teeth_h_fuzz);
	}
	*/ 
	
	// create sprocket wheels in back wheels position
	for (j = 0, ind = FIRST_SPROCKET; j < 2; ++j, ++ind) {
		wheel_obj[ind] = wheel_design->create(world, space);
		sprocket[j] = wheel_obj[ind]->body[0];
		const dReal *sprocket_pos =
		    track_design->getBackWheelPos();
		PVEC(sprocket_pos);
		dReal y = sprocket_pos[YY];
		if (j == 1)
			y += car_design->getTrackToTrack();
		PEXP(y);
		dBodySetPosition(sprocket[j], sprocket_pos[XX], y,
				  sprocket_pos[ZZ]);
	}
	
	// create sprockets and set their position
	
	// create front wheels and set their position
	for (j = 0, ind = FIRST_FRONT; j < 2; ++j, ++ind) {
		wheel_obj[ind] = wheel_design->create(world, space);
		front[j] = wheel_obj[ind]->body[0];
		const dReal *front_pos = track_design->getFrontWheelPos();
		PVEC(front_pos);
		dReal y = front_pos[YY];
		if (j == 1)
			y += car_design->getTrackToTrack();
		PEXP(y);
		dBodySetPosition(front[j], front_pos[XX], y,
				  front_pos[ZZ]);
	}
	
	// create chains
	chain_obj[0] =
	new Chain(world, space, track_design, link_design);
	track_design->moveDesign(0, car_design->getTrackToTrack(), 0);	// instead of recreating design
	chain_obj[1] = new Chain(world, space, track_design, link_design);
	
	// create axle joints for sprockets
	for (j = 0, ind = FIRST_SPROCKET; j < 2; ++j, ++ind) {
		axle[ind] = dJointCreateHinge(world, 0);
		dBodyID second = (body_obj ? body_obj->body[0] : 0);
		dJointAttach(axle[ind], sprocket[j], second);
		const dReal *sp_v = dBodyGetPosition(sprocket[j]);
		const dReal *rot = dBodyGetRotation(sprocket[j]);
		dJointSetHingeAnchor(axle[ind], sp_v[XX], sp_v[YY],
				      sp_v[ZZ]);
		dJointSetHingeAxis(axle[ind], rot[1], rot[5], rot[9]);
	} 
/*
  for ( j = 0, ind = FIRST_BACK ; j < 2 ; ++j, ++ind ) {
  	axle[ind] = dJointCreateHinge(world, 0);			// no joint group (0==NULL)
  	dJointAttach(axle[ind], back[j], 0);			// attach to world
	const dReal* v = dBodyGetPosition(back[j]);
  	dJointSetHingeAnchor(axle[ind], v[XX], v[YY], v[ZZ]);
  	dJointSetHingeAxis(axle[ind], y_axis[XX], y_axis[YY], y_axis[ZZ]);   // axis is up (positive z)
  }
*/ 
	// create front wheels joints
	for (j = 0, ind = FIRST_FRONT; j < 2; ++j, ++ind) {
		axle[ind] = dJointCreateHinge(world, 0);
		dBodyID second = (body_obj ? body_obj->body[0] : 0);
		dJointAttach(axle[ind], front[j], second);
		const dReal *v = dBodyGetPosition(front[j]);
		const dReal *rot = dBodyGetRotation(sprocket[j]);
		dJointSetHingeAnchor(axle[ind], v[XX], v[YY], v[ZZ]);
		dJointSetHingeAxis(axle[ind], rot[1], rot[5], rot[9]);
	} if (body_obj)
		body_obj->show_forces = 1;
	
	// create appendage
	if (body_obj && app_design) {
		
		    //app_obj = new App(world, space, app_design, body_obj->body[0]);
		    app_obj = 0;
	};
	
	// lets see what we have, height wise
	if (chain_obj[0] != 0) {
		const dReal *pos =
		    dBodyGetPosition(chain_obj[0]->body[0]);
		std::cout << "debug: first link position, z wise\n";
		PEXP(pos[ZZ]);
	}

#ifdef ARE_YOU_NUTS_ABOUT_SPRINGS
	// now create the suspension - it is between the wheels and the body.
	Init(0, 2, 0);
	// initially lets make some springs between the wheels and the body.
	for (int i = 0 ; i < 2 ; ++i) {
		dBody a_wheel = wheel_obj[i + FIRST_SPROCKET]->body[0];
		
	};
#endif
}

void Car::dump(MyContainerID objbin) 
{
	int i;
	if (body_obj)
		objbin->Add(body_obj);
	for (i = 0; i < 6; ++i)
		if (wheel_obj[i]) {
			objbin->Add(wheel_obj[i]);
		}
	for (i = 0; i < 2; ++i)
		if (chain_obj[i]) {
			objbin->Add(chain_obj[i]);
		}
	if (app_obj)
		objbin->Add(app_obj);
}

Car::~Car() 
{
	int i;
	for (i = 0; i < 6; ++i)
		if (wheel_obj[i]) {
			
			    //delete wheel_obj[i]; // not needed. why? (no memory remains alloced even if this isn't used)
		}
	for (i = 0; i < 2; ++i)
		if (chain_obj[i]) {
			
			    //chain_obj[i]->DestroyContents();
			    delete chain_obj[i];
		}
	
	//body_obj->DestroyContents();
	if (body_obj)
		delete body_obj;
	if (app_obj)
		delete app_obj;
}


