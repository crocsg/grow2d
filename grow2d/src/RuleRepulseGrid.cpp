#include "RuleRepulseGrid.h"

void CRuleRepulseGrid::getspeed(CPopulation& pop, float param)
{
	cell_container & cells = pop.getContainer();

	for (auto it = cells.begin(); it != cells.end(); it++)
	{
		CCellContainer *ptr = pop.getGrid().getCellGrid((*it)->pos().x, (*it)->pos().y);
		// check in cell
		auto nit = ptr->cells().begin();

		for (; nit != ptr->cells().end(); ++nit)
		{
			if ((*it)->getnext() == *nit || (*it)->getprev() == *nit)
				continue;
			(*it)->addSpeedV(repulse(*it, *nit), this->m_coef);
		}

		// check adjacent
		auto ait = ptr->adajcent().begin();
		for (; ait != ptr->adajcent().end(); ++ait)
		{
			nit = (*ait)->cells().begin ();
			for (; nit != (*ait)->cells().end(); ++nit)
			{
				if ((*it)->getnext() == *nit || (*it)->getprev() == *nit)
					continue;
				(*it)->addSpeedV(repulse(*it, *nit), this->m_coef);
			}
		}
					
	}
}

int CRuleRepulseGrid::gettype(void)
{
	return REPULSE;
}

