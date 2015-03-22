#include "Master.h"
#include <Windows.h>
#include <sstream>
#include "CommonDefinitions.h"
#include "MessagePacket.h"
#include <algorithm>
#include <string>

const std::string SLAVE_PROCESS_NAME = "SlaveProcess.exe";
const int DEFAULT_ID = 0;
const int MAX_LIMIT_PER_CATEGORY = 5;
const int MAX_LIMIT_PER_TYPE = 5;

Master::Master(JobDataReader *reader, int numberOfSlaves)
: reader(reader), maxSlaves(numberOfSlaves), jobShcedulerThreadHandle(NULL)
{
  GenerateSlaves();
  jobSchedulerThreadStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  if(NULL == jobSchedulerThreadStartEvent)
  {
    //Error. throw.
  }

  jobSchedulerThreadStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  if(NULL == jobSchedulerThreadStopEvent)
  {
    CloseHandle(jobSchedulerThreadStartEvent);
    //Error. throw.
  }
}

Master::~Master()
{
  if(jobShcedulerThreadHandle)
  {
    SetEvent(jobSchedulerThreadStopEvent);
    if(WAIT_TIMEOUT, WaitForSingleObject(jobShcedulerThreadHandle, THREAD_WAIT_TIMEOUT))
    {
      //Log error
    }
    CloseHandle(jobSchedulerThreadStartEvent);
    CloseHandle(jobSchedulerThreadStopEvent);
    CloseHandle(jobShcedulerThreadHandle);
  }
}

void Master::ReadInput() 
{
  reader->Read(jobDataQueue);
}

void Master::GenerateSlaves() 
{
  int slaveIndex = 0;
  for(; slaveIndex < maxSlaves; slaveIndex++)
  {
    std::ostringstream slaveIndexString;
    slaveIndexString << slaveIndex;

    STARTUPINFOA startupInfo;
    PROCESS_INFORMATION processInfo;

    ZeroMemory( &startupInfo, sizeof(startupInfo) );
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory( &processInfo, sizeof(processInfo) );
    
    CreateProcessA(SLAVE_PROCESS_NAME.c_str(), 
                  (LPSTR)slaveIndexString.str().c_str(),
                  NULL,
                  NULL,
                  FALSE,
                  0,
                  NULL,
                  NULL,
                  &startupInfo,
                  &processInfo);
 
    SlaveDetails *slaveDetail = new SlaveDetails(slaveIndex, MAX_LIMIT_PER_TYPE, MAX_LIMIT_PER_CATEGORY);
    slaveIdSlaveDetailsMap[slaveIndex] = slaveDetail;
  }
}

void Master::JobSchedulerThread(Master *master) 
{
  SetEvent(master->jobSchedulerThreadStartEvent);
  while(WaitForSingleObject(master->jobSchedulerThreadStopEvent, 0) != WAIT_TIMEOUT)
  {
    if(master->jobDataQueue.size() > 0)
    {
      IntToSlaveDetailMap::iterator slaveIterator;
      for(slaveIterator = master->slaveIdSlaveDetailsMap.begin(); 
          slaveIterator!= master->slaveIdSlaveDetailsMap.end();
          slaveIterator++)
      {
        FileTypeList freeTypes;
        slaveIterator->second->GetFreeTypes(freeTypes);
        FileTypeList::iterator typesIterator = freeTypes.begin();
        for(; typesIterator != freeTypes.end(); typesIterator++)
        {
          FileType::Type fileType = (*typesIterator);
          JobDataQueue::iterator jobDataIterator = std::find_if(master->jobDataQueue.begin(), 
                                                                master->jobDataQueue.end(), 
                                                                [fileType](JobData *jobData){return (fileType == jobData->getFileType());});
          if(jobDataIterator != master->jobDataQueue.end())
          {
            MessagePacket message(MessagePacket::JOBDATA, DEFAULT_ID, (*jobDataIterator)->getFileId(), master->GenerateMessageData(*jobDataIterator));
            master->ipc->Send(slaveIterator->first, message.GetMessagePacket());
            master->jobDataQueue.erase(jobDataIterator);
          }
        }
      }
    }
  }
}

std::string Master::GenerateMessageData(JobData *jobData)
{
  std::ostringstream messageDataStream;
  messageDataStream << jobData->getFileId() << "|"
                    << jobData->getFilePath() << "|" 
                    << jobData->getFileType() << "|"
                    << jobData->getFileCategory() << "|"
                    << jobData->getDestinationPath();

  std::string messageData = messageDataStream.str();
  return messageData;
}

