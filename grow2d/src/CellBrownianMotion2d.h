#pragma once
#include "CellMotion.h"

class CCellBrownianMotion2d : CCellMotion
{
public:
	// Inherited via CCellMotion
	virtual ofVec3f getspeed(float param) override;

};