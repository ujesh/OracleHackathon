
#pragma once


#include <Windows.h>


// \cond
// MutexLock is example code for how to construct another lock type. Since the
// lock/unlock operation are different for different construct, a template
// class cannot be created for each kind of lock (Semaphore, Critical Section,
// Mutex, etc.)
// IPETK++ does not currently use any mutexes so the code is not compiled.
#define USE_MUTEX_LOCK 0
// \endcond

/*! \brief IPE Toolkit project namespace. */

  /*! \addtogroup tk2-infrastructure Infrastructure
   *  @{
   */

  /*! \brief Manages the acquisition and release of a CRITICAL_SECTION.
   *
   * It does not create or destroy the CRITICAL_SECTION, but merely manages
   * locking and unlocking of the resource protected by it.
   */
  class CriticalSectionLock
  {
  public:
    /*! \brief Constructor.
     *
     *  This constructor takes a reference to the CRITICAL_SECTION that will be
     *  managed by this class. It saves the reference and makes a call to
     *  acquire the resource.
     *
     * \param criticalSection A reference to a CRITICAL_SECTION type.
     *
     * \throws InvalidArgumentException if criticalSection is NULL.
     */
    CriticalSectionLock(LPCRITICAL_SECTION criticalSection);

    /*!
    * \brief Destructor.
    *
    * Release the CRITICAL_SECTION managed by this class.
    */
    ~CriticalSectionLock();

    /*! Release the CITICAL_SECTION. */
    void Unlock();
  private:
    /*! Saved reference to a CRITICAL_SECTION object. */
    LPCRITICAL_SECTION mCriticalSection;
    /*! Declared private to prevent instantiation on the heap. */
    static void * operator new(size_t);
    /*! Declared private to prevent instantiation on the heap. */
    static void operator delete(void *);
  };

  #if USE_MUTEX_LOCK
  /*! Mutex Lock
   * This user-defined type manages the acquisition and release of a mutex.
   * It does not create or destroy the mutex. It only manages locking and
   * unlocking of the resource it protects.
   */
  class MutexLock
  {
  public:
    MutexLock(HANDLE);
    ~MutexLock();
    Unlock();
  private:
    HANDLE mMutex;
  };
  #endif
    /*! @} */

