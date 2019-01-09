#pragma once
#include <ofVec3f.h>
#include "Cell.h"
#include "CellPopulation.h"
#include "RuleTypes.h"

float dist2(ofVec3f a, ofVec3f b);

class CRule
{
public:
	CRule() {}
	CRule(float coef) { m_coef = coef; }
	virtual void getspeed(cell_container& cells, float param) = 0;
	virtual int gettype(void) = 0;
	float getCoef(void) { return m_coef; }
	void setCoef(float coef) { m_coef = coef; }

protected:
	float m_coef;
};

