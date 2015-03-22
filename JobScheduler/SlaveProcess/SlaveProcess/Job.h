

#if !defined(_JOB_H)
#define _JOB_H
#include "JobData.h"
#include "ExceptionBase.h"

class Job {
public:
	virtual void Execute() = 0;
	JobData* GetJobData();
	void SetJobData(JobData*);
	Job();
	~Job();
protected:
	JobData	*mup_jobData;
};
class  JobException : public ExceptionBase
  {
  public:
	    JobException(const char * file, const char * function,
                                    int line, const std::string& what);
};
#endif  //_JOB_H
