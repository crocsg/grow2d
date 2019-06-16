#pragma once

#include <ofGraphics.h>
#include <deque>
#include <vector>
#include <list>
#include <array>
#include "Cell.h"
#include "CellMemContainer.h"
#include "CellGridWorld.h"


typedef  std::deque< CCell* > cell_container;

class CPopulation
{
public:
	CPopulation() 
	{
		m_pop.clear();
		m_iteration = 0;
	}

	~CPopulation()
	{

	}

	void displayGrid()
	{
		ofPushStyle();
		CCellGrid* pgrid = m_posgrid.getGrid();

		for (size_t y = 0; y < pgrid->getysize(); y++)
		{
			for (size_t x = 0; x < pgrid->getxsize(); x++)
			{
				CCellContainer* ptr = pgrid->getCellGrid(x, y);
				size_t d = ptr->cells().size();
				if (d)
					ofSetColor(0, ofMap(d, 0, 16, 128, 255, true), 0);
				else
					ofSetColor(128, 0, 0);
				ofDrawRectangle(x * 2, y * 2, 2, 2);
			}
		}
		ofPopStyle();
	}

	void setPlayField(float min_x, float max_x, float min_y, float max_y)
	{
		m_min_x = min_x;
		m_max_x = max_x;
		m_min_y = min_y;
		m_max_y = max_y;
	}

	void updateposition(bool force=false)
	{
		for (auto cit = m_pop.begin(); cit != m_pop.end(); ++cit)
		{
			ofVec3f oldpos = (*cit)->pos();
			(*cit)->updatePosition();
			(*cit)->limitposition(m_min_x, m_max_x, m_min_y, m_max_y);
			ofVec3f newpos = (*cit)->pos();
			if (force || oldpos.x != newpos.x || oldpos.y != newpos.y)
				m_posgrid.updateCellInfo(*cit, oldpos.x, oldpos.y, newpos.x, newpos.y);
		}

	}

	
	
	void updateptr();

	void addCell(CCell& cell);
	void insertCell(cell_container::iterator& pos, CCell& cell);
	void removecell(CCell * ptr);
	

	cell_container& getContainer() { return m_pop; }
	CCellGridWorld& getGrid() { return m_posgrid; }
	CCell*	alloc_cell();
	void	delete_cell(CCell* ptr);
	void	delete_all_cell();

	void	add_iteration() { m_iteration++; }
	void	setiteration(size_t n) { m_iteration = n; }
	size_t	getiteration() { return m_iteration; }

private:
	float m_min_x, m_max_x, m_min_y, m_max_y;
	size_t m_iteration;

	std::array< CCellMemContainer, 128000 >	m_arr_cells;
	cell_container	m_pop;		// cell population
	CCellGridWorld	m_posgrid;	// population position grid structure
};