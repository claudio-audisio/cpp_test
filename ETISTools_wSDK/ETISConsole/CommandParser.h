#pragma once

#include "CETISRecConsole.h"
#include <ISInterface/ISDictionary.h>
#include <ITLib/TagValue.h>

#define _HAS_STD_BYTE 0

#define MAX_LINE_SIZE	10*1024

#define EXIT_CMD		"exit"
#define HELP_CMD		"help"
#define STATUS_CMD		"status"
#define REC_CMD			"rec"
#define SUB_CMD			"sub"
#define UNSUB_CMD		"unsub"
#define CLEAR_CMD		"clear"
#define CLOSE_CMD		"close"
#define ALL_CMD			"all"
#define TOPICS_CMD		"topics"
#define UPDATE_MODE_CMD	"update"

using namespace std;

TagSet	m_vAvailableTags{ T_AVAILABLE, T_SNAPSHOT, T_TO_SNAP };
TagSet	m_vTradeTags{ T_CONTRACT_ID, T_CONTRACT_ID_CHART_LOW, T_CONTRACT_ID_CHART_HIGH, T_TIMESTAMP_NANO, T_VALID_CONTRACT, T_DELAY_CONTRACT, T_MARKET_CHANNEL, T_LAST_PRC, T_LAST_QTY, T_LAST_TIME, T_LAST_CME_IND, T_LAST_YIELD, T_LAST_NOM_VALUE, T_LAST_ID, T_LAST_DATE, T_LAST_AUTOMATIC_TRADE, T_LAST_AUTOMATIC_TIME };
TagSet	m_vReferenceTags{};
TagSet	m_vPhaseTags{};
TagSet	m_vBookTags{};
TagSet	m_vBookOrderTags{};
TagSet	m_vPerformanceTags{};
TagSet	m_vStatisticsTags{};

extern CETISRecConsole* m_pRec;
extern bool StartNewRec(const string& sHost, const string& sPort, string& sError, const string& sUser = string(), const string& sPassword = string());
extern string m_sHost;
extern string m_sPort;

bool CreateTagSet(const string& sTagSet, TagSet& vTags)
{
	TagList vTagList;

	if (sTagSet.compare("avail") == 0)
		vTags = m_vAvailableTags;
	else if (sTagSet.compare("trade") == 0)
		vTags = m_vTradeTags;
	else if (Value(sTagSet).ParseTagList(vTagList))
	{
		for (int i = 0; i < vTagList.size(); i++)
			vTags.insert(vTagList[i]);
	}

	return true;
}

bool PrintTopic(const its::Topic& Tpc = its::Topic(), const string& sTagSet = string())
{
	TagSet vTags;

	if (!sTagSet.empty())
		CreateTagSet(sTagSet, vTags);

	if (m_pRec)
		m_pRec->DumpCache(Tpc, vTags);
	else
		cout << "not connected" << endl;

	return true;
}

bool Status()
{
	if (m_pRec)
		cout << "connected to " << m_sHost << ":" << m_sPort << " with " << m_pRec->CacheSize() << " topic (update mode " << m_pRec->GetUpdateMode_Str() << ")" <<  endl;
	else
		cout << "not connected" << endl;

	return true;
}

bool Clear()
{
	if (m_pRec && m_pRec->Clear())
		cout << "cache cleared" << endl;
	else
		cout << "not connected" << endl;

	return true;
}

bool Help()
{
	cout << "\trec <host> <port> [<user>] [<password>]\tconnect to ETIS publisher on given host:port" << endl;
	cout << "\tsub <topic> [avail|trade|<tag list>]\tsubscribe snapshot using urrent connection for given topic with given tag list" << endl;
	cout << "\t<topic> [avail|trade|<tag list>]\tshow data for given topic with given tag list" << endl;
	cout << "\tstatus\t\t\t\t\tprint current connection (if any)" << endl;
	cout << "\tall\t\t\t\t\tprint all data for all topics in cache" << endl;
	cout << "\ttopics [<topic filter>]\t\t\tshow list of topic in cache that matches with filter (if any)" << endl;
	cout << "\tclear\t\t\t\t\tunsubscribe and remove all topics from cache" << endl;
	cout << "\tclose\t\t\t\t\tclose current connection (if any)" << endl;
	cout << "\tupdate [all|snap|none]\t\t\tset frequency of update showing" << endl;
	cout << "\thelp\t\t\t\t\tprint this help" << endl;
	cout << "\texit\t\t\t\t\tquit ETISConsole" << endl;

	return true;
}

bool CloseRec()
{
	if (m_pRec)
	{
		m_pRec->Disconnect();
		delete m_pRec;
		m_pRec = 0;
		//cout << m_sHost << " disconnected" << endl;
	}

	return true;
}

bool Tokenize(const string& sCommandLine, vector<string>& vCommandList)
{
	char* next_token;
	char* token = strtok_s((char*)sCommandLine.c_str(), " ", &next_token);

	while (token)
	{
		vCommandList.push_back(string(token));
		token = strtok_s(NULL, " ", &next_token);
	}

	return vCommandList.size() > 0;
}

bool ParsePrintCommand(const string& sCommand)
{
	vector<string> vPrint;

	if (Tokenize(sCommand, vPrint))
	{
		string sTagSet;

		if (vPrint.size() > 1)
			sTagSet = vPrint.at(1);

		string sTopic = vPrint.at(0);
		transform(sTopic.begin(), sTopic.end(), sTopic.begin(), ::toupper);
		PrintTopic(its::Topic(sTopic), sTagSet);
	}

	return true;
}

bool ParseSubCommand(const string& sCommand)
{
	vector<string> vSub;

	if (Tokenize(sCommand, vSub))
	{
		string sError;
		TagSet vTags;

		string sTopic = vSub.at(1);
		transform(sTopic.begin(), sTopic.end(), sTopic.begin(), ::toupper);

		if (vSub.size() > 2)
			CreateTagSet(vSub.at(2), vTags);

		if (m_pRec)
		{
			if (!m_pRec->Subscribe(its::Topic(sTopic), REQ_SNAP_REF, vTags, false, sError))
				cout << sError << endl;
			//else
				//cout << "subscribed" << endl;
		}
		else
			cout << "not connected" << endl;
	}

	return true;
}

bool ParseRecCommand(const string& sCommand)
{
	vector<string> vRec;

	if (Tokenize(sCommand, vRec))
	{
		if (vRec.size() != 3 && vRec.size() != 5)
		{
			cout << "wrong parameters" << endl;
			return true;
		}

		string sError;
		m_sHost = vRec.at(1);
		m_sPort = vRec.at(2);

		CloseRec();

		bool bRecRes = vRec.size() < 4 ? StartNewRec(m_sHost, m_sPort, sError) : StartNewRec(m_sHost, m_sPort, sError, vRec.at(3), vRec.at(4));

		if (!bRecRes)
			cout << sError << endl;
		//else
			//cout << "connected" << endl;
	}

	return true;
}

bool ParseTopicsCommand(const string& sCommand)
{
	vector<string> vTopics;
	string sTopicFilter;

	if (Tokenize(sCommand, vTopics))
	{
		if (m_pRec)
		{
			if (vTopics.size() > 1)
			{
				string sTopicFilter = vTopics.at(1);
				transform(sTopicFilter.begin(), sTopicFilter.end(), sTopicFilter.begin(), ::toupper);
				m_pRec->PrintTopicList(its::Topic(sTopicFilter));
			}
			else
				m_pRec->PrintTopicList();
		}
		else
			cout << "not connected" << endl;
	}

	return true;
}

bool ParseUpdateModeCommand(const string& sCommand)
{
	vector<string> vUpdateMode;

	if (Tokenize(sCommand, vUpdateMode))
	{
		if (vUpdateMode.size() != 2)
		{
			cout << "wrong parameters" << endl;
			return true;
		}
		
		const string& sMode = vUpdateMode.at(1);

		if (m_pRec)
		{
			if (sMode.compare("all") == 0)
				m_pRec->SetUpdateMode(CETISRecConsole::UM_ALL);
			else if (sMode.compare("snap") == 0)
				m_pRec->SetUpdateMode(CETISRecConsole::UM_SNAP);
			else if (sMode.compare("none") == 0)
				m_pRec->SetUpdateMode(CETISRecConsole::UM_NONE);
			else
				cout << "wrong parameters" << endl;
		}
		else 
			cout << "not connected" << endl;
	}

	return true;
}

bool ParseCommand()
{
	string sCommandLine;
	
	cout << "> ";

	getline(cin, sCommandLine);

	transform(sCommandLine.begin(), sCommandLine.end(), sCommandLine.begin(), ::tolower);

	string::size_type nPos = sCommandLine.find(" ");

	string sCommand = sCommandLine.substr(0, nPos);

	if (sCommand.compare(EXIT_CMD) == 0)
		return false;

	if (sCommand.compare(HELP_CMD) == 0)
		return Help();
	
	if (sCommand.compare(STATUS_CMD) == 0)
		return Status();
	
	if (sCommand.compare(CLOSE_CMD) == 0)
		return CloseRec();
	
	if (sCommand.compare(ALL_CMD) == 0)
		return PrintTopic();
	
	if (sCommand.compare(CLEAR_CMD) == 0)
		return Clear();
	
	if (sCommand.compare(REC_CMD) == 0)
		return ParseRecCommand(sCommandLine);
	
	if (sCommand.compare(SUB_CMD) == 0)
		return ParseSubCommand(sCommandLine);

	if (sCommand.compare(TOPICS_CMD) == 0)
		return ParseTopicsCommand(sCommandLine);

	if (sCommand.compare(UPDATE_MODE_CMD) == 0)
		return ParseUpdateModeCommand(sCommandLine);

	transform(sCommand.begin(), sCommand.end(), sCommand.begin(), ::toupper);

	if (m_pRec && m_pRec->IsTopicPresent(sCommand))
		return ParsePrintCommand(sCommandLine);
	
	cout << "command unknown" << endl;

	return true;
}
