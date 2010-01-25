
#include <string>
#include <iostream>
#include <sstream>

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#include "Simulation.h"
#include "automate.h"
#include "scripting.h" // definition of all sorts of Machine's
#include "cconfig.h"
#include "linkboxdesign.h"
#include "mboxlinkdesign.h"
#include "plainwheeldesign.h"
#include "twheeldesign.h"
#include "mydraw.h"
#include "tags.h"
#include "keymap.h"

// singleton pattern
Simulation* Simulation::g_Simulation = NULL;

char textures_path[100] = "textures";

void Simulation::readConfig(cConfig& cfg)
{
	int j;

	if (!cfg.isOk()) return;

	cfg.read("Step");
	*(cfg.iss) >> step;

    cfg.read("UseStepFast");
    *(cfg.iss) >> use_step_fast;

    cfg.read("Iterations");
    *(cfg.iss) >> iterations;

	cfg.read("Gravity");
	for (j = 0; j < 3; ++j)
		*(cfg.iss) >> gravity[j];	// NOTE: no checks for correct input (except in cfg.read)

	cfg.read("GlobalCFM");
	dReal cfm;
	*(cfg.iss) >> cfm;
	dWorldSetCFM(world, cfm);

	cfg.read("GroundSoftERP");
	*(cfg.iss) >> ground_soft_erp;

	cfg.read("GroundSoftCFM");
	*(cfg.iss) >> ground_soft_cfm;

	cfg.read("MuGround");
	mu_ground = cfg.get_dReal();
	ASSERT(mu_ground >= 0);

	cfg.read("MuNonGround");
	mu_non_ground = cfg.get_dReal();
	ASSERT(mu_non_ground >= 0);

	cfg.read("SprocketTeeth");
	*(cfg.iss) >> sprocket_teeth;
	ASSERT(sprocket_teeth > 0);

	cfg.read("BodyMass");
	*(cfg.iss) >> body_mass;
	ASSERT(body_mass > 0);

	cfg.read("InitZ");
	*(cfg.iss) >> initz;
	ASSERT(initz >= 0);

	for (int k = 0; k < LINK_PARTS; ++k) {
        std::string s = "Box";
		s = s + char (k + '0') + "Sides";
		cfg.read(s.c_str());
		for (int j = 0; j < 3; ++j)
			*(cfg.iss) >> link_sides[k][j];
	};

	cfg.read("TeethXYZ");
	for (j = 0; j < 3; ++j)
		*(cfg.iss) >> t_sides[j];

	cfg.read("Spacer");
	*(cfg.iss) >> spacer;

	cfg.read("Radius");
	*(cfg.iss) >> R;

	cfg.read("Height");
	*(cfg.iss) >> height;

	cfg.read("TeethHFuzz");
	*(cfg.iss) >> teeth_h_fuzz;

	cfg.read("MaxF");
	*(cfg.iss) >> max_f;

	cfg.read("MaxSpeed");
	*(cfg.iss) >> max_speed;
	max_speed = fabs(max_speed);
	min_speed = -max_speed;

	// twitch behavior
	cfg.read("TwitchHertz");
	*(cfg.iss) >> twitch_hertz;

	// some testing parameters (explained at declaration point)
	cfg.read("Linear");
	*(cfg.iss) >> linear;
	if (linear != 0)
		linear = 1;

	cfg.read("ChangeLen");
	*(cfg.iss) >> change_len;
	if (change_len != 0)
		change_len = 1;

	cfg.read("Start");
	*(cfg.iss) >> start_tooth;

	cfg.read("Stop");
	*(cfg.iss) >> stop_tooth;

	ASSERT(stop_tooth > start_tooth);

	cfg.read("TexturesPath");
	*(cfg.iss) >> textures_path;

	// Movie parameters
	cfg.read("MovieEventTime");
	*(cfg.iss) >> event_time;

	cfg.read("MovieFinishTime");
	*(cfg.iss) >> finish_time;

	cfg.read("LinkType", 1);
	*(cfg.iss) >> LinkType;

	cfg.read("UsePlainWheel", 1);
	*(cfg.iss) >> UsePlainWheel;

	// init other variables
	for (int i = 0; i < 2; ++i)
		s_speed[i] = 0;
	twitch_flag = 0;
    std::cout << "debug: link_parts = " << link_parts << "\n";

	// link constants calculation
	L = link_sides[1][XX] + link_sides[2][XX] + link_sides[0][XX] / 4;
	link_forward_len =
	    link_sides[1][XX] / 2 + link_sides[2][XX] -
	    link_sides[0][XX] / 4;
	link_backward_len = (link_sides[0][XX] + link_sides[1][XX]) / 2;

////////////////////////////////////////////////////////////////////////////////

	center[XX] = center[YY] = 0;
	center[ZZ] = initz;

////////////////////////////////////////////////////////////////////////////////

//  joint_arrow = link_sides[0][YY] + link_sides[2][YY] + link_sides[4][YY];
}

// simulation parameters
// =====================
// used constantly in the main loop
//

//const int Simulation::link_parts = LINK_PARTS;	// number of parts the link geometry is made of
Simulation::Simulation() : link_parts(LINK_PARTS) {
    t_sides[0] = BOX_X;
    t_sides[1] = BOX_Y;
    t_sides[2] = BOX_Z;	// size of teeth of sprocket

    gravity[0] = 0;
    gravity[1] = -0.5;
    gravity[2] = -0.5;	// gravity vector

    teeth_h_fuzz = 0;	// used to reduce teeth radius compared to their base
    spacer = SPACER;		// added to radius of link chain compared to sprocket
    R = RADIUS;		// radius of sprocket
    height = HEIGHT;		// width of sprocket (should rename)
    mu_ground = MU;		// friction constant for ground(>0)
    mu_non_ground = MU;	// friction constant for track-sprocket etc(>0)
    step = 0.05;		// step size
    use_step_fast = 0;  // use dWorldStep or dWorldStepFast1
    iterations = 10;    // number of iterations for dWorldStepFast1
    mywriteframes = 0;
    lockCamera = 0;		// camera locked to vehicle or not
    linear = 0;			// testing: use a linear or a circular chain
    change_len = 0;		// testing: change L or change radius
    max_speed = 15, min_speed = -15;
    UsePlainWheel = 0;		// determine design for wheel of car
    m_myMachine = new DoNothingMachine(this);
    m_exitMachine = new ExitMachine(this);
    create();
}

void Simulation::create()
{
	// create world

	world = dWorldCreate();
	space = dHashSpaceCreate(NULL);
	//space = dSimpleSpaceCreate();
	contactgroup = dJointGroupCreate(1000000);

	// cfg.read configuration file (param.dat . actually the name of this file should be set in one place)
    cConfig cfg("param.dat");
    this->readConfig(cfg);
    // Check that config file was ok until now
	if (!cfg.isOk()) {
		dWorldDestroy(world);
		dSpaceDestroy(space);
	};

	simTime = 0;		// zero simulation time
	automat = 0;		// zero the automat

	// set gravity

	dWorldSetGravity(world, gravity[XX], gravity[YY], gravity[ZZ]);

	// create ground

	ground = dCreatePlane(space, 0, 0, 1, 0);

////////////////////////////////////////////////////////////////////////////////

	dMatrix3 init_R = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 };
	//dRFromAxisAndAngle(init_R, 0, 1, 0, 0.01);

	LinkDesign *link_design;
	if (LinkType=="Box")
		link_design = new LinkBoxDesign(
				(link_sides[1][XX] + link_sides[2][XX]) / 1.2,
				link_sides[0][YY], link_sides[0][ZZ]);
	else //if (LinkType=="7")
		link_design = new MBoxLinkDesign(link_sides);

	WheelDesignID wheel_design;
	if (UsePlainWheel)
		wheel_design = new PlainWheelDesign(R);
	else {
		wheel_design = new TWheelDesign(teeth_h_fuzz, t_sides, sprocket_teeth);
		wheel_design->setRadius(R);
	}
	
	// wheel to use radius of, number of links in circle, spacer (in addition to link_z)
	// one way of doing this:
	// wheel_design is independent.
	// link_design is adjusted to the wheel design by track_design
	// track_design is done
	// car_design is done using the track_design and the wheel_design.
	// phase 2: do it all with lists of joints to do, and make a general constructor from a design.
	// which uses names to make less general things viables, basically doing a RTTI thing.
	TrackDesignID track_design = new TrackDesign(wheel_design, link_design, sprocket_teeth, 0);
//	    new TrackDesign(R + link_design->getDepth() / 2.0,
//			    sprocket_teeth, back_pos, Rot, link_design);
	CarDesignID car_design =
	    new CarDesign(center, init_R, track_design, wheel_design, spacer, sprocket_teeth, body_mass);

	// make sure the car is above ground
	dReal aboveground = car_design->centerButtomZ();
	PEXP(aboveground);
	if (aboveground < 0.1)
		car_design->moveDesign(0, 0, 0.1 - aboveground);

	//AppDesignID app_design = new AppDesign(car_design);

	car = new Car(world, space, car_design);	//, app_design);
	car->setMaxF(max_f);
	car->dump(&objbin);

	camera_object = car->getMainBody();	//car->getSprocketBody(0);

////////////////////////////////////////////////////////////////////////////////

}

void Simulation::enterMovieMode()
{
    std::cout << "MovieMode: Entered\n";
	mode = MovieMode;	// change mode to MovieMode
	aimCamera(camera_object);	// aim camera at object
	mywriteframes = 1;	// start to capture frames
}

void Simulation::exitMovieMode()
{
    std::cout << "MovieMode: exiting movie mode\n";
	mode = KeyMode;
	mywriteframes = 0;
	if (automat != 0) {
		delete automat;
		automat = 0;
	};
}

void Simulation::mainMovie()
{
	if (automat && mode == MovieMode) {
		automat->Action(simTime);
		if (automat->Finished())
			exitMovieMode();
	}
}

Simulation::~Simulation()
{
	// delete everything

	dGeomDestroy(ground);

	delete car;
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);
	dWorldDestroy(world);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//      Main Functions:  nearCallback, simLoop, command, start          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

static void nearCallback(void *data, dGeomID o1, dGeomID o2);
static void simLoop(int pause);
static void command(int cmd);
static void start();


static void simLoop(int pause)
{
    Simulation* sim = Simulation::instance();
    sim->simLoop(pause);
}

static void command(int cmd)
{
    Simulation::instance()->command(cmd);
}

static void start()
{
//  static float xyz[3] = {-5.0575, 8.4513, 14.300};
//  static float hpr[3] = {-57.0000, -42.5000, 0.000};
	static float xyz[3] = { -4.0068, 8.0185, 2.8900 };
	static float hpr[3] = { -84.000, -20.5000, 0.0000 };
	//xyz[ZZ] = initz*1.75;
	dsSetViewpoint(xyz, hpr);
}

// this is called by dSpaceCollide when two objects in space are
// potentially colliding.

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
    Simulation::instance()->nearCallback(data, o1, o2);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void Simulation::simLoop(int pause)
{
	static dReal last_capture_time = 0.0;
	static dReal capture_dt = 1.0 / 100;	// for 100 fps in simulation time
	// profiling variables
	this->callback_count = this->not_ground_count = this->ground_count = 0;

	// use timer to track time (you need to stop the watch to update the time.
	// apparently the digits change so fast it can't be read :-)
	//dStopwatchStop(&this->timer);
	//dStopwatchStart(&this->timer);

	// if there is an automat running, run it. if it is done, delete and *zero* it
	this->mainMovie();

	// capture a picture every capture_dt second
#ifdef ODE_FRAME_PATCH
	clearWriteFrames();
#endif
	//cout << "debug: " << (dStopwatchTime(&timer)) << "\n";
	if (mywriteframes && (simTime - last_capture_time > capture_dt)) {
#ifdef ODE_FRAME_PATCH
		setWriteFrames();
#endif
		PEXP(this->simTime);
		last_capture_time = this->simTime;
	}
	// apply forces

	for (int i = 0; i < 2; ++i) {
		if (twitch_flag) {
			this->car->setSpeed(i,
				      (int) ((dStopwatchTime(&timer) *
					      twitch_hertz)) %
				      2 ? min_speed : max_speed);
		} else {
			this->car->setSpeed(i, s_speed[i]);
		}
	}

	// Accounting
	//  - zero number of contacts in each chain
	this->car->chain_obj[0]->ZeroUserData();
	this->car->chain_obj[1]->ZeroUserData();
	// oh, if this was ruby!
	// car.chains.each { |chain| chain.ZeroUserData() }
	
	// time step

	if (!pause) {
		dSpaceCollide(space, 0, &::nearCallback);
		//if (contactgroup->num) PEXP(contactgroup->num);
        // Test fast step
        if (use_step_fast)
            //dWorldStepFast1(world, step, iterations);
		    dWorldQuickStep(world, step);
        else
		    dWorldStep(world, step);
		simTime += step;
		// remove all contact joints
		dJointGroupEmpty(contactgroup);
	}
	// profiling
	//printf("debug: callback %4d not_ground %4d ground %4d\n", callback_count,
	//              not_ground_count, ground_count);

	// draw

	//dsSetTexture (DS_WOOD);
	
	// set camera if it is tracking the body
	if (lockCamera) {
		float xyz[3], hpr[3];
		dVector3 newpoint;
		dsGetViewpoint(xyz, hpr);
		dBodyGetRelPointPos(camera_object, cameraRelVec[0],
				cameraRelVec[1], cameraRelVec[2], newpoint);
		xyz[0] = newpoint[0]; xyz[1] = newpoint[1]; xyz[2] = newpoint[2];
		dsSetViewpoint(xyz, hpr);
	}

	if (objbin.first)
		objbin.first->DrawChain();
}

void Simulation::nearCallback(void* data, dGeomID o1, dGeomID o2)
{
	int i;
	// if (o1->body && o2->body) return;

	// exit without doing anything if the two bodies are connected by a joint
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	//PEXP(b1);
	//PEXP(b2);
	//if (dAreConnected (b1, b2)) cout << "debug: CONNECTED CONNECTED OVER\n";
	if (b1 && b2 && dAreConnected(b1, b2))
		return;

	int g1 = (o1 == this->ground);
	int g2 = (o2 == this->ground);
	int not_ground = (!(g1 ^ g2));
	int one_is_link = (o1 != this->ground) && (b1) && ( dBodyGetData(b1) == (void*)tagLink);
	int two_is_link = (o2 != this->ground) && (b2) && ( dBodyGetData(b2) == (void*)tagLink);
	int link_link = one_is_link && two_is_link;
	double mu;

	// determine friction
	if (link_link)
		mu = 0;
	else if (not_ground)
		mu = this->mu_non_ground;
	else mu = this->mu_ground;
		
	// profiling
	this->callback_count++;

	// debug
	//if (b1 == sprocket || b2 == sprocket) { cout << "debug: collision with sprocket\n"; }

	dContact contact[3];	// up to 3 contacts per box
	if (not_ground) { // except for mu, link_link and link_sprocket are bundled together
		for (i = 0; i < 3; i++) {
			//      contact[i].surface.mode = dContactSoftCFM | dContactApprox1;
			contact[i].surface.mode = 0;
			contact[i].surface.mu = mu;
			contact[i].surface.soft_cfm = 0.01;
		}
	} else {
		for (i = 0; i < 3; i++) {
			contact[i].surface.mode =
			    dContactSlip1 | dContactSlip2 | dContactSoftERP
			    | dContactSoftCFM | dContactApprox1;  // Approx1 means friction only for non zero normal
			contact[i].surface.mu = mu;//dInfinity;
			contact[i].surface.slip1 = 0.1;
			contact[i].surface.slip2 = 0.1;
			contact[i].surface.soft_erp = this->ground_soft_erp;	//0.3;
			contact[i].surface.soft_cfm = this->ground_soft_cfm;	//0.5;
		}
	}
	if (int numc =
	    dCollide(o1, o2, 3, &contact[0].geom, sizeof(dContact))) {
		// profiling
		if (not_ground) {
			this->not_ground_count++;
//              std::cout << "debug: not_ground collision\n";
		} else {
			this->ground_count++;
			//cout << "debug:     ground collision\n";
			
			// if a link is involved, increment its count of contact joints
			dBodyID one_link = (o1 == this->ground? b2 : b1);
			dBodySetData(one_link, (void*)((int)dBodyGetData(one_link) + numc));
		};
		//dMatrix3 RI;
		//dRSetIdentity (RI);
		//const dReal ss[3] = {0.02,0.02,0.02};
		//PEXP(numc);
		for (i = 0; i < numc; i++) {
			dJointID c =
			    dJointCreateContact(this->world, this->contactgroup,
						contact + i);
			dJointAttach(c, b1, b2);
			// dsDrawBox (contact[i].geom.pos,RI,ss);
		}
	}
}

void Simulation::command(int cmd)
{
	dMass mass;
	static int view;	// which view was set last
	static std::string input = "";	// input string ("" initially)
	//static dJointID tagging_joint = 0;            // joint that keeps one link static


	if (cmd >= 'A' && cmd <= 'B')
		cmd = cmd + 'a' - 'A';	// lower case
	if (mode == KeyMode) {
		switch (cmd) {
		case keyWriteFrame:
			mywriteframes ^= 1;
			break;
		case keyCreateMainBody:
			if (car->body_obj == 0) {
				car->createBody(center);
				objbin.Add(car->body_obj);
			};
			break;
			// switch mode to input
		case keyInputMode:
			mode = InputMode;
            std::cout << "input: ";
            std::cout.flush();
			break;
			// abandon ship
		case keyBreakWheel:
			car->breakWheel();
			break;
			// change speed
		case keyZeroSpeed:
			s_speed[0] = 0;
			s_speed[1] = 0;
			break;
		case keyRightSpeedIncrease:
			s_speed[1] += 0.1;
			break;
		case keyRightSpeedDecrease:
			s_speed[1] -= 0.1;
			break;
		case keyRightSpeedMax:
			s_speed[1] = max_speed;
			break;
		case keyRightSpeedMin:
			s_speed[1] = min_speed;
			break;
		case keyLeftSpeedIncrease:
			s_speed[0] += 0.1;
			break;
		case keyLeftSpeedDecrease:
			s_speed[0] -= 0.1;
			break;
		case keyLeftSpeedMax:
			s_speed[0] = max_speed;
			break;
		case keyLeftSpeedMin:
			s_speed[0] = min_speed;
			break;
		case keyBothSpeedIncrease:
			s_speed[0] += 0.1;
			s_speed[1] += 0.1;
			break;
		case keyBothSpeedDecrease:
			s_speed[0] -= 0.1;
			s_speed[1] -= 0.1;
			break;
		case keyBothSpeedMax:
			s_speed[0] = max_speed;
			s_speed[1] = max_speed;
			break;
		case keyBothSpeedMin:
			s_speed[0] = min_speed;
			s_speed[1] = min_speed;
			break;
			// do the twitch
		case keyTwitchToggle:
			twitch_flag = 1 - twitch_flag;
			break;
		case keyTwitchHertzHalf:
			twitch_hertz /= 2.0;
			break;
		case keyTwitchHertzDouble:
			twitch_hertz *= 2.0;
			break;
			// camera views change
		case keyAimCamera:
			aimCamera(camera_object);
			break;
		case keyLockCameraToVehicle: // only pos, not orientation so far
			lockCamera = 1 - lockCamera;
			float xyz[3], hpr[3];
			dsGetViewpoint(xyz, hpr);
			dBodyGetPosRelPoint(camera_object, xyz[0], xyz[1], xyz[2], cameraRelVec);	// returns point in body relative coordinates
			break;
		case keyIncreaseViewByTwoMod:
			view = (view + 2) % set_view(-1);
			set_view(view);
			break;
		case keyIncreaseViewByOneMod:
			view = (view + 1) % set_view(-1);
			set_view(view);
			break;
			// show speed and wanted speed (uncorrect when in twitch mode)
		case keyDisplayWheels:
			for (int i = 0 ; i  < 4 ; ++i)
				car->wheel_obj[i]->toggleDrawFlag();
		case keyPrintSprocketMass:
			car->getSprocketMass(&mass);
			printf("mass %3.3f %3.3f %3.3f %3.3f\n",
			       mass.mass, mass.I[0], mass.I[5],
			       mass.I[10]);
			break;
		case keyPrintManyVariables:
			printf
			    ("s_speed %3.3f | actual %3.3f | hertz %3.3f | time %3.3f | lv %d\n",
			     s_speed[0], car->getSprocketSpeed(),
			     twitch_hertz, dStopwatchTime(&timer), view);
			break;
		case keyEnterMovieMode:
			enterMovieMode();
			automat = new AutoEventFinish(simTime, event_time, m_myMachine, finish_time, m_exitMachine, camera_object);	// create automat
			break;
		};
		if (cmd >= '0' && cmd < ('0' + LINK_PARTS))
			Chain::showbit[cmd - '0'] =
			    1 - Chain::showbit[cmd - '0'];
		for (int i = 0; i < 2; ++i)
			s_speed[i] =
			    BRACKET(s_speed[i], min_speed, max_speed);
	} else if (mode == InputMode) {
		// switch mode to single key
		if (cmd == ' ') {
			mode = KeyMode;
			//process (input);
            std::istringstream s(input);
			int tagged_link;
			s >> tagged_link;
			/*
			   tagged_link = BRACKET(tagged_link, 0, chain_obj[0]->b_num - 1);
			   if (tagging_joint != 0) // delete old
			   dJointDestroy(tagging_joint);
			   tagging_joint = dJointCreateFixed(world, 0);
			   dJointAttach(tagging_joint, chain_obj[0]->body[tagged_link], 0);
			   dJointSetFixed(tagging_joint);
			 */
            std::cout << "\n";
			// reset input
			input = "";
		} else {
			input += cmd;
            std::cout << (char) cmd;
            std::cout.flush();
		}
	} else if (mode == MovieMode) {	// capture pictures until key is m or automat says stop
		// any key stops the movie
		exitMovieMode();
	}
};

int Simulation::set_view(int v)
{
	static int max = 4;

	float xyz[3];
	float hpr[3];
	const dReal *dxyz;

	if (camera_object == 0)
		return v;
	dxyz = dBodyGetPosition(camera_object);
	switch (v) {
	case -1:
		return max;
		break;
	case 0:
		start();
		return 0;
	case 1:
		xyz[0] = dxyz[0];
		xyz[1] = dxyz[1] + R * 2;
		xyz[2] = dxyz[2];
		hpr[0] = -90;
		hpr[1] = 0;
		hpr[2] = 0;
		break;
	case 2:
		xyz[0] = dxyz[0];
		xyz[1] = dxyz[1];
		xyz[2] = dxyz[2] + R * 2;
		hpr[0] = -90;
		hpr[1] = -90;
		hpr[2] = 0;
		break;
	case 3:
		xyz[0] = dxyz[0];
		xyz[1] = dxyz[1] + R;
		xyz[2] = dxyz[2] - R;
		hpr[0] = -90;
		hpr[1] = 0;
		hpr[2] = 0;
		break;
	}
	dsSetViewpoint(xyz, hpr);
	return v;
}

void Simulation::run(int argc, char** argv)
{
	// setup pointers to drawstuff callback functions
	dsFunctions fn;
	fn.version = DS_VERSION;
	//fn.start = makeFunctor((CBFunctor0*)0,&start);
	//fn.step = makeFunctor((CBFunctor1<int>*)0,&simLoop);
	//fn.command = makeFunctor((CBFunctor1<int>*)0,&command);
	fn.start = &start;
	fn.step = &::simLoop;
	fn.command = &::command;
	fn.stop = 0;
	fn.path_to_textures = textures_path;

	// reset timer
	dStopwatchReset(&timer);
	//dStopwatchStart(&this->timer);

	// run simulation
	dsSimulationLoop(argc, argv, 352, 288, &fn);
}

