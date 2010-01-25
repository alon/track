#include "plainwheeldesign.h"
#include "error.h"
#include "plainwheel.h"

PlainWheelID PlainWheelDesign::create(dWorldID world, dSpaceID space)
{
	return new PlainWheel(world, space, this);
}
