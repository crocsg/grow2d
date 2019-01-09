
#include <ofVec3f.h>
#include <ofMath.h>

#include "RuleMiddle.h"
#include "CellPopulation.h"

void CRuleMiddle::getspeed(cell_container & cells, float param)
{
	auto pit = cells.begin();
	auto it = pit + 1;
	auto nit = pit + 2;

	for (; it != cells.end() && pit != cells.end() && nit != cells.end(); ++it, ++pit, ++nit)
	{
		it->addSpeedV(speed(pit, it, nit), this->m_coef);
	}

	pit = cells.end() -1;
	it = cells.begin();
	nit = it++;
	
	it->addSpeedV(speed(pit, it, nit), this->m_coef);

	pit--;
	it = cells.end() -1;
	nit = cells.begin();

	it->addSpeedV(speed(pit, it, nit), this->m_coef);
}

int CRuleMiddle::gettype(void)
{
	return MIDDLE;
}
