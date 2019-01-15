#include "RuleCenter.h"

void CRuleCenter::getspeed(CPopulation& pop, float param)
{
	cell_container & cells = pop.getContainer();

	const float dmin = 100 * 100;
	ofVec3f c = ofVec3f::zero();

	for (auto it = cells.begin(); it != cells.end(); it++)
	{
		float d = (*it)->pos().squareDistance(c);
		if (d < dmin && d > 0.1)
		{
			(*it)->addSpeedV((*it)->pos()/d, this->m_coef);
		}
	}
}

int CRuleCenter::gettype(void)
{
	return CENTER;
}
