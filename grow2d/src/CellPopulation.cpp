#include "CellPopulation.h"


void CPopulation::addCell(CCell & cell)
{

	CCell* prevptr = nullptr;
	
	if (m_pop.size() > 0)
		prevptr = m_pop.back();
	
	CCell *ptr = alloc_cell();
	ptr->setprev(prevptr);
	ptr->setPos(cell.getPos());
	ptr->setRadius(cell.getRadius());

	m_pop.push_back(ptr);
	
	if (prevptr)
		prevptr->setnext(ptr);
}

void CPopulation::updateptr()
{


	for (auto it = m_pop.begin(); it != m_pop.end(); ++it)
	{
		auto nit = it + 1;
		auto pit = it - 1;

		if (nit != m_pop.end())
			(*it)->setnext(*nit);
		else
			(*it)->setnext(nullptr);
		
		if (it != m_pop.begin())
			(*it)->setprev(*pit);
		else
			(*it)->setprev(nullptr);
	}
}
void CPopulation::insertCell(cell_container::iterator & pos, CCell & cell)
{
	size_t indice = pos - m_pop.begin();
	CCell *pprev = nullptr;
	CCell* pnext = *pos;
	
	if (indice > 0)
		pprev = m_pop[indice - 1];

	//cell.setnext(pnext);
	//cell.setprev(pprev);
	cell.setRadius(0.5);

	// dirty hack
	CCell* ptr = alloc_cell();
	ptr->setPos(cell.getPos());
	ptr->setRadius(cell.getRadius());
	ptr->setnext(pnext);
	ptr->setprev(pprev);
	auto it = m_pop.insert(pos, ptr);

	auto pit = it - 1;
	(*pit)->setnext(ptr);

	auto nit = it + 1;
	if (nit != m_pop.end())
		(*nit)->setprev(ptr);
		
		
	//pprev->setnext(it.operator->());
	//pnext->setprev(it.operator->());
}

void CPopulation::removecell(CCell *ptr)
{
	for (auto it = m_pop.begin(); it != m_pop.end(); ++it)
	{
		if (*it == ptr)
		{
			if (ptr->getprev() != nullptr)
				ptr->getprev()->setnext(ptr->getnext());
			if (ptr->getnext() != nullptr)
				ptr->getnext()->setprev(ptr->getprev());
			m_pop.erase(it);

			size_t gx, gy;
			CCellContainer *pcont = m_posgrid.getCellGrid(ptr->pos().x, ptr->pos().y);
			if (pcont != nullptr)
			{
				pcont->removeCell(ptr);
			}
			delete_cell(ptr);

			break;
		}
	}
}

CCell * CPopulation::alloc_cell()
{
	for (auto it = m_arr_cells.begin(); it != m_arr_cells.end(); it++)
	{
		if (!it->valid())
		{
			it->setvalid(true);
			return &it->cell();
		}
	}
	return nullptr;
}

void CPopulation::delete_cell(CCell * ptr)
{
	for (auto it = m_arr_cells.begin(); it != m_arr_cells.end(); ++it)
	{
		if (&it->cell() == ptr)
			it->setvalid(false);
		return;
	}
}

void CPopulation::delete_all_cell()
{
	for (auto it = m_arr_cells.begin(); it != m_arr_cells.end(); ++it)
		it->setvalid(false);
}