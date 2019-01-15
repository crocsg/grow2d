#pragma once
#include "Cell.h"

class CCellMemContainer
{
public:
	CCellMemContainer(CCell& cell) : m_cell(cell) { m_valid = true; }
	CCellMemContainer()  { m_valid = false; }

	bool valid() { return m_valid; }
	CCell& cell() { return m_cell; }

	void setvalid(bool v) { m_valid = v; }

private:
	bool m_valid;
	CCell m_cell;
};