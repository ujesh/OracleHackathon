

#include "Locks.h"
//#include "ExceptionBase.h"

/*! \brief IPE Toolkit project namespace. */


CriticalSectionLock::CriticalSectionLock(LPCRITICAL_SECTION criticalSection)
    : mCriticalSection(criticalSection)
{
  if (criticalSection == NULL){
    /*throw InvalidArgumentException(CURRENT_FILE_LOCATION,
                                   "criticalSection is NULL");*/
				//throw from here
  }
  EnterCriticalSection(mCriticalSection);
}

CriticalSectionLock::~CriticalSectionLock()
{
  Unlock();
}

void CriticalSectionLock::Unlock()
{
  LeaveCriticalSection(mCriticalSection);
}

#if USE_MUTEX_LOCK

MutexLock::MutexLock(HANDLE h) : mMutex(h)
{
  WaitForSingleObject(mMutex, INFINITE);
}

MutexLockMLock::~MutexLock()
{
  Unlock();
}

MutexLockMLock::Unlock()
{
  ReleaseMutex(mMutex);
}

#endif // USE_MUTEX_LOCK

