#include "Rule.h"


/*
ofVec3f CRule::getspeed(CCell *, std::vector<CCell>& cells, float param)
{
	return ofVec3f::zero();
}
*/

float dist2(ofVec3f a, ofVec3f b)
{
	ofVec3f dt = b - a;

	return (dt.x * dt.x + dt.y * dt.y + dt.z * dt.z);
}

