// ConsoleInfrontTest.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#define MAX_MSG_COUNT 10

/** Include ETIS api */
#include <ETISRec/ETISReceiver.h>
#include <ISTools/Topic.h>
/** Include ETIS common tags */
#include <ISInterface/ISDictionary.h>

#include <string>
#include <iostream>

using namespace its;

CRITICAL_SECTION mutex;
HANDLE eDone;

/* Forward declarations */
class OurReceiver : public CETISReceiver
{
public:

    virtual bool OnUpdate(const its::Topic& Tpc,			//!< updated topic
        const TagValueMap& mValues	//!< specific values
    );

    virtual bool OnReply(const string& sRequestID,		//!< unique request ID
        const TagValueMap& mValues,	//!< reply tags
        bool bLast					//!< true if this is last reply for specified request
    );

    virtual void OnServerStatus(const string& sServer,		//!< server that changed its status (empty for all)
        ServerStatus eStatus,		//!< server connection status
        const string& sDescription	//!< event description
    );
};


int main(int argc, char* argv[])
{
    std::cout << "ETIS SDK Receiver test..." << std::endl;

    Sleep(2500);
    /* The first thing we need is an mutex */
    InitializeCriticalSection(&mutex);

    vector<OurReceiver*> receivers(1);
    for (unsigned int i = 0; i < receivers.size(); ++i)
    {
        receivers[i] = new OurReceiver;
    }
    const string confFile = "ESDK.cfg";
    string sError(320000, '\0');
    if (!receivers[0]->SetConfigFile(confFile, sError))
    {
        std::cout << "Failed to open the specified configuration file \""
            << argv[1] << "\": " << sError << std::endl;
        return 1;
    }

    /* Applies the configuration files settings and opens the listening server */
    if (!receivers[0]->Initialize(sError))
    {
        std::cout << "Failed to initialize the receiver: "
            << sError << std::endl;

        return 1;
    }

    eDone = CreateEvent(NULL, FALSE, FALSE, NULL);

    /* Applies the configuration files settings and opens the listening server */
    for (unsigned int i = 0; i < receivers.size(); ++i)
    {
        if (!receivers[i]->Connect("RecUser", "RecPassword", sError, true))
        {
            std::cout << "Failed to start the connection:"
                << sError << std::endl;
            return 1;
        }
        else
        {
            string sTopic = "TIT.FCA.MTA";
            Topic topic(sTopic);
            if (receivers[i]->Subscribe(topic, REQ_SNAP_REF, TagSet(), false, sError))
                std::cout << "Subscription accepted";
            else
                std::cout << "Failed to subscribe:"
                << sError << std::endl;
        }
    }

    /* Ok, we started the connection process. Now it's time to wait for the connection;
       for simplicity, this program will subscribe directly from the OnServerStatus
       callback, and will wait here for termination request. */
    WaitForSingleObject(eDone, INFINITE);

    std::cout << "Terminating." << std::endl;

    CloseHandle(eDone);
    for (unsigned int i = 0; i < receivers.size(); ++i)
    {
        receivers[i]->Disconnect();
        delete receivers[i];
    }

    return 0;
}


/*******************************************
   Callbacks
********************************************/

bool OurReceiver::OnUpdate(const its::Topic& Tpc,
    const TagValueMap& mValues
)
{
    static int count = 0;

    //std::string sValues = DumpTagValues(mValues);

    std::string sValues = "ciao";
    /* Displays 10 updates and exit */
    std::cout << "Upd on " << Tpc.Get() << ": " << sValues << std::endl;

    if (++count == MAX_MSG_COUNT)
    {
        SetEvent(eDone);
        return false;
    }

    return true;
}

bool OurReceiver::OnReply(const string& sRequestID,
    const TagValueMap& mValues,
    bool bLast
)
{
    /* We don't make any request */
    return true;
}

void OurReceiver::OnServerStatus(const string& sServer,
    ServerStatus eStatus,
    const string& sDescription
)
{
    std::cout << "Received status notify: \"" << eStatus << ": "
        << sDescription << std::endl;

    /* This is performed only on frist connection;
       On reconnection, the receiver re-subscribes any previously subscribed topic (that has
       received some data).
    */
    if (eStatus == STS_CONNECTED)
    {
        std::cout << "Subscribing all tags on topic TIT.TEST0.MKT" << std::endl;
        std::string sError;

        /* an empty tagset means "everything" */

        //if (!Subscribe(its::Topic("TIT.TEST0.MKT"), REQ_SNAP_REF,
        //    TagSet(), false, sError))
        //{
        //    std::cout << "Subscription failed: " << sError << std::endl;
        //    /* Make the program to terminate */
        //    SetEvent(eDone);
        //}
    }
}