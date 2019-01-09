#include "RuleRepulse.h"

void CRuleRepulse::getspeed(cell_container & cells, float param)
{
	for (auto it = cells.begin(); it != cells.end(); it++)
	{
		for (auto nit = cells.begin(); nit != cells.end(); ++nit)
		{
			if (nit == it || nit == it + 1)
				continue;
			if (it == cells.begin() && nit == cells.end() - 1)
				continue;
			if (it == cells.end() - 1 && nit == cells.begin())
				continue;
			it->addSpeedV(repulse(it.operator->(), nit.operator->()), this->m_coef);
		}
	}
}

int CRuleRepulse::gettype(void)
{
	return REPULSE;
}
