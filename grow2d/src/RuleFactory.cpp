#include "RuleFactory.h"

#include "RuleBrownian.h"
#include "RuleMiddle.h"
#include "RuleSpring.h"
#include "RuleRepulse.h"
#include "RuleCenter.h"

CRule * CRuleFactory::BuildRule(RULE_TYPE type, float coef)
{
	CRule* prule = nullptr;

	prule = BuildRule(type);
	if (prule != NULL)
		prule->setCoef(coef);

	return (prule);
}

CRule * CRuleFactory::BuildRule(RULE_TYPE type)
{
	CRule* prule = nullptr;
	switch (type)
	{
	case BROWNIAN:
		prule = new CRuleBrownian ();
		break;
	case MIDDLE:
		prule = new CRuleMiddle ();
		break;
	case SPRING:
		prule = new CRuleSpring();
		break;
	case REPULSE:
		prule = new CRuleRepulse();
		break;
	case CENTER:
		prule = new CRuleCenter();
		break;
	}

	return prule;
}

void CRuleFactory::DeleteRule(CRule * prule)
{
	delete prule;
}
