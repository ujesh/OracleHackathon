
#if !defined(_SLAVE_H)
#define _SLAVE_H
#include "MessagePacket.h"
#include "JobData.h"
#include "SlaveData.h"
#include "Job.h"
#include "IPCManager.h"
#include "CommonDefinitions.h"
//#include "ExceptionBase.h"
#include <Windows.h>

#include <queue>

struct ThreadArgs;

class Slave {
public:
	std::queue<std::string> JobDataQueue;
	std::queue<Job*>	JobWorkerQueue;
	Slave();
	~Slave();
	void StartThreads();
	static void JobDelegateThread(Slave *);
	static void JobProcessThread(ThreadArgs  *);
	SlaveData* GetSlaveData();
	IPCManager* GetIPCManager();
private:
	SlaveData * slavedata;
	IPCManager * ipc;
	HANDLE		mDelegateThreadStartedEvent;
	HANDLE		mDelegateThreadStopEvent;
	HANDLE		m_DelegateThreadHandle;

	CRITICAL_SECTION	lu_syncLockThreadPool;
	CRITICAL_SECTION	lu_syncLockDelegateThread;


	/*
	HANDLE		mProcessThreadStartedEvent;
	HANDLE		mProcessThreadStopEvent;
	HANDLE		mProcessThreadHandle;
	*/
	HANDLE		mProcessThreadStartedEvent[NUM_WORKER_THREADS];
	HANDLE		mProcessThreadStopEvent[NUM_WORKER_THREADS];
	HANDLE		mProcessThreadHandle[NUM_WORKER_THREADS];
	
};
struct ThreadArgs
{
	Slave	*mup_slave;
	int		mi_threadIndex;
};

class  SlaveException : public ExceptionBase
  {
  public:
	    SlaveException(const char * file, const char * function,
                                    int line, const std::string& what);
};
#endif  //_SLAVE_H
