#define _HAS_STD_BYTE 0

#include <iostream>
#include "CETISRecConsole.h"

#define	STATUS_CONN_TPC		its::Topic("STATUS.CONNECTION.*")
#define	ERR_SUB_TPC			its::Topic("ERR.SUB")

using namespace std;

CETISRecConsole::CETISRecConsole()
{}

CETISRecConsole::~CETISRecConsole()
{}

bool CETISRecConsole::OnUpdate(const its::Topic& Tpc, const TagValueMap& mValues)
{
	if (!Tpc.Matches(STATUS_CONN_TPC) && !Tpc.Matches(ERR_SUB_TPC))
	{
		cout << endl << Tpc.Get() << " - " << DumpTagValues(mValues) << endl;
		
		CacheMap::iterator itC = Cache.find(Tpc);

		if (itC == Cache.end())
			Cache.insert(make_pair(Tpc, mValues));
		else
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

void CETISRecConsole::DumpCache(const its::Topic& Tpc)
{
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
			cout << itC->first.Get() << " - " << DumpTagValues(itC->second) << endl;
			itC++;
		}
	}	
}

bool CETISRecConsole::Clear()
{
	Cache.clear();
	return true;
}