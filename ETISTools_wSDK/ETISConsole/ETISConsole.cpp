// ETISConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _HAS_STD_BYTE 0

#include <iostream>
#include <string.h>
#include <ISTools/Topic.h>
#include "CETISRecConsole.h"

#define EC_VER			"0.0.1"

#define REC_CFG_FILE	"ETISSTDRec.cfg"

//#define DEFAULT_USER	"ETISConsole"
//#define DEFAULT_PASSWD	"ETISConsole"

#define DEFAULT_USER	"WndEtis"
#define DEFAULT_PASSWD	"mcwnd08"

#define EXIT_CMD		"exit"
#define HELP_CMD		"help"
#define STATUS_CMD		"status"
#define REC_CMD			"rec"
#define SNAP_CMD		"snap"
#define CLEAR_CMD		"clear"
#define STOP_CMD		"stop"
#define ALL_CMD			"all"

using namespace std;

CETISRecConsole* pRec = 0;
string	m_sHost,
		m_sPort;

void StopRec()
{
	if (pRec)
	{
		pRec->Disconnect();
		delete pRec;
		pRec = 0;
		cout << m_sHost << " disconnected" << endl;
	}
}

bool StartNewRec(const string& sHost, const string& sPort, string& sError, const string& sUser = string(), const string& sPassword = string())
{
	StopRec();

	pRec = new CETISRecConsole();

	if (!pRec->SetConfigFile(REC_CFG_FILE, sError))
	{
		sError = "Configuration file not set: " + sError;
		delete pRec;
		pRec = 0;
		return false;
	}

	if (!pRec->Initialize(sError))
	{
		sError = "Error initializing recevier: " + sError;
		delete pRec;
		pRec = 0;
		return false;
	}

	if (!pRec->Connect(sUser.empty() ? DEFAULT_USER : sUser, sPassword.empty() ? DEFAULT_PASSWD : sPassword, sError, false))
	{
		sError = "Connection failed: " + sError;
		delete pRec;
		pRec = 0;
		return false;
	}

	return true;
}

void PrintTopic(const its::Topic& Tpc = its::Topic())
{
	if (pRec)
		pRec->DumpCache(Tpc);
	else
		cout << "not connected" << endl;
}

void Status()
{
	if (pRec)
		cout << "connected to " << m_sHost << ":" << m_sPort <<	" with " << pRec->CacheSize() << " topic" << endl;
	else
		cout << "not connected" << endl;
}

void Clear()
{
	if (pRec && pRec->Clear())
		cout << "cache cleared" << endl;
	else
		cout << "not connected" << endl;
}

void Help()
{
	// TODO fare i subset di tag
	cout << "\trec host port [user] [password]\tconnect to ETIS publisher on given host:port" << endl;
	cout << "\tsnap topic [ref|trade|book]\t\tsubscribe snapshot using urrent connection for given topic with given tag list" << endl;
	cout << "\tstatus\t\t\t\tprint current connection (if any)" << endl;
	cout << "\tall\t\t\t\tprint all topic in cache" << endl;
	cout << "\tclear\t\t\t\tclear cache" << endl;
	cout << "\tstop\t\t\t\tclose current connection (if any)" << endl;
	cout << "\thelp\t\t\t\tprint this help" << endl;
	cout << "\texit\t\t\t\tquit ETISConsole" << endl;
}

bool ParseCommand()
{
	string sCommand;

	cout << "> ";
	getline(cin, sCommand);

	transform(sCommand.begin(), sCommand.end(), sCommand.begin(), ::tolower);

	vector<string>	vRec,
					vSnap;

	const char* delim = " ";
	char* next_token;
	char* token = strtok_s((char*)sCommand.c_str(), " ", &next_token);

	while (token)
	{
		string sToken(token);

		if (vRec.empty() && vSnap.empty())
		{
			if (sToken.compare(EXIT_CMD) == 0)
				return false;

			if (sToken.compare(HELP_CMD) == 0)
				Help();
			else if (sToken.compare(STATUS_CMD) == 0)
				Status();
			else if (sToken.compare(STOP_CMD) == 0)
				StopRec();
			else if (sToken.compare(ALL_CMD) == 0)
				PrintTopic();
			else if (sToken.compare(CLEAR_CMD) == 0)
				Clear();
			else if (sToken.compare(REC_CMD) == 0)
				vRec.push_back(sToken);
			else if (sToken.compare(SNAP_CMD) == 0)
				vSnap.push_back(sToken);
			else
			{
				transform(sToken.begin(), sToken.end(), sToken.begin(), ::toupper);
				if (pRec && pRec->IsTopicPresent(sToken))
					PrintTopic(sToken);
				else
					cout << "command unknown" << endl;
				break;
			}
		}
		else if (!vRec.empty())
			vRec.push_back(sToken);
		else if (!vSnap.empty())
		{
			transform(sToken.begin(), sToken.end(), sToken.begin(), ::toupper);
			vSnap.push_back(sToken);
		}
		
		token = strtok_s(NULL, " ", &next_token);
	}

	if (!vRec.empty())
	{
		string sError;
		m_sHost = vRec.at(1);
		m_sPort = vRec.at(2);

		bool bRecRes = vRec.size() < 4 ? StartNewRec(m_sHost, m_sPort, sError) : StartNewRec(m_sHost, m_sPort, sError, vRec.at(3), vRec.at(4));
		
		if (bRecRes)
			cout << "connected" << endl;
		else
			cout << sError << endl;
	}

	if (!vSnap.empty())
	{
		string sError;
		const string& sTopic = vSnap.at(1);
		
		if (pRec)
		{
			if (!pRec->Subscribe(its::Topic(sTopic), REQ_SNAPSHOT, TagSet(), false, sError))
				cout << sError << endl;
			else
				cout << "subscribed" << endl;
		}
		else
			cout << "not connected" << endl;
	}

	return true;
}

void Init()
{
	cout << "ETISConsole ver. " << EC_VER << endl;
}
	
int main()
{
	Init();

	bool bExit = false;

	while (!bExit)
		bExit = !ParseCommand();

	StopRec();
}


