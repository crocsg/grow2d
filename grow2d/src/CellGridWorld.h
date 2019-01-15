#pragma once


#include "CellGrid.h"

class CCellGridWorld
{
public:
	CCellGridWorld() {
		m_pgrid = nullptr;
	}

	void begin();
	void end();

	void setGridSize(size_t xsize, size_t ysize)
	{
		m_grid_sizex = xsize;
		m_grid_sizey = ysize;
	}

	void setWorldSize(size_t xsize, size_t ysize)
	{
		m_world_xsize = xsize;
		m_world_ysize = ysize;
	}

	void setWorldOffset(size_t xoffset, size_t yoffset)
	{
		m_world_offset_x = xoffset;
		m_world_offset_y = yoffset;
	}
	
	size_t getGridSizeX() { return m_grid_sizex; }
	size_t getGridSizeY() { return m_grid_sizex; }

	CCellGrid* getGrid() { return m_pgrid; }

	CCellContainer* getCellGrid(float worldx, float worldy);

	void			GetGridCellPosFromWorldPos(float x, float y, size_t& gx, size_t& gy)
	{
		float ox = x + m_world_offset_x;
		float oy = y + m_world_offset_y;
		ox /= m_cell_sizex;
		oy /= m_cell_sizey;
		gx = floor(ox);
		gy = floor(oy);

	}

	void			updateCellInfo(CCell* ptr, float oldx, float oldy, float x, float y);

private:
	CCellGrid *m_pgrid;

	size_t m_grid_sizex;
	size_t m_grid_sizey;
	size_t m_world_xsize;
	size_t m_world_ysize;
	size_t m_world_offset_x;
	size_t m_world_offset_y;
	float  m_cell_sizex;
	float  m_cell_sizey;
};