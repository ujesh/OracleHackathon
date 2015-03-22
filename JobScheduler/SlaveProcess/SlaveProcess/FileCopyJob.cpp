

#include "FileCopyJob.h"
#include <Windows.h>
//#include "ExceptionBase.h"

void FileCopyJob::Execute() {
	JobData*	lu_jobData = NULL;
	try
	{
	lu_jobData = GetJobData();
	//copy file
	std::cout<<"Copying file..."<<lu_jobData->getFilePath()<<std::endl;
	if(CopyFile( (LPCWSTR)lu_jobData->getFilePath().c_str(), (LPCWSTR)lu_jobData->getDestinationPath().c_str(), FALSE ) == false)
	{
		//FileCopyException lu_fileCopyException;
				std::cout<<"Error in Copying file..."<<lu_jobData->getFilePath()<<std::endl;
		ExceptionBase	*lup_jobException = new JobException(__FILE__,__FUNCTION__,__LINE__,"Exception Raised in Job Execute");
		throw lup_jobException;
		//throw lu_fileCopyException;
	}

	}
	catch(ExceptionBase *lup_jobException)
	//catch(FileCopyException lu_exception)
	{
		throw lup_jobException;
		
	}

}


