#pragma once

#include "ofMain.h"
#include "ofxMQTT.h"
#include "CellPopulation.h"
#include "RuleFactory.h"
#include "StringCmd.h"

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

		
		void onBrownian(std::vector<std::string>& param);
		void onCenter(std::vector<std::string>& param);
		void onCurve(std::vector<std::string>& cmd);
		void onContour(std::vector<std::string>& cmd);
		void onDie(std::vector<std::string>& param);
		void onFill(std::vector<std::string>& cmd);
		void onHelp(std::vector<std::string>& param);
		void onMaxPts(std::vector<std::string>& cmd);
		void onMiddle(std::vector<std::string>& param);
		void onRepulse(std::vector<std::string>& param);
		void onScale(std::vector<std::string>& param);
		void onSpeed(std::vector<std::string>& cmd);
		void onTest(std::vector<std::string>& param);
		void onReset(std::vector<std::string>& cmd);
		
		
		void onMessage(ofxMQTTMessage &msg);

		ofxMQTT m_clientmqtt;
		

		
		

		CStringCmdInterpretor m_cmds;
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

		std::vector<std::string> m_display;
};
