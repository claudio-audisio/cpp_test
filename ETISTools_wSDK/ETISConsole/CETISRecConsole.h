#pragma once

#include <ETISRec\ETISReceiver.h>

class CETISRecConsole : public CETISReceiver
{
public:
	CETISRecConsole();
	~CETISRecConsole();
	unsigned int CacheSize() { return Cache.size(); }
	void DumpCache(const its::Topic& Tpc);
	bool IsTopicPresent(const its::Topic& Tpc) { return Cache.find(Tpc) != Cache.end(); }
	bool Clear();

protected:
	bool OnUpdate(const its::Topic& Tpc, const TagValueMap& mValues);
	bool OnReply(const string& sRequestID, const TagValueMap& mValues, bool bLast);
	void OnServerStatus(const string& sServer, ServerStatus eStatus, const string& sDescription);

private:
	typedef std::map<its::Topic, TagValueMap> CacheMap;
	CacheMap Cache;
};
