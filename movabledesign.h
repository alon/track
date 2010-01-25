#ifndef __MOVABLEDESIGN_H__
#define __MOVABLEDESIGN_H__

#include "design.h"

class MovableDesign : public Design {
public:
	virtual void moveDesign(dReal x, dReal y, dReal z) = 0;
};

#endif // __MOVABLEDESIGN_H__
