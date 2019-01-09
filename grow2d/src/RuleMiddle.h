#pragma once

#include "Rule.h"

class CRuleMiddle : public CRule
{
public:
	CRuleMiddle() {};
	CRuleMiddle(float coef) { m_coef = coef; };
	virtual void getspeed(cell_container & cells, float param) override;
	ofVec3f speed(cell_container::iterator prev, cell_container::iterator cur, cell_container::iterator next)
	{
		ofVec3f v = ((prev->pos() + next->pos()) / 2) - cur->pos();
		
		return v;
	}

	// Inherited via CRule
	virtual int gettype(void) override;
};