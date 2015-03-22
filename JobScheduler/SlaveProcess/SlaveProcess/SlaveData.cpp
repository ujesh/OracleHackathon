
#include "SlaveData.h"
#include "FileSpecifications.h"
#include "Locks.h"

SlaveData::SlaveData()
{
	ActiveJobsCount = 0;
	Id=999;
}
SlaveData::~SlaveData()
{

}

int SlaveData::GetActiveJobCount() {
	return ActiveJobsCount;
}
int SlaveData::GetID()
{
	return Id;
}
 void SlaveData::SetID(int ai_id)
 {
	 Id = ai_id;
 }
 void SlaveData::SetActiveJobCount(int ai_activeJobCount)
 {
	 //Lock mutex
	 ActiveJobsCount = ai_activeJobCount;
	 //Unlock mutex
 }
/*
int SlaveData::GetActiveJobCount(FileCategory::Category au_fileCategory) {
	if(ActiveCategoryCountMap.find(au_fileCategory) != ActiveCategoryCountMap.end())
	{
		return ActiveCategoryCountMap[au_fileCategory];
	}
	return 0;
}

int SlaveData::GetActiveJobCount(FileType::Type au_fileType) {
	if(ActiveTypeCountMap.find(au_fileType) != ActiveTypeCountMap.end())
	{
		return ActiveTypeCountMap[au_fileType];
	}
	return 0;
}

int SlaveData::GetMaxJobCount(FileCategory::Category au_jobCategory) {
	if(MaxCategoryCountMap.find(au_jobCategory) != MaxCategoryCountMap.end())
	{
		return MaxCategoryCountMap[au_jobCategory];
	}
	return 0;
}

int SlaveData::GetMaxJobCount(FileType::Type au_fileType) {
		if(MaxTypeCountMap.find(au_fileType) != MaxTypeCountMap.end())
	{
		return MaxTypeCountMap[au_fileType];
	}
	return 0;
}

*/