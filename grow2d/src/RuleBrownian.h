#pragma once
#include <ofMath.h>
#include "Rule.h"
#include "RuleTypes.h"
class CRuleBrownian : public CRule
{
public:
	CRuleBrownian() {}
	CRuleBrownian(float f) : CRule(f) {}

	// Inherited via CRule
	virtual void getspeed(cell_container & cells, float param) override;


	// Inherited via CRule
	virtual int gettype(void) override;

};