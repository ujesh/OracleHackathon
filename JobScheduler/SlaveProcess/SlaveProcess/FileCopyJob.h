

#if !defined(_FILECOPYJOB_H)
#define _FILECOPYJOB_H

#include "Job.h"
#include <iostream>

class FileCopyJob : public Job {
public:
	void Execute();
};

#endif  //_FILECOPYJOB_H
