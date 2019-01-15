#pragma once

#include "Rule.h"

class CRuleMiddle : public CRule
{
public:
	CRuleMiddle() {};
	CRuleMiddle(float coef) { m_coef = coef; };
	virtual void getspeed(CPopulation& pop, float param) override;
	
	ofVec3f speed(CCell* prev, CCell *cur, CCell *next)
	{
		ofVec3f v = ((prev->pos() + next->pos()) / 2) - cur->pos();
		v.limit(2);

		return v;
	}

	// Inherited via CRule
	virtual int gettype(void) override;
};