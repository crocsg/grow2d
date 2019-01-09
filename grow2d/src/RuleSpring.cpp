#include "RuleSpring.h"



void CRuleSpring::getspeed(cell_container & cells, float param)
{
	for (auto it = cells.begin(), nit = cells.begin () + 1; nit != cells.end(); ++it, ++nit)
	{
		it->addSpeedV(spring(it.operator->(), nit.operator->()), this->m_coef);
	}

	for (auto it = cells.rbegin(), nit = cells.rbegin() + 1; nit != cells.rend(); ++it, ++nit)
	{
		it->addSpeedV(spring(it.operator->(), nit.operator->()), this->m_coef);
	}
		
	cells.back().addSpeedV(spring(&cells.back(), &cells.front()), this->m_coef);
	cells.front().addSpeedV(spring(&cells.front(), &cells.back()), this->m_coef);
}

int CRuleSpring::gettype(void)
{
	return SPRING;
}
