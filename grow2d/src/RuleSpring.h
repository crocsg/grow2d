#pragma once

#include "Rule.h"

#define DIST_R1 3
#define DIST_R2 5

class CRuleSpring : public CRule
{
public:
	CRuleSpring() {};
	CRuleSpring(float coef) { m_coef = coef; };
	virtual void getspeed(CPopulation& pop, float param) override;

	ofVec3f spring(CCell* pcur, CCell *pnext)
	{
		float d = pcur->pos().squareDistance(pnext->pos());
		ofVec3f v = ofVec3f::zero();

		if (d < DIST_R1 * DIST_R1)
		{
			v = pcur->pos() - pnext->pos();
			if (d < 1) d = 1;
			v /= d;
			v.limit(2);
		}
		else if (d > DIST_R2 * DIST_R2)
		{
			v = pnext->pos() - pcur->pos();
			v *= (d - DIST_R2 * DIST_R2) *0.1;
			v.limit(2);
		}

		return (v);
	}

	// Inherited via CRule
	virtual int gettype(void) override;
};