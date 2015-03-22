
#include "Slave.h"
#include "Locks.h"


SlaveException::SlaveException(const char * file,const char * function,
                                                              int line,
                                                              const std::string& what)
 :ExceptionBase("SlaveException", file, function, line, what)
{
}

void toUpper(basic_string<char>& s) {
   for (basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = toupper(*p); // toupper is for char
   }
}

Slave::Slave() {

mDelegateThreadStartedEvent = CreateEvent(NULL,false,false,NULL);
mDelegateThreadStopEvent = CreateEvent(NULL,false,false,NULL);
/*
mProcessThreadStartedEvent = CreateEvent(NULL,false,false,NULL);
mProcessThreadStopEvent = CreateEvent(NULL,false,false,NULL);
*/
InitializeCriticalSection(&lu_syncLockThreadPool);
InitializeCriticalSection(&lu_syncLockDelegateThread);


for(int li_index = 0;li_index<NUM_WORKER_THREADS;li_index++)
{
	mProcessThreadStartedEvent[li_index] = CreateEvent(NULL,false,false,NULL);
	mProcessThreadStopEvent[li_index] = CreateEvent(NULL,false,false,NULL);
}


}
Slave::~Slave() {



SetEvent(mDelegateThreadStopEvent);
if(WaitForSingleObject(m_DelegateThreadHandle,THREAD_WAIT_TIMEOUT) == WAIT_TIMEOUT)
{
	std::cout<<"Error in stopping thread .."<<std::endl;
}

CloseHandle(mDelegateThreadStartedEvent);
CloseHandle(mDelegateThreadStopEvent);
CloseHandle(m_DelegateThreadHandle);

for(int li_index = 0;li_index<NUM_WORKER_THREADS;li_index++)
{
	SetEvent(mProcessThreadStopEvent[li_index]);
	if(WaitForSingleObject(mProcessThreadHandle[li_index],THREAD_WAIT_TIMEOUT) == WAIT_TIMEOUT)
	{
		std::cout<<"Error in stopping thread .."<<li_index+1<<std::endl;
	}

	CloseHandle(mProcessThreadStartedEvent[li_index]);
	CloseHandle(mProcessThreadStopEvent[li_index]);
	CloseHandle(mProcessThreadHandle[li_index]);
}

DeleteCriticalSection(&lu_syncLockThreadPool);
DeleteCriticalSection(&lu_syncLockDelegateThread);


}


SlaveData* Slave::GetSlaveData()
{
	return slavedata;
}
IPCManager* Slave::GetIPCManager()
{
	return ipc;
}

void Slave::JobProcessThread(ThreadArgs * aup_threadArgs) {
	
	try
	{
	Slave		*aup_slave = NULL;
	
	aup_slave = aup_threadArgs->mup_slave;
	int li_threadIndex = aup_threadArgs->mi_threadIndex;

	SetEvent(aup_slave->mProcessThreadStartedEvent[li_threadIndex]);
	std::cout<<"Started Job Processing Thread..."<<li_threadIndex+1<<std::endl;
	while (WaitForSingleObject(aup_slave->mProcessThreadStopEvent[li_threadIndex], 0) != WAIT_OBJECT_0)
	{
		
		Sleep(1000);
		Job	*lup_job = NULL;
		CriticalSectionLock lu_ProcessThreadlock(&aup_slave->lu_syncLockThreadPool);
		bool	lb_dataPresent = false;
		if(aup_slave->JobWorkerQueue.size())
		{
			
			
			//Lock Mutex
				//pick data from queue
				lup_job = aup_slave->JobWorkerQueue.front();
				aup_slave->JobWorkerQueue.pop();
				lb_dataPresent = true;
			//Unlock muted
		}
		lu_ProcessThreadlock.Unlock();

		if(lb_dataPresent)
		{
			string ls_executeStatus;
			try
			{
				//call Execute task
				lup_job->Execute();
				ls_executeStatus = "SUCCESS";
			}
			catch(JobException *lup_jobException)
			{
				std::cout<<"JobException raised in Process Job Thread"<<lup_jobException->what()<<std::endl;
				ls_executeStatus = "FAILED";
			}
			catch(...)
			{
				ls_executeStatus = "FAILED";
			}
			try
			{
				//create Response message
				MessagePacket	lu_message(MessagePacket::JOBSTATUS ,aup_slave->GetSlaveData()->GetID() ,lup_job->GetJobData()->getFileId() , ls_executeStatus);
				std::string	ls_message;
				ls_message = lu_message.GetMessagePacket();
				// Send it to master
				aup_slave->GetIPCManager()->Send(MASTER_PROCESS_ID,ls_message);
				delete lup_job;
			
				aup_slave->GetSlaveData()->SetActiveJobCount(aup_slave->GetSlaveData()->GetActiveJobCount() - 1);
			} catch(ExceptionBase *lu_slaveException)
			{
					std::cout<<"Exception raised while sending response back to master process..."<<lu_slaveException->what()<<endl;
			}
		}
	}
	} catch(...)
	{
		std::cout<<"Exception Raised in JobProcessThread"<<endl;
	}
}


void Slave::JobDelegateThread(Slave *aup_slave) {
	SetEvent(aup_slave->mDelegateThreadStartedEvent);
	std::cout<<"Started Job Delegation Thread..."<<std::endl;
	try
	{
	while (WaitForSingleObject(aup_slave->mDelegateThreadStopEvent, 0) != WAIT_OBJECT_0)
	{
		int li_queueIndex = 0;
		CriticalSectionLock lu_DelegateThreadlock(&aup_slave->lu_syncLockDelegateThread);
		bool lb_dataPresent = false;
		string	ls_jobDataOrg;
		string ls_jobData;
		if(aup_slave->JobDataQueue.size())
		{
			if(li_queueIndex == NUM_WORKER_THREADS) {
				li_queueIndex = 0;
			}
			ls_jobDataOrg = aup_slave->JobDataQueue.front();
			ls_jobData = ls_jobDataOrg;

			aup_slave->JobDataQueue.pop();
			lb_dataPresent = true;
		}
		lu_DelegateThreadlock.Unlock();
		if(lb_dataPresent)
		{
			
			bool lb_validData = true;
			std::string ls_fileId, ls_filePath, ls_fileType, ls_fileCategory, ls_destinationPath;
			std::string ls_delimiter = "|";
			try
			{
				//Make Job object

			
				size_t pos = 0;
				std::string ls_token;
				int li_index1 = 0;
				while ((pos = ls_jobData.find(ls_delimiter)) != std::string::npos) {
					ls_token = ls_jobData.substr(0, pos);
					ls_jobData.erase(0, pos + ls_delimiter.length());
					if(li_index1 == 2)
						ls_fileId = ls_token.c_str();
					else if(li_index1 == 3)
						ls_filePath = ls_token;
					else if(li_index1 == 4)
						ls_fileType = ls_token;
					else if(li_index1 == 5)
						ls_fileCategory = ls_token;
					li_index1++;
				}
				ls_destinationPath = ls_jobData;
				if(li_index1<6)
				{
					lb_validData = false;
				}
			}
			catch(...)
			{
				lb_validData = false;
			}
			if(lb_validData)
			{
				try
				{
				JobData	*lup_jobData = new JobData;
				lup_jobData->setFileId( atoi(ls_fileId.c_str()));
				lup_jobData->setFilePath( ls_filePath) ;
				FileType::Type lu_fileType;
				toUpper(ls_fileType);
				if(ls_fileType == "TEXT")
				{
					lu_fileType = FileType::TEXT;
				}
				else
				{
					lu_fileType = FileType::BINARY;
				}
				lup_jobData->setFileType( lu_fileType );
				toUpper(ls_fileCategory);
				FileCategory::Category lu_fileCategory;

				if(ls_fileCategory == "DOCUMENT")
				{
					lu_fileCategory = FileCategory::DOCUMENT;
				}
				else if(ls_fileCategory == "IMAGE")
				{
					lu_fileCategory = FileCategory::IMAGE;
				}
				else if(ls_fileCategory == "MOVIE")
				{
					lu_fileCategory = FileCategory::MOVIE;
				}
				else
				{
					lu_fileCategory = FileCategory::REPORT;
				}

				lup_jobData->setFileCategory(lu_fileCategory) ;
				lup_jobData->setDestinationPath( ls_destinationPath ) ;
				lup_jobData->setJobType( JobType::COPY_FILE);

				Job	*lup_job = NULL;
				lup_job = new FileCopyJob;
				lup_job->SetJobData(lup_jobData);

				//Lock Mutex
				CriticalSectionLock lu_ProcessThreadlock(&aup_slave->lu_syncLockThreadPool);
				//push to worker thread queue
				aup_slave->JobWorkerQueue.push(lup_job);
				//Unlock Mutex
				lu_ProcessThreadlock.Unlock();
				aup_slave->GetSlaveData()->SetActiveJobCount(aup_slave->GetSlaveData()->GetActiveJobCount() + 1);
				}
				catch(ExceptionBase *lup_slaveException)
				{
					std::cout<<"Exception raised in Deletation thread.."<<lup_slaveException->what()<<std::endl;
				}
				catch(...)
				{
					std::cout<<"Unknown Exception raised in Deletation thread.."<<std::endl;
				}
			}
		} else {
			std::cout<<"No jobs present in the queue"<<std::endl;
			//Send Message to Master Process
			//create Response message
			MessagePacket	lu_message(MessagePacket::SLAVEIDLE ,aup_slave->GetSlaveData()->GetID() ,0 , "");
			std::string	ls_message;
			ls_message = lu_message.GetMessagePacket();
			// Send it to master
			aup_slave->GetIPCManager()->Send(MASTER_PROCESS_ID,ls_message);
			Sleep(10000);
		}
		Sleep(100);
	}
	} catch(ExceptionBase *lup_slaveException)
	{
		std::cout<<"Exception raised in Deletation thread.."<<lup_slaveException->what()<<std::endl;
	}
}

void Slave::StartThreads()
{
//  DBGLOG(Logger::LVL_DEBUG, "%s, %s, %d", CURRENT_FILE_LOCATION);
	std::cout<<"Starting Delegation Thread..."<<std::endl;
	try
	{
  ResetEvent(mDelegateThreadStartedEvent);
  ResetEvent(mDelegateThreadStopEvent);
  //Start the output thread
  DWORD threadId;
  m_DelegateThreadHandle = CreateThread(
          NULL,
          0,
          (LPTHREAD_START_ROUTINE)Slave::JobDelegateThread,
          this,
          0,
          &threadId);
  if (m_DelegateThreadHandle == NULL) 
  {
    //throw OSObjectCreationFailedException(CURRENT_FILE_LOCATION,
                                      //"CreateThread: mUDPListenerThreadHandle");
  }
  WaitForSingleObject(mDelegateThreadStartedEvent,
                      THREAD_WAIT_TIMEOUT);

  std::cout<<"Starting Job Process Thread..."<<std::endl;

  for(int li_index2 = 0; li_index2< NUM_WORKER_THREADS;li_index2++)
  {
	  ThreadArgs	lu_threadArgs;
	  lu_threadArgs.mi_threadIndex = li_index2;
	  lu_threadArgs.mup_slave = this;
  	  

	  ResetEvent(mProcessThreadStartedEvent[li_index2]);
	  ResetEvent(mProcessThreadStopEvent[li_index2]);
	  //Start the output thread
	  DWORD threadId1;
	  mProcessThreadHandle[li_index2] = CreateThread(
			  NULL,
			  0,
			  (LPTHREAD_START_ROUTINE)Slave::JobProcessThread,
			  &lu_threadArgs,
			  0,
			  &threadId1);
	  if (mProcessThreadHandle[li_index2] == NULL) {
		//throw OSObjectCreationFailedException(CURRENT_FILE_LOCATION,
										  //"CreateThread: mUDPListenerThreadHandle");
	  }
	  WaitForSingleObject(mProcessThreadHandle[li_index2],
						  THREAD_WAIT_TIMEOUT);

  }
	} catch(ExceptionBase *lup_slaveException)
	{
		throw lup_slaveException;
	}
	catch(...)
	{
		ExceptionBase	*lup_slaveException = new SlaveException(__FILE__,__FUNCTION__,__LINE__,"Exception Raised in StartThreads");
		throw lup_slaveException;
	}

}


