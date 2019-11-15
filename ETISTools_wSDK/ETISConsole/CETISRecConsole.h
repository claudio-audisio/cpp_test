#pragma once

#include <ETISRec\ETISReceiver.h>
#include <atomic>

using namespace std;

class CETISRecConsole : public CETISReceiver
{
public:
	CETISRecConsole();
	~CETISRecConsole();

	enum UpdateMode
	{
		UM_ALL,
		UM_SNAP,
		UM_NONE
	};

	bool Subscribe(const its::Topic& Tpc, ReqType eRequest, const TagSet& vTags, bool bSynchronous, string& sError);
	unsigned int CacheSize() { return Cache.size(); }
	void DumpCache(const its::Topic& Tpc, const TagSet& vTags);
	bool IsTopicPresent(const its::Topic& Tpc) { return Cache.find(Tpc) != Cache.end(); }
	bool Clear();
	void PrintTopicList(const its::Topic& TpcFilter = its::Topic());
	void SetUpdateMode(const UpdateMode eMode);
	string GetUpdateMode_Str() const;

protected:
	bool OnUpdate(const its::Topic& Tpc, const TagValueMap& mValues);
	bool OnReply(const string& sRequestID, const TagValueMap& mValues, bool bLast);
	void OnServerStatus(const string& sServer, ServerStatus eStatus, const string& sDescription);

private:
	typedef std::map<its::Topic, TagValueMap> CacheMap;
	CacheMap Cache;
	bool m_bConnected;
	HANDLE m_hConsole;
	UpdateMode m_eUpdateMode;
	atomic<bool> m_bCacheBusy;
	vector<its::Topic> m_vSubscriptions;
};