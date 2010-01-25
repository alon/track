#ifndef __CAR_H__
#define __CAR_H__

#include "cardesign.h"
#include "wheel.h"
#include "chain.h"
#include "myobj.h"
#include "app.h"

class Car:MyObject {
      public:			// !!!! not private
	CarDesignID car_design;
	AppDesignID app_design;

	// body
	MyObjectID body_obj;	// main body

	//AppID app;                            // appendage

	WheelID wheel_obj[6];	// 0,1 back, 2,3 sprocket, 4,5 front
	enum { FIRST_FRONT = 0, FIRST_SPROCKET = 2, FIRST_BACK = 4 };
	dBodyID sprocket[2], front[2], back[2];

	ChainID chain_obj[2];	// chain of links

	AppID app_obj;		// appendage

	dReal body_mass;	// main body mass

	dJointID axle[6];	// wheel shafts

	// control
	dReal max_f;

      public:
	 Car(dWorldID world, dSpaceID space, CarDesignID n_car_design,
	     AppDesignID n_app_design = 0);
	~Car();
	void setMaxF(dReal mf) {
		max_f = mf;
	};
	void setSpeed(int wheel_index, dReal speed);
	void dump(MyContainerID objbin);
	void createBody(const dReal * pos);
	void breakWheel();
	void getSprocketMass(dMass * mass);
	dBodyID getSprocketBody(int i) {
		return sprocket[i];
	};
	dBodyID getMainBody() {
		if (body_obj)
			return body_obj->body[0];
		return 0;
	}
	dReal getSprocketSpeed() {
		return (axle[0] ==
			0 ? 0 : dJointGetHingeAngleRate(axle[0]));
	}
};

typedef Car *CarID;

#endif
