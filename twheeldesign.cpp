#include "twheeldesign.h"
#include "error.h"
#include "myobj.h"
#include "track.h"

TWheelDesign::TWheelDesign(dReal _teeth_h_fuzz, dReal _t_sides[3], int _sprocket_num)
:  sprocket_num(_sprocket_num), teeth_h_fuzz(_teeth_h_fuzz)
{
	setRadius(1.0);
	t_sides[0] = _t_sides[0];
	t_sides[1] = _t_sides[1];
	t_sides[2] = _t_sides[2];
}
