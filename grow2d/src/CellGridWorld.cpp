#include "CellGridWorld.h"

void CCellGridWorld::begin()
{
	if (m_pgrid != nullptr)
		throw (std::runtime_error("m_pgrid already allocated"));

	m_pgrid = new CCellGrid(m_grid_sizex, m_grid_sizey);
	m_cell_sizex = m_world_xsize / m_grid_sizex;
	m_cell_sizey = m_world_ysize / m_grid_sizey;
	
}

void CCellGridWorld::end()
{
	delete m_pgrid;
	m_pgrid = nullptr;
}

CCellContainer * CCellGridWorld::getCellGrid(float worldx, float worldy)
{
	size_t x, y;
	
	GetGridCellPosFromWorldPos(worldx, worldy, x, y);

	return m_pgrid->getCellGrid(x, y);
}

void CCellGridWorld::updateCellInfo(CCell * ptr, float oldx, float oldy, float x, float y)
{
	size_t goldx, goldy;
	size_t gx, gy;
	GetGridCellPosFromWorldPos(oldx, oldy, goldx, goldy);
	GetGridCellPosFromWorldPos(x, y, gx, gy);

	m_pgrid->updateCellInfo(ptr, goldx, goldy, gx, gy);
}
