#pragma once
#include <vector>
#include <ofVec3f.h>



#define DEBUG_RULES 0

class CCell
{
public:
	CCell();
	CCell(float x, float y); 

	ofVec3f&	pos()		{ return m_pos; }
	ofVec3f		getPos()	{ return (m_pos); }
	void		setPos(ofVec3f v)	{ m_pos = v; }
	float		getRadius() { return m_radius; }
	void		setRadius(float r)	{ m_radius = r; }
	ofVec3f		getSpeed()
	{
		#if DEBUG_RULES
		ofVec3f v = ofVec3f::zero();
		auto vit = m_speed.begin();
		auto cit = m_coef.begin();
		for (; vit != m_speed.end() && cit != m_coef.end(); ++vit, cit++)
		{
			v += (*vit) * (*cit);
		}

		return v;
		#else
		return (m_speed);
		#endif
		
	}
	void addSpeedV(ofVec3f v, float coef) { 
#if DEBUG_RULES
		m_speed.push_back(v); 
		m_coef.push_back(coef);
#else
		m_speed += (v * coef);
#endif
	}

	void updatePosition(float speed = 1)
	{
		#if DEBUG_RULES
		auto vit = m_speed.begin();
		auto cit = m_coef.begin();
			for (; vit != m_speed.end() && cit != m_coef.end(); ++vit, cit++)
			{
				m_pos += (*vit) * (*cit) * speed;
			}

			
		#else
			m_pos += m_speed * speed;
		#endif
	}

	void limitposition(float minx, float maxx, float miny, float maxy)
	{
		if (m_pos.x < minx)
			m_pos.x = minx;
		if (m_pos.x > maxx)
			m_pos.x = maxx;
		if (m_pos.y < miny)
			m_pos.y = miny;
		if (m_pos.y > maxy)
			m_pos.y = maxy;

	}

	void clearSpeed()
	{
		#if DEBUG_RULES
		m_speed.clear();
		m_coef.clear();
		#else
		m_speed = ofVec3f::zero();
		#endif
	}

	CCell * getprev() { return m_pprev; }
	CCell * getnext() { return m_pnext; }
	void setnext(CCell* ptr) { m_pnext = ptr; }
	void setprev(CCell* ptr) { m_pprev = ptr; }

private:
	ofVec3f m_pos;
	float	m_radius;
	int		m_count;

	CCell* m_pnext;
	CCell* m_pprev;

#if DEBUG_RULES
	std::vector<ofVec3f> m_speed;
	std::vector<float> m_coef;
#else
	ofVec3f m_speed;
#endif
};