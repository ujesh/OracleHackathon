#if !defined(_MASTER_H)
#define _MASTER_H

#include "JobDataReader.h"
#include "JobData.h"
#include "IPCManager.h"
#include <list>
#include <Windows.h>
#include <map>
#include "FileSpecifications.h"
#include "SlaveDetails.h"

typedef std::list<JobData *> JobDataQueue;
typedef std::map<int, SlaveDetails*> IntToSlaveDetailMap;

class Master {
public:
  Master(JobDataReader *reader, int numberOfSlaves);
  ~Master();
  void ReadInput();
  static void JobSchedulerThread(Master *master);
private: //Functions
  void GenerateSlaves();
  void StartThread();
  std::string GenerateMessageData(JobData *jobData);
private: //Variables
  JobDataReader *reader;
  JobDataQueue jobDataQueue;
  IPCManager *ipc;
  int maxSlaves;
  HANDLE jobShcedulerThreadHandle;
  HANDLE jobSchedulerThreadStartEvent;
  HANDLE jobSchedulerThreadStopEvent;
  IntToSlaveDetailMap slaveIdSlaveDetailsMap;
};

#endif  //_MASTER_H
