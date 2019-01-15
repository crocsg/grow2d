
#pragma once

#include "Rule.h"


class CRuleCenter : public CRule
{
public:
	CRuleCenter() {};
	CRuleCenter(float coef) { m_coef = coef; };
	virtual void getspeed(CPopulation& pop, float param) override;

	

	// Inherited via CRule
	virtual int gettype(void) override;
};