#include "ofApp.h"

std::string m_last_message;

//--------------------------------------------------------------
void ofApp::setup(){
	m_clientmqtt.begin("devmqtt");
	m_clientmqtt.connect("ofx");
	m_clientmqtt.subscribe("/ofx/cell/param/input");

	ofAddListener(m_clientmqtt.onOnline, this, &ofApp::onOnline);
	ofAddListener(m_clientmqtt.onOffline, this, &ofApp::onOffline);
	ofAddListener(m_clientmqtt.onMessage, this, &ofApp::onMessage); 

	CRuleFactory rf;
	m_nb_start_pt = 8;

	for (int i = 0; i < 360; i += 360/m_nb_start_pt)
	{
		m_pop.getContainer().push_back(CCell(cos(ofDegToRad(i)) * 16, sin (ofDegToRad(i)) * 16));
	}
	m_rules.push_back(rf.BuildRule(BROWNIAN, 0.01));
	m_rules.push_back(rf.BuildRule(MIDDLE, 0.01));
	m_rules.push_back(rf.BuildRule(SPRING, 0.5));
	m_rules.push_back(rf.BuildRule(REPULSE, 0.5));
	m_rules.push_back(rf.BuildRule(CENTER, 1));
	m_scale = 1;
	m_speed = 1;
	m_maxpts = 2000;
	m_save = false;
	m_save_svg = false;
}

//--------------------------------------------------------------
void ofApp::update(){
	m_clientmqtt.update();

	for (auto it = m_rules.begin(); it != m_rules.end(); ++it)
	{
		(*it)->getspeed(m_pop.getContainer(), 0);
	}

	bool todo = false;
	if (m_pop.getContainer().size() < m_maxpts)
	{
		do
		{
			todo = false;
			for (auto nit = m_pop.getContainer().begin(); nit != m_pop.getContainer().end() - 1; ++nit)
			{
				auto nnit = nit + 1;
				float d = nit->pos().squareDistance(nnit->pos());
				if (d > 5 * 5)
				{
					ofVec3f p = (nit->pos() + nnit->pos()) / 2;
					m_pop.getContainer().insert(nnit, CCell(p.x, p.y));
					todo = true;
					break;
				}
			}
		} while (todo);
	}
	for (auto cit = m_pop.getContainer().begin(); cit != m_pop.getContainer().end(); ++cit)
	{
		cit->updatePosition(m_speed);
		
	}

	//m_clientmqtt.publish("/ofx/cell/output", "update");
	
}

float m_min = 0;
float m_max = 1;
//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(32);
	ofNoFill();
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
	if (m_save)
	{
		m_output.beginEPS("saved.eps", 0,0, ofGetWidth () * m_scale, ofGetHeight () * m_scale);
		
		
	}
	if (m_pop.getContainer().size() > 2)
	{	
		ofColor color;
		float min = 255;
		float max = 0;

		
		
		
		ofPath path;
		path.setFilled(false);
		path.setStrokeWidth(1);
		path.setStrokeColor(ofColor(0,0,0));
		path.curveTo(m_pop.getContainer().front().pos().x, m_pop.getContainer().front().pos().y);
		
		for (auto it = m_pop.getContainer().begin(); it != m_pop.getContainer().end() - 1; ++it)
		{
			auto nit = it + 1;
			float d = it->getSpeed().lengthSquared();
			float c = ofMap(d, m_min, m_max, 85, 170, true);
			if (m_save_svg)
			{
				ofSetColor(0);
				
			}
			else
			{
				color.setHsb(c, 255, 255);
				ofSetColor(color);
				
			}
			if (! m_save_svg)
				ofLine(it->getPos(), nit->getPos());
			path.curveTo(nit->pos().x, nit->pos().y);
			
			if (d < min) min = d;
			if (d > max) max = d;
			
		}
		ofLine(m_pop.getContainer().back().pos(), m_pop.getContainer().front().pos());
		path.curveTo(m_pop.getContainer().front().pos().x, m_pop.getContainer().front().pos().y);
		path.curveTo(m_pop.getContainer().at(1).pos().x, m_pop.getContainer().at(1).pos().y);
					
		if (!m_save_svg)
		{
			path.setFilled(true);
			path.setFillColor(ofColor(0, 255, 0, 32));
			path.setStrokeColor(ofColor(0, 255, 0, 32));
			path.setStrokeWidth(0);

		}
		path.draw();
		if (m_save_svg)
		{
			
			ofEndSaveScreenAsSVG();
			m_save_svg = false;
		}
		m_min = min;
		m_max = max;
	
	}
	
	
	for (auto cit = m_pop.getContainer().begin(); cit != m_pop.getContainer().end(); ++cit)
	{
		cit->clearSpeed();

	}
	cout << endl ;

	ofPopMatrix();
	ofSetColor(0, 255, 255);
	
	ostringstream s;
	
	s << "N :" << ofGetFrameNum() << " FPS :" << ofGetFrameRate() << " cell :" << m_pop.getContainer().size() << " ";


	ofDrawBitmapString(s.str(), 10, 10);
	ofDrawBitmapString(m_last_message, 10, 24);

	if (m_save)
	{
		m_output.endEPS();
		m_save = false;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'S' || key == 's')
		m_save_svg = true;
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
}

void ofApp::onOffline()
{
}
void ofApp::onHelp()
{
	m_clientmqtt.publish("/ofx/cell/output", "reset\n");
}
void ofApp::onScale(std::string msg)
{
	istringstream s(msg);
	string cmd;
	float factor = 0;

	s >> cmd >> factor;

	if (factor > 0.0)
		m_scale = factor;
}

void ofApp::onSpeed (std::string msg)
{
	istringstream s(msg);
	string cmd;
	float factor = 0;

	s >> cmd >> factor;

	if (factor > 0.0)
		m_speed = factor;
}

void ofApp::onRepulse(std::string msg)
{
	istringstream s(msg);
	string cmd;
	float factor = 0;

	s >> cmd >> factor;

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

void ofApp::onCenter(std::string msg)
{
	istringstream s(msg);
	string cmd;
	float factor = 0;

	s >> cmd >> factor;

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

void ofApp::onBrownian(std::string msg)
{
	istringstream s(msg);
	string cmd;
	float factor = 0;

	s >> cmd >> factor;

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

void ofApp::onMiddle(std::string msg)
{
	istringstream s(msg);
	string cmd;
	float factor = 0;

	s >> cmd >> factor;

	if (factor > 0.0)
	{

		for (int i = 0; i < this->m_rules.size(); i++)
		{
			if (m_rules[i]->gettype() == MIDDLE)
			{
				m_rules[i]->setCoef(factor);
			}
		}
	}
}

void ofApp::onMaxPts(std::string msg)
{
	istringstream s(msg);
	string cmd;
	float factor = 0;

	s >> cmd >> factor;

	if (factor > 0.0)
	{

		m_maxpts = (size_t) factor;
	}
}
void ofApp::onMessage(ofxMQTTMessage & msg)
{
	cout << msg.topic << " " << msg.payload << endl;
	m_last_message = msg.payload;

	if (m_last_message.find("reset") != -1)
	{
		m_pop.getContainer().clear();
		for (int i = 0; i < 360; i +=360 / m_nb_start_pt)
		{
			m_pop.getContainer().push_back(CCell(cos(ofDegToRad(i)) * 16, sin(ofDegToRad(i)) * 16));
		}
	}
	else if (m_last_message.find("scale") != -1)
	{
		onScale(m_last_message);
	}
	else if (m_last_message.find("brownian") != -1)
	{
		onBrownian(m_last_message);
	}
	else if (m_last_message.find("repulse") != -1)
	{
		onRepulse(m_last_message);
	}
	else if (m_last_message.find("center") != -1)
	{
		onCenter(m_last_message);
	}
	else if (m_last_message.find("middle") != -1)
	{
		onMiddle(m_last_message);
	}
	else if (m_last_message.find("max") != -1)
	{
		onMaxPts(m_last_message);
	}
	else if (m_last_message.find("speed") != -1)
	{
		onSpeed(m_last_message);
	}
	else if (m_last_message.find("help") != -1)
	{
		onHelp();
	}
}