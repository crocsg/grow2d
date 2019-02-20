#pragma once


#include <string>
#include <vector>
#include <istream>
#include <sstream>

class ofApp;
typedef void (ofApp::*cmdcallback) (std::vector<string>& param);

class CStringCmd
{
public:
	/*
	CStringCmd (const std::string& cmd, cmdcallback callback)
	{
		m_cmd = cmd;
		m_pcallback = callback;
	}
	*/
	CStringCmd(const char* pcmd, ofApp* pcontroler, cmdcallback callback)
	{
		m_cmd = pcmd;
		m_pcontroler = pcontroler;
		m_pcallback = callback;
	}


	const CStringCmd& operator= (CStringCmd& cmd)
	{
		this->m_cmd = cmd.m_cmd;
		this->m_pcontroler = cmd.m_pcontroler;
		this->m_pcallback = cmd.m_pcallback;

		return (*this);
	}

	bool compare(std::string& str) { return  (str.find(m_cmd) == 0) ? true : false; }

	void execute(std::vector<std::string>& param); 

	const std::string& getName() { return m_cmd; }

private:
	CStringCmd() {}

	std::string m_cmd;
	ofApp* m_pcontroler;
	cmdcallback m_pcallback;
};

class CStringCmdInterpretor
{
public:
	CStringCmdInterpretor() {}
	CStringCmdInterpretor(const std::vector<CStringCmd> cmds) { m_cmds = cmds; }
	
	const CStringCmdInterpretor& operator= (CStringCmdInterpretor& r)
	{
		this->m_cmds = r.m_cmds;

		return (*this);
	}

	void add(std::string& cmd, ofApp* pcontroler, cmdcallback pcallback)
	{
		m_cmds.push_back(CStringCmd(cmd.c_str(), pcontroler, pcallback));
	}

	void remove(std::string cmd)
	{
		for (auto it = m_cmds.begin(); it != m_cmds.end(); ++it)
		{
			if (it->compare(cmd))
			{
				m_cmds.erase(it);
				return;
			}
		}
	}

	std::vector<std::string> split(std::string str)
	{
		std::istringstream iss(str);
		std::vector<std::string> res (std::istream_iterator<std::string>{iss},
			std::istream_iterator<std::string>());

		return res;
	}

	void execute(std::string cmd)
	{
		std::vector<std::string> p;
		p = split(cmd);
		
		for (auto it = m_cmds.begin(); it != m_cmds.end(); it++)
		{
			if (it->compare(cmd))
			{
				it->execute(p);
				return;
			}
		}
	}

	std::vector<std::string> getlist()
	{
		std::vector<std::string> p;
		
		p.reserve(m_cmds.size());
		for (auto it = m_cmds.begin(); it != m_cmds.end(); ++it)
				p.push_back(it->getName());
		
		return p;
	}
private:
	std::vector<CStringCmd>	m_cmds;
};