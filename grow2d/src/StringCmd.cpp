
#include "ofApp.h"
#include "StringCmd.h"

void CStringCmd::execute(std::vector<std::string>& param) {
	if (m_pcallback != nullptr)
		((*m_pcontroler).*(m_pcallback)) (param);
	//std::invoke(*m_pcontroler, m_pcallback, param);
}