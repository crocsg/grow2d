#pragma once

#include "Rule.h"

#define MAX_DIST 12
class CRuleRepulse : public CRule
{
public:
	CRuleRepulse() {};
	CRuleRepulse(float coef) { m_coef = coef; };
	virtual void getspeed(cell_container & cells, float param) override;

	ofVec3f repulse(CCell* pcur, CCell *pnext)
	{
		float d = pcur->pos().squareDistance(pnext->pos());
		ofVec3f v = ofVec3f::zero();

		if (d < MAX_DIST * MAX_DIST && d >1)
		{
			v = pcur->pos() - pnext->pos();
			v /= d;
		}
		

		return (v);
	}

	// Inherited via CRule
	virtual int gettype(void) override;
};