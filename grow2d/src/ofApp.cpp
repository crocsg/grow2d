#include "ofApp.h"

std::string m_last_message;
const size_t WORLDSIZEX = 2048;
const size_t WORLDSIZEY = 1024;

//--------------------------------------------------------------
void ofApp::setup(){
	
	m_cmds = CStringCmdInterpretor ({ 
		{ "test", this, &ofApp::onTest },
		
		{ "brownian",	this, &ofApp::onBrownian },
		{ "center",		this, &ofApp::onCenter },
		{ "curve",		this, &ofApp::onCurve   },
		{ "contour",	this, &ofApp::onContour },
		{ "dieproba",	this, &ofApp::onDie		},
		{ "fill",		this, &ofApp::onFill	},
		{ "help",		this, &ofApp::onHelp },
		{ "max",		this, &ofApp::onMaxPts	},
		{ "middle",		this, &ofApp::onMiddle  },
		{ "repulse",	this, &ofApp::onRepulse },
		{ "reset",		this, &ofApp::onReset },
		{ "scale",		this, &ofApp::onScale   },
		{ "speed",		this, &ofApp::onSpeed },
	});
	
	m_clientmqtt.begin("devmqtt");
	m_clientmqtt.setProtocol(MQTT_PROTOCOL_V31);
	m_clientmqtt.connect("ofx");
	m_clientmqtt.subscribe("/ofx/cell/param/input");

	ofAddListener(m_clientmqtt.onOnline, this, &ofApp::onOnline);
	ofAddListener(m_clientmqtt.onOffline, this, &ofApp::onOffline);
	ofAddListener(m_clientmqtt.onMessage, this, &ofApp::onMessage); 

	CRuleFactory rf;
	m_nb_start_pt = 100;

	
	m_pop.getGrid().setWorldSize(WORLDSIZEX, WORLDSIZEY);
	m_pop.getGrid().setGridSize(WORLDSIZEX / 16, WORLDSIZEY / 16);
	m_pop.getGrid().setWorldOffset(WORLDSIZEX / 2, WORLDSIZEY / 2);
	m_pop.getGrid().begin();
	m_pop.setPlayField(-(WORLDSIZEX / 2.0) + 1., WORLDSIZEX / 2.0 - 1., -(WORLDSIZEY / 2.0) + 1., WORLDSIZEY / 2.0 - 1.);
	

	for (float i = 0; i < 360.0; i += 360./m_nb_start_pt)
	{
		m_pop.addCell(CCell(cos(ofDegToRad(i)) * 32, sin(ofDegToRad(i)) * 32));
	}

	m_pop.updateposition(true);

	m_rules.push_back(rf.BuildRule(BROWNIAN, 0.01));
	m_rules.push_back(rf.BuildRule(MIDDLE, 0.01));
	m_rules.push_back(rf.BuildRule(SPRING, 0.5));
	m_rules.push_back(rf.BuildRule(REPULSE, 0.5));
	m_rules.push_back(rf.BuildRule(CENTER, 1));


	m_scale = 3;
	m_speed = 1;
	m_maxpts = 10; // m_nb_start_pt; // 200;
	m_save = false;
	m_save_svg = false;
	m_contour = true;
	m_draw_fill = false;
	m_curve_svg = false;
	
	m_dieprob = 0;

	m_min = 0;
	m_max = 1;

	ofSetFrameRate(60);
	ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void ofApp::update(){

	try
	{
		
		m_clientmqtt.update();
			

		for (auto it = m_rules.begin(); it != m_rules.end(); ++it)
		{
			(*it)->getspeed(m_pop, 0);
		}

		bool todo = false;
		if (ofGetFrameNum() % 3 == 0)
		{
			int cnt = 0;

			if (m_pop.getContainer().size() < m_maxpts)
			{
				do
				{
					todo = false;
					for (auto nit = m_pop.getContainer().begin(); nit != m_pop.getContainer().end() - 1; ++nit)
					{
						auto nnit = nit + 1;
						float d = (*nit)->pos().squareDistance((*nnit)->pos());
						if (d > 5 * 5)
						{
							ofVec3f p = ((*nit)->pos() + (*nnit)->pos()) / 2;
							//m_pop.getContainer().insert(nnit, CCell(p.x, p.y));
							m_pop.insertCell(nnit, CCell(p.x, p.y));
							cnt++;
							todo = true;
							break;
						}
					}
				} while (todo);

				//m_pop.updateptr();
			}
		}

		if (ofGetFrameNum() % 7 == 1)
		{
			if (m_dieprob > 0.00001)
			{
				std::vector<CCell *> styx;
				for (auto it = m_pop.getContainer().begin(); it != m_pop.getContainer().end(); ++it)
				{
					if (ofRandom(1) < m_dieprob)
					{
						// kill the cell
						styx.push_back(*it);
					}
				}
				for (auto kit = styx.begin(); kit != styx.end(); kit++)
				{
					m_pop.removecell(*kit);
				}
				//m_dieprob = 0;
			}
		}

		m_pop.updateposition();

		//m_clientmqtt.publish("/ofx/cell/output", "update");
	}
	catch (...)
	{
		cout << "Exception " << endl;
		throw;
	}
}


//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(32);
	ofNoFill();
	ofEnableAntiAliasing();
	ofSetColor(255, 255, 0);

	if (m_save_svg)
	{
		ostringstream fname;
		fname << "hyphae_save" << ofGetTimestampString() << ".svg";
		ofBeginSaveScreenAsSVG(fname.str());
		ofBackground(255);
	}

	ofPushMatrix();
	ofTranslate(ofGetViewportWidth() / 2, ofGetViewportHeight() / 2);
	ofScale(m_scale, m_scale, 1);
	

	if (m_pop.getContainer().size() > 2)
	{	
		ofColor color;
		float min = 256;
		float max = 0;
			
		ofPath path;
		path.setFilled(false);
		path.setStrokeWidth(1);
		path.setStrokeColor(ofColor(0,0,0));

		if (m_curve_svg)
			path.curveTo(m_pop.getContainer().front()->pos().x, m_pop.getContainer().front()->pos().y);
		else
			path.moveTo(m_pop.getContainer().front()->pos().x, m_pop.getContainer().front()->pos().y);
		
		for (auto it = m_pop.getContainer().begin(); it != m_pop.getContainer().end() - 1; ++it)
		{
			auto nit = it + 1;
			float d = (*it)->getSpeed().lengthSquared();
			float c = ofMap(d, 0, m_max, 0, 42, true);
			if (m_save_svg)
			{
				ofSetColor(0);
				
			}
			else
			{
				color.setHsb(42-c, 255, 255);
				ofSetColor(color);
				
			}
			if (! m_save_svg && m_contour) 
				ofLine((*it)->getPos(), (*nit)->getPos());
			if (m_save_svg || m_draw_fill)
			{
				if (m_curve_svg)
					path.curveTo((*nit)->pos().x, (*nit)->pos().y);
				else
					path.lineTo((*nit)->pos().x, (*nit)->pos().y);
			}
			
			if (d < min) min = d;
			if (d > max) max = d;
			
		}
		if (!m_save_svg && m_contour)
			ofLine(m_pop.getContainer().back()->pos(), m_pop.getContainer().front()->pos());
		
		if (m_curve_svg)
		{
			path.curveTo(m_pop.getContainer().front()->pos().x, m_pop.getContainer().front()->pos().y);
			path.curveTo(m_pop.getContainer().at(1)->pos().x, m_pop.getContainer().at(1)->pos().y);
		}
		
		path.close();

		if (!m_save_svg)
		{
			path.setFilled(true);
			path.setFillColor(ofColor(0, 255, 0, 32));
			path.setStrokeColor(ofColor(0, 255, 0, 32));
			path.setStrokeWidth(0);

		}
		if (m_save_svg || m_draw_fill)
			path.draw();

		if (m_save_svg)
		{
			ofPushStyle();
			ofSetColor(255, 0, 0);
			ofDrawLine(-5, 0, 5, 0);
			ofDrawLine(0, -5, 0, 5);
			ofPopStyle();
			ofEndSaveScreenAsSVG();
			m_save_svg = false;
		}
		m_min = min;
		m_max = max;
	
	}
	
	
	
	ofPopMatrix();

	for (auto cit = m_pop.getContainer().begin(); cit != m_pop.getContainer().end(); ++cit)
	{
		(*cit)->clearSpeed();
	}
	m_pop.add_iteration();


	// display info
#if 0
	ofPushMatrix();
	ofTranslate(ofGetViewportWidth() - 256, 0, 0);
	m_pop.displayGrid();
	ofPopMatrix();
#endif
	ofSetColor(0, 255, 255);
	
	ostringstream s;
	
	s << "N :" << m_pop.getiteration() << " FPS :" << ofGetFrameRate() << " cell :" << m_pop.getContainer().size() << " ";


	ofDrawBitmapString(s.str(), 10, 10);
	ofDrawBitmapString(m_last_message, 10, 24);
	
	ofSetColor(0, 255, 0);
	for (auto it = m_display.begin(); it != m_display.end(); ++it)
	{
		ofDrawBitmapString(it->c_str(), 10, (it - m_display.begin()) * 16 + 64);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'S' || key == 's')
		m_save_svg = true;
	else if (key == ' ')
		m_clientmqtt.publish("/ofx/cell/output", "space");
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::onOnline()
{
	cout << "mqtt going online" << endl;
	m_clientmqtt.subscribe("/ofx/cell/param/input");
}

void ofApp::onOffline()
{
	cout << "mqtt going offline" << endl;
}

void ofApp::onHelp(std::vector<std::string>& param)
{
	

	std::vector<std::string> c = m_cmds.getlist();
	ostringstream s;
	for (auto it = c.begin(); it != c.end(); ++it)
		s << *it << std::endl << "\n\r";

	m_clientmqtt.publish("/ofx/cell/output", s.str());
}
void ofApp::onScale(std::vector<std::string>& param)
{
	if (param.size() < 2)
		return;
	istringstream s(param[1]);
	
	float factor = 0;

	s >> factor;

	if (factor > 0.0)
		m_scale = factor;
}

void ofApp::onContour(std::vector<std::string>& cmd)
{
	if (cmd.size() < 2)
		return;

	istringstream s(cmd[1]);
	float factor = 0;

	s >> factor;
		
	m_contour  = (factor > 0.0) ? true : false;
}

void ofApp::onCurve(std::vector<std::string>& cmd)
{
	if (cmd.size() < 2)
		return;

	istringstream s(cmd[1]);

	float factor = 0;

	s >> factor;

	m_curve_svg = (factor > 0.0) ? true : false;
}
void ofApp::onFill(std::vector<std::string>& cmd)
{
	if (cmd.size() < 2) 
		return;

	istringstream s(cmd[1]);
	
	float factor = 0;
	s >> factor;

	m_draw_fill = (factor > 0.0) ? true : false;
}

void ofApp::onSpeed (std::vector<std::string>& cmd)
{
	if (cmd.size() < 2)
		return;

	istringstream s(cmd[1]);
	
	float factor = 0;

	s >> factor;

	if (factor > 0.0)
		m_speed = factor;
}

void ofApp::onReset(std::vector<std::string>& cmd)
{
	m_pop.getContainer().clear();
	m_pop.delete_all_cell();
	m_pop.setiteration(0);

	for (float i = 0; i < 360.0; i += 360. / m_nb_start_pt)
	{
		m_pop.addCell(CCell(cos(ofDegToRad(i)) * 32, sin(ofDegToRad(i)) * 32));
	}

	m_pop.updateposition(true);
}

void ofApp::onRepulse(std::vector<std::string>& param)
{
	if (param.size() < 2)
		return;
	istringstream s(param[1]);

	float factor = 0;

	s >> factor;

	if (factor > 0.0)
	{
		
		for (int i = 0; i < this->m_rules.size(); i++)
		{
			if (m_rules[i]->gettype() == REPULSE)
			{
				m_rules[i]->setCoef(factor);
			}
		}
	}
}

void ofApp::onCenter(std::vector<std::string>& param)
{
	if (param.size() < 2)
		return;
	istringstream s(param[1]);
	
	float factor = 0;

	s >> factor;

	if (factor > 0.0)
	{

		for (int i = 0; i < this->m_rules.size(); i++)
		{
			if (m_rules[i]->gettype() == CENTER)
			{
				m_rules[i]->setCoef(factor);
			}
		}
	}
}

void ofApp::onBrownian(std::vector<std::string>& param)
{
	istringstream s(param[1]);
	float factor = 0;

	s >> factor;

	if (factor > 0.0)
	{

		for (int i = 0; i < this->m_rules.size(); i++)
		{
			if (m_rules[i]->gettype() == BROWNIAN)
			{
				m_rules[i]->setCoef(factor);
			}
		}
	}
}

void ofApp::onMiddle(std::vector<std::string>& cmd)
{
	if (cmd.size() < 2)
		return;
	istringstream s(cmd[1]);
	float factor = 0;

	s >>  factor;

	for (int i = 0; i < this->m_rules.size(); i++)
	{
		if (m_rules[i]->gettype() == MIDDLE)
		{
			m_rules[i]->setCoef(factor);
		}
	}
}

void ofApp::onMaxPts(std::vector<std::string>& cmd)
{
	if (cmd.size() < 2)
		return;

	istringstream s(cmd[1]);

	float factor = 0;

	s >> factor;

	if (factor > 0.0)
	{

		m_maxpts = (size_t) factor;
	}
}

void ofApp::onDie(std::vector<std::string>& param)
{
	if (param.size() < 2)
		return;

	istringstream s(param[1]);
	float factor = 0;

	s >> factor;

	m_dieprob = factor;
	
}
void ofApp::onMessage(ofxMQTTMessage & msg)
{
	cout << msg.topic << " " << msg.payload << endl;
	m_last_message = msg.payload;
	
	m_cmds.execute(m_last_message);

}

void ofApp::onTest(std::vector<std::string>& param)
{
	m_display = param;
}