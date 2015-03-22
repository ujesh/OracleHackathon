

#if !defined(_SLAVEDATA_H)
#define _SLAVEDATA_H

#include "FileSpecifications.h"
#include "JobData.h"
#include "FileCopyJob.h"
#include <map>
#include <queue>

class SlaveData {
public:
	int GetActiveJobCount();
	int GetID();
	void SetID(int );
	void SetActiveJobCount(int);
	SlaveData();
	~SlaveData();
	
	/*
	int GetActiveJobCount(FileCategory::Category );
	int GetActiveJobCount(FileType::Type);
	int GetMaxJobCount(FileCategory::Category );
	int GetMaxJobCount(FileType::Type);
	*/
private:
	int Id;
	int ActiveJobsCount;
	/*
	std::map<FileCategory::Category, int> ActiveCategoryCountMap;
	std::map<FileType::Type, int> ActiveTypeCountMap;
	std::map<FileCategory::Category, int> MaxCategoryCountMap;
	std::map<FileType::Type, int> MaxTypeCountMap;
	*/
};

#endif  //_SLAVEDATA_H
