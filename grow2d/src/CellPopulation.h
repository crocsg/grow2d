#pragma once

#include <deque>
#include <vector>
#include <list>
#include "Cell.h"

typedef  std::deque< CCell > cell_container;

class CPopulation
{
public:
	CPopulation()
	{
		m_pop.clear();
	}

	~CPopulation()
	{

	}

	cell_container& getContainer() { return m_pop; }

private:
	cell_container m_pop;
	
};