#include <ofMath.h>
#include "CellBrownianMotion2d.h"

ofVec3f CCellBrownianMotion2d::getspeed(float param)
{ 
	
	return ofVec3f(ofRandomf() * param, ofRandomf()*param, 0);
}
