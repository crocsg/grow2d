#include "RuleBrownian.h"

void CRuleBrownian::getspeed(cell_container & cells, float param)
{
	for (auto it = cells.begin(); it != cells.end(); ++it)
	{
		ofVec3f v = ofVec3f(ofRandomf(), ofRandomf(), 0);
		it->addSpeedV(v, this->m_coef);
	}
}

int CRuleBrownian::gettype(void)
{
	return BROWNIAN;
}
