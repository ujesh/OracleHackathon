#include "Job.h"


JobException::JobException(const char * file,const char * function,
                                                              int line,
                                                              const std::string& what)
 :ExceptionBase("JobException", file, function, line, what)
{
}

JobData* Job::GetJobData()
{
	return mup_jobData;
}
Job::Job()
{
	mup_jobData = NULL;
}
Job::~Job()
{
	if(mup_jobData != NULL)
	{
		delete mup_jobData;
	}
}
void Job::SetJobData(JobData* aup_jobData)
{
	if(aup_jobData != NULL)
	{
		mup_jobData = aup_jobData;
	}
}