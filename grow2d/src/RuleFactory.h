#pragma once

#include <vector>
#include "Rule.h"
#include "RuleTypes.h"

typedef std::vector<CRule*> VRule;

class CRuleFactory
{
public:
	

	CRule* BuildRule(RULE_TYPE type, float coef);
	CRule* BuildRule(RULE_TYPE type) ;

	void DeleteRule(CRule *prule);
};