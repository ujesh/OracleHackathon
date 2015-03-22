#if !defined(_JOBDATAREADER_H)
#define _JOBDATAREADER_H

#include <list>
#include "JobData.h"

class JobDataReader {
public:
	virtual void Read(std::list<JobData *>&) = 0;
};

#endif  //_JOBDATAREADER_H
