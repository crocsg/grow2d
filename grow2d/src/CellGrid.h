#pragma once

#include <vector>
#include <set>

#include "Cell.h"

class CCellContainer;

typedef std::set<CCell*> cell_ptr_container;
typedef std::vector<CCellContainer*> adjacent_container;

class CCellContainer
{
public:
	CCellContainer() {};

	void addAdjacent(CCellContainer* ptr) { m_adjacent_gridcell.push_back(ptr); }
	cell_ptr_container& cells() { return m_cells; }
	adjacent_container& adajcent() { return m_adjacent_gridcell; }
	void addCell(CCell* ptr);
	void removeCell(CCell* ptr);

private:
	cell_ptr_container	m_cells;
	adjacent_container	m_adjacent_gridcell;
};

class CCellGrid
{
public:
	CCellGrid(size_t xsize, size_t ysize);

	size_t			getCellIndice(size_t x, size_t y);
	CCellContainer* getCellGrid(size_t x, size_t y);
	void			updateCellInfo(CCell* ptr, size_t oldx, size_t oldy, size_t x, size_t y);
	
	size_t			getxsize() { return m_xsize; }
	size_t			getysize() { return m_ysize; }
	
protected:
	void updateAdjacentsCells(size_t x, size_t y);
private:
	CCellGrid() {};
	
	vector<CCellContainer> m_grid;
	size_t m_xsize;
	size_t m_ysize;
	
};

