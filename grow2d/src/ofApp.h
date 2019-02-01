#pragma once

#include "ofMain.h"
#include "ofxMQTT.h"
#include "CellPopulation.h"
#include "RuleFactory.h"

class ofApp : public ofBaseApp{

	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		

		void onOnline();
		void onOffline();
		void onHelp();
		void onScale(std::string msg);
		void onContour(std::string msg);
		void onCurve(std::string msg);
		void onFill(std::string msg);
		void onSpeed(std::string msg);
		void onRepulse(std::string msg);
		void onCenter(std::string msg);
		void onBrownian(std::string msg);
		void onMiddle(std::string msg);
		void onMaxPts(std::string msg);
		void onDie(std::string msg);
		void onMessage(ofxMQTTMessage &msg);

		ofxMQTT m_clientmqtt;
		bool m_renewmqtt;

		CPopulation m_pop;
		VRule		m_rules;

		float m_scale;
		size_t m_maxpts;
		float m_speed;
		int m_nb_start_pt;
		bool m_save;
		bool m_save_svg;
		bool m_curve_svg;
		bool m_contour;
		bool m_draw_fill;
		

		float m_dieprob;

		float m_min = 0;
		float m_max = 1;
};
