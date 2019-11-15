// ETISConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _HAS_STD_BYTE 0

#include <iostream>
#include <string.h>
#include <ISTools/Topic.h>
#include "CETISRecConsole.h"
#include "CommandParser.h"

#define EC_VER			"0.1.0"

#define REC_CFG_FILE_ORIG	"ETISSTDRec.cfg"
#define REC_CFG_FILE		"ETISSTDRec_current.cfg"

//#define DEFAULT_USER	"ETISConsole"
//#define DEFAULT_PASSWD	"ETISConsole"
#define DEFAULT_USER	"WndEtis"
#define DEFAULT_PASSWD	"mcwnd08"

using namespace std;

CETISRecConsole* m_pRec = 0;
string	m_sHost,
		m_sPort;
FILE*	m_pRecConfFile;

void RemoveFile(const string& sFileName)
{
	FILE* pTemp = fopen(sFileName.c_str(), "r");

	if (pTemp != NULL)
	{
		fclose(pTemp);
		const string sCommand = string("del /q ") + sFileName;
		system(sCommand.c_str());
	}
}

void SetNewConnectionValues(const string& sHost, const string& sPort)
{
	RemoveFile(REC_CFG_FILE);

	const string sNewConnection = string("Connection = ") + sHost + string(":") + sPort + string("\n\0");

	fseek(m_pRecConfFile, 0, SEEK_SET);

	FILE* pCurrentCfgFile = fopen(REC_CFG_FILE, "w");

	char sLine[MAX_LINE_SIZE];
	
	while (fgets(sLine, MAX_LINE_SIZE, m_pRecConfFile) != NULL)
	{
		if (string(sLine).find("host:port") != string::npos)
			memcpy(sLine, sNewConnection.c_str(), sNewConnection.length() + 1);

		fputs(sLine, pCurrentCfgFile);
	}

	fflush(pCurrentCfgFile);
	fclose(pCurrentCfgFile);
}

bool StartNewRec(const string& sHost, const string& sPort, string& sError, const string& sUser, const string& sPassword)
{
	SetNewConnectionValues(sHost, sPort);

	m_pRec = new CETISRecConsole();

	if (!m_pRec->SetConfigFile(REC_CFG_FILE, sError))
	{
		sError = "Configuration file not set: " + sError;
		delete m_pRec;
		m_pRec = 0;
		return false;
	}

	if (!m_pRec->Initialize(sError))
	{
		sError = "Error initializing recevier: " + sError;
		delete m_pRec;
		m_pRec = 0;
		return false;
	}

	if (!m_pRec->Connect(sUser.empty() ? DEFAULT_USER : sUser, sPassword.empty() ? DEFAULT_PASSWD : sPassword, sError, false))
	{
		sError = "Connection failed: " + sError;
		delete m_pRec;
		m_pRec = 0;
		return false;
	}

	return true;
}


bool Init()
{
	cout << "ETISConsole ver. " << EC_VER << endl;

	string sError;

	if (!system(NULL))
	{
		cout << "command processor not available" << endl;
		return false;
	}

	m_pRecConfFile = fopen(REC_CFG_FILE_ORIG, "r");

	if (m_pRecConfFile == NULL)
	{
		cout << "missing receiver configuration file" << endl;
		return false;
	}

	return true;
}

void Shutdown()
{
	CloseRec();
	
	fclose(m_pRecConfFile);

	RemoveFile(REC_CFG_FILE);
}
	
int main()
{
	if (!Init())
		return 0;

	bool bExit = false;

	while (!bExit)
		bExit = !ParseCommand();

	Shutdown();
}
