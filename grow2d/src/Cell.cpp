#include "Cell.h"

CCell::CCell()
{
	m_pnext = m_pprev = nullptr;
}

CCell::CCell(float x, float y)
{	m_pos.x = x; 
	m_pos.y = y; 
	m_pos.z = 0; 
	m_pnext = m_pprev = nullptr;
}
