#define _HAS_STD_BYTE 0

#include <iostream>
#include "CETISRecConsole.h"
#include <ISInterface/ISDictionary.h>

#define	STATUS_CONN_TPC				its::Topic("STATUS.CONNECTION.*")
#define	SL_TOPIC_ERROR_SUB_TPC		its::Topic(SL_TOPIC_ERROR_SUB)
#define SL_TOPIC_ERROR_QUEUE_TPC	its::Topic(SL_TOPIC_ERROR_QUEUE)

using namespace std;

CETISRecConsole::CETISRecConsole() :
	m_bConnected(false),
	m_eUpdateMode(UM_SNAP)
{
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	m_bCacheBusy.store(false);
}

CETISRecConsole::~CETISRecConsole()
{}

bool CETISRecConsole::Subscribe(const its::Topic& Tpc, ReqType eRequest, const TagSet& vTags, bool bSynchronous, string& sError)
{
	if (CETISReceiver::Subscribe(Tpc, eRequest, vTags, bSynchronous, sError))
	{
		m_vSubscriptions.push_back(Tpc);
		return true;
	}
		
	return false;
}

bool CETISRecConsole::OnUpdate(const its::Topic& Tpc, const TagValueMap& mValues)
{
	if (Tpc.Matches(SL_TOPIC_ERROR_SUB_TPC))
	{
		TagValueMap::const_iterator itT = mValues.find(T_ERROR_DESC);

		SetConsoleTextAttribute(m_hConsole, 4);

		if (itT != mValues.end())
			cout << "subscription error: " << itT->second.GetString() << endl;
		else
			cout << "subscription error" <<  endl;

		SetConsoleTextAttribute(m_hConsole, 7);

		return true;
	}

	if (Tpc.Matches(SL_TOPIC_ERROR_QUEUE_TPC))
	{
		TagValueMap::const_iterator itT = mValues.find(T_ERROR_MESSAGE);

		SetConsoleTextAttribute(m_hConsole, 4);

		if (itT != mValues.end())
			cout << "subscription error: " << itT->second.GetString() << endl;
		else
			cout << "subscription error" << endl;

		SetConsoleTextAttribute(m_hConsole, 7);

		return true;
	}

	if (Tpc.Matches(STATUS_CONN_TPC))
	{
		TagValueMap::const_iterator itT = mValues.find(T_AVAILABLE);

		if (itT != mValues.end())
		{
			m_bConnected = itT->second.GetBool();
			
			if (m_bConnected)
			{
				SetConsoleTextAttribute(m_hConsole, 10);
				cout << "connected" << endl;
			}
			else
			{
				SetConsoleTextAttribute(m_hConsole, 4);
				cout << "disconnected" << endl;
			}

			SetConsoleTextAttribute(m_hConsole, 7);
		}

		return true;
	}

	if (!m_bConnected)
		return false;

	if (m_bCacheBusy.load())
		return false;

	CacheMap::iterator itC = Cache.find(Tpc);

	if (itC == Cache.end())
	{
		bool bBusy = false;

		if (m_bCacheBusy.compare_exchange_weak(bBusy, true))
		{
			m_bCacheBusy.store(true);

			Cache.insert(make_pair(Tpc, mValues));

			if (m_eUpdateMode != UM_NONE)
			{
				SetConsoleTextAttribute(m_hConsole, 10);
				cout << endl << Tpc.Get() << " - " << DumpTagValues(mValues) << endl;
				SetConsoleTextAttribute(m_hConsole, 7);
				cout << "> ";
			}

			m_bCacheBusy.store(false);
		}
	}			
	else
	{
		if (m_eUpdateMode == UM_ALL)
		{
			SetConsoleTextAttribute(m_hConsole, 10);
			cout << endl << Tpc.Get() << " - " << DumpTagValues(mValues) << endl;
			SetConsoleTextAttribute(m_hConsole, 7);
			cout << "> ";
		}

		itC->second += mValues;
	}

	return true;
}

bool CETISRecConsole::OnReply(const string& sRequestID, const TagValueMap& mValues, bool bLast)
{
	//cout << "received reply for " << sRequestID << endl;
	return true;
}

void CETISRecConsole::OnServerStatus(const string& sServer, ServerStatus eStatus, const string& sDescription)
{
	//cout << "received status " << eStatus << " from " << sServer << ": " << sDescription << endl;
}

void CETISRecConsole::DumpCache(const its::Topic& Tpc, const TagSet& vTags)
{
	if (m_bCacheBusy.load())
	{
		cout << "retry later" << endl;
		return;
	}

	SetConsoleTextAttribute(m_hConsole, 10);

	if (Tpc == its::Topic())
	{
		CacheMap::const_iterator itC = Cache.begin();

		while (itC != Cache.end())
		{
			cout << itC->first.Get() << " - " << DumpTagValues(itC->second) << endl;
			itC++;
		}
	}
	else
	{
		CacheMap::const_iterator itC = Cache.find(Tpc);

		if (itC != Cache.end())
		{
			if (vTags.empty())
				cout << itC->first.Get() << " - " << DumpTagValues(itC->second) << endl;
			else
			{
				TagValueMap mValues = itC->second;
				TagValueMap::iterator itT = mValues.begin();

				while (itT != mValues.end())
				{
					if (vTags.find(itT->first) == vTags.end())
						itT = mValues.erase(itT);
					else
						itT++;
				}

				if (!mValues.empty())
					cout << itC->first.Get() << " - " << DumpTagValues(mValues) << endl;
				else
					cout << "tag(s) not found" << endl;
			}
		}
	}

	SetConsoleTextAttribute(m_hConsole, 7);
}

bool CETISRecConsole::Clear()
{
	string sError;

	for (int i = 0; i < m_vSubscriptions.size(); i++)
		Subscribe(m_vSubscriptions[i], REQ_FORGET, TagSet(), false, sError);
	   
	bool bBusy = false;

	if (m_bCacheBusy.compare_exchange_weak(bBusy, true))
	{
		CSystemTime::Sleep(1000);

		Cache.clear();

		m_bCacheBusy.store(false);
	}
	else
		cout << "retry later" << endl;

	return true;
}

void CETISRecConsole::PrintTopicList(const its::Topic& TpcFilter /* = its::Topic() */)
{
	if (m_bCacheBusy.load())
	{
		cout << "retry later" << endl;
		return;
	}

	const bool bFilter = TpcFilter != its::Topic();

	CacheMap::const_iterator itC = Cache.begin();

	while (itC != Cache.end())
	{
		if (!bFilter || (bFilter && TpcFilter.Matches(itC->first)))
			cout << itC->first.Get() << endl;

		itC++;
	}
}

void CETISRecConsole::SetUpdateMode(const UpdateMode eMode)
{
	m_eUpdateMode = eMode;

	switch (eMode)
	{
	case UM_ALL: 
	case UM_SNAP:
	case UM_NONE: 
		cout << "update mode set" << endl; return;
		return;
	default:
		cout << "wrong parameter" << endl;
		break;
	}
}

string CETISRecConsole::GetUpdateMode_Str() const
{
	switch (m_eUpdateMode)
	{
	case UM_ALL: return "all";;
	case UM_SNAP: return "snapshot";
	case UM_NONE: return "none";
	default: break;
	}

	return "";
}