#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <string>

#include <ode/ode.h>

#include "track.h"
#include "myobj.h" // MyContainer
#include "car.h"

// friction
#define MU 0.5

#define RADIUS 10.0f
#define HEIGHT 1.0f
#define SPACER 0.1f

#define BOX_X 2.0f
#define BOX_Y 1.0f
#define BOX_Z 1.0f

enum { KeyMode = 0, InputMode, MovieMode };	// modes are used to change the overall state
					// of the program between:
					// KeyMode - interactive input, single key
					// InputMode - interactive input, command input
					// MovieMode - run a subclass of an Automat and capture frames

class Machine; // automation / scripting object
class Automat;
class cConfig;

class Simulation {
public:
    ~Simulation();
private:
    Simulation(); // constructor
    static Simulation* g_Simulation;
    void readConfig(cConfig& cfg);
    void create();  // creates all ODE/Drawstuff objects
private:
    dReal simTime;			// simulation time
    dReal step;		// step size
    int use_step_fast;  // use dWorldStep or dWorldStepFast1
    int iterations;    // number of iterations for dWorldStepFast1
   
    Automat *automat;		// automat that can run the program if in a non interactive mode
    dReal event_time, finish_time;	// movie taking automat parameters. used in ctor
    
    // Control Objects / Variables
    // ===========================
    
    dBodyID camera_object;		// used by set_view
    int mywriteframes;
    int lockCamera;		// camera locked to vehicle or not
    dVector3 cameraRelVec;		// camera vector relative to vehicle
    
    dStopwatch timer;		// timer for twitch behavior
    
    dReal s_speed[2];		// speed of sprocket[0,1]
    dReal max_f;			// read and set into car object
    
    int twitch_flag;		// twitch behavior (to really check the physical
    						// implementation)
    
    dReal twitch_hertz;		// twitch frquency (Hz)
    
    int UsePlainWheel;		// determine design for wheel of car
    std::string LinkType;		// determine design for links of chain
    
    // configuration parameters
    // ========================
    // used once in cConfig or before the main loop
    // NOTE: alot of them can be made part of cConfig, or just part of main, or both, thus making
    // it possible for multiple simulations and code clarity
    
    int sprocket_teeth;	    // number of teeth in sprocket
    dReal spacer;		    // added to radius of link chain compared to sprocket
    dReal R;    		    // radius of sprocket
    dReal height;		    // width of sprocket (should rename)
    dReal teeth_h_fuzz;	    // used to reduce teeth radius compared to their base
    dReal initz;			// initial height of center of sprocket
    dReal link_sides[LINK_PARTS][3];	// size of different parts of link (all boxes)
    dReal t_sides[3];	    // size of teeth of sprocket
    dReal center[3];		// main body center
    
    //dReal joint_arrow;                    // length of joint arrow drawen at joint locations
    dReal L;			// axis to axis
    dReal link_forward_len;		// center to axis
    dReal link_backward_len;	// axis to center (!= link_forward_len necessarily)
    
    dReal gravity[3];	// gravity vector
    
    int start_tooth, stop_tooth;	// testing link creation
    
    dReal body_mass;		// mass of car body
    
    // Constructions Objects: the world, space, objects, geoms, joints
    // ===============================================================
    
    
    MyContainer objbin;		// for batch operations (draw)
    
    CarID car;

    dWorldID world;
    dSpaceID space;
    dJointGroupID contactgroup;

private:
   
    int linear;			// testing: use a linear or a circular chain
    int change_len;		// testing: change L or change radius
    
    int mode;			// program overall mode.
    
    Machine* m_myMachine;
    Machine* m_exitMachine;
    
    const int link_parts;	// number of parts the link geometry is made of

public:
    static Simulation* instance() { if (!g_Simulation) g_Simulation = new Simulation(); return g_Simulation; }
    // definitions
    // ===========
    
    // from joint.h
    struct dxJointGroup {
    	int num;		// number of joints on the stack
    	dxJoint *firstjoint;	// address of first joint on the stack
    };				// objects.
    
    void enterMovieMode();
    void exitMovieMode();

    // Machine - scripting interface
    void setSpeed(int index, dReal speed);
    dReal max_speed, min_speed;
    
private:
    // for collision reaction function currently ground needs to be public
    dGeomID ground;
    dReal ground_soft_erp, ground_soft_cfm;	// ground softcfm and softerp values. set through param.dat (cConfig)
    dReal mu_ground;		// friction constant for ground(>0)
    dReal mu_non_ground;	// friction constant for track-sprocket etc(>0)
    // Profiling
    // =========
    
    int ground_count, not_ground_count;
    int callback_count;
 
public:
    void simLoop(int pause);
    void nearCallback(void* data, dGeomID o1, dGeomID o2);
    void command(int cmd);
    void mainMovie();
    int set_view(int v);
    void run(int argc, char** argv); // start the simulation loop (exits upon loop exit only)
};

inline void Simulation::setSpeed(int index, dReal speed) {
    if (index==0 || index ==1) {
        this->s_speed[index] = speed;
    }
}

#endif // __SIMULATION_H__

