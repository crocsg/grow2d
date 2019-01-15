#include "CellGrid.h"

#define DEBUG_GRID 1

CCellGrid::CCellGrid(size_t xsize, size_t ysize)
{
	m_xsize = xsize;
	m_ysize = ysize;

	size_t n = xsize * ysize;

	for (size_t i = 0; i < n; i++)
		m_grid.push_back(CCellContainer());

	for (size_t y = 0; y < ysize; y++)
	{
		for (size_t x = 0; x < xsize; x++)
		{
			updateAdjacentsCells(x,y);
#if DEBUG_GRID
			CCellContainer *ptr = this->getCellGrid(x, y);
			cout << "cell " << x << "," << y << " " << ptr->adajcent().size() << endl;
			if (ptr->adajcent().size() > 8)
			{
				cout << "we ave a problem " << x << " " << y << endl;
			}
#endif

		}
	}
		
}

size_t CCellGrid::getCellIndice(size_t x, size_t y)
{
	size_t n = (y * m_xsize) + x;
	return n;
}

CCellContainer* CCellGrid::getCellGrid(size_t x, size_t y)
{
	size_t i = getCellIndice(x, y);
	return (&m_grid[i]);
}

void CCellContainer::addCell(CCell * ptr)
{
	m_cells.insert(ptr);
}

void CCellContainer::removeCell(CCell * ptr)
{
	m_cells.erase(ptr);
}




void CCellGrid::updateCellInfo(CCell * ptr, size_t oldx, size_t oldy, size_t x, size_t y)
{
	CCellContainer *pold = getCellGrid(oldx, oldy);
	pold->removeCell(ptr);
	CCellContainer *pnew = getCellGrid(x, y);
	pnew->addCell(ptr);
}

void CCellGrid::updateAdjacentsCells(size_t x, size_t y)
{
	size_t minx, miny, maxx, maxy;
	size_t p = getCellIndice(x, y);
	CCellContainer* pcurcell = &m_grid[p];

	minx = (x > 0) ? x - 1 : 0;
	maxx = (x < m_xsize - 1) ? x + 1 : m_xsize - 1;
	miny = (y > 0) ? y - 1 : 0;
	maxy = (y < m_ysize - 1) ? y + 1 : m_ysize - 1;

	for (size_t ay = miny; ay <= maxy; ay++)
	{
		for (size_t ax = minx; ax <= maxx; ax++)
		{
			size_t i = getCellIndice(ax, ay);
			CCellContainer* ptr = &m_grid[i];
			if (ptr != pcurcell)
			{
				pcurcell->addAdjacent(ptr);
			}
		}
	}
		

	


}

