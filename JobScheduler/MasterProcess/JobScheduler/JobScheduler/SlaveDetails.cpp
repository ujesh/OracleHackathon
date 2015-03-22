#include "SlaveDetails.h"
#include <algorithm>

SlaveDetails::SlaveDetails(int id, int maxTypeLimit, int maxCategoryLimit)
  : id(id)
{
  InitFileTypeLimits(maxTypeLimit);
  InitFileCategoryLimits(maxCategoryLimit);
}

void SlaveDetails::SetId(int id)
{
  id = id;
}

void SlaveDetails::SetMaxLimit(FileType::Type type, int maxLimit)
{
  fileTypeMaxLimitMap[type] = maxLimit;
}

void SlaveDetails::SetMaxLimit(FileCategory::Category category, int maxLimit)
{
  fileCategoryMaxLimitMap[category] = maxLimit;
}

void SlaveDetails::IncrementActiveCount(FileType::Type type)
{
  FileTypeToIntMap::iterator fileTypeIterator = fileTypeActiveCountMap.find(type);
  if(fileTypeIterator != fileTypeActiveCountMap.end())
  {
    fileTypeIterator->second++;
    FileType::Type fileType = fileTypeIterator->first;
    if(fileTypeIterator->second >= (fileTypeMaxLimitMap.find(fileType))->second)
    {
      FileTypeList::iterator fileTypeIterator = std::find(freeTypes.begin(), freeTypes.end(), fileType);
      if(fileTypeIterator != freeTypes.end())
        freeTypes.erase(fileTypeIterator);
    }
  }
}

void SlaveDetails::DecrementActiveCount(FileType::Type type)
{
  FileTypeToIntMap::iterator fileTypeIterator = fileTypeActiveCountMap.find(type);
  if(fileTypeIterator != fileTypeActiveCountMap.end())
  {
    fileTypeIterator->second--;
     FileType::Type fileType = fileTypeIterator->first;
     freeTypes.push_front(fileType);
  }
}

void SlaveDetails::IncrementActiveCount(FileCategory::Category category)
{
  FileCategoryToIntMap::iterator fileCategoryIterator = fileCategoryActiveCountMap.find(category);
  if(fileCategoryIterator != fileCategoryActiveCountMap.end())
  {
    fileCategoryIterator->second++;
    FileCategory::Category fileCategory = fileCategoryIterator->first;
    if(fileCategoryIterator->second >= (fileCategoryMaxLimitMap.find(fileCategory))->second)
    {
      FileCategoryList::iterator fileCategoryIterator = std::find(freeCategories.begin(), freeCategories.end(), fileCategory);
      if(fileCategoryIterator != freeCategories.end())
        freeCategories.erase(fileCategoryIterator);
    }
  }
}

void SlaveDetails::DecrementActiveCount(FileCategory::Category category)
{
  FileCategoryToIntMap::iterator fileCategoryIterator = fileCategoryActiveCountMap.find(category);
  if(fileCategoryIterator != fileCategoryActiveCountMap.end())
  {
    fileCategoryIterator->second--;
    FileCategory::Category fileCategory = fileCategoryIterator->first;
    freeCategories.push_front(fileCategory);
  }
}

int SlaveDetails::GetId()
{
  return id;
}

int SlaveDetails::GetMaxLimit(FileType::Type type)
{
  int maxLimit = -1;
  FileTypeToIntMap::iterator fileTypeIterator = fileTypeMaxLimitMap.find(type);
  if(fileTypeIterator != fileTypeMaxLimitMap.end())
    maxLimit = fileTypeIterator->second;

  return maxLimit;
}

int SlaveDetails::GetMaxLimit(FileCategory::Category category)
{
  int maxLimit = -1;
  FileCategoryToIntMap::iterator fileCategoryIterator = fileCategoryMaxLimitMap.find(category);
  if(fileCategoryIterator != fileCategoryMaxLimitMap.end())
    maxLimit = fileCategoryIterator->second;

  return maxLimit;
}

int SlaveDetails::GetActiveCount(FileType::Type type)
{
  int activeCount = -1;
  FileTypeToIntMap::iterator fileTypeIterator = fileTypeActiveCountMap.find(type);
  if(fileTypeIterator != fileTypeActiveCountMap.end())
    activeCount = fileTypeIterator->second;

  return activeCount;
}

int SlaveDetails::GetActiveCount(FileCategory::Category category)
{
  int activeCount = -1;
  FileCategoryToIntMap::iterator fileCategoryIterator = fileCategoryActiveCountMap.find(category);
  if(fileCategoryIterator != fileCategoryActiveCountMap.end())
    activeCount = fileCategoryIterator->second;

  return activeCount;
}

void SlaveDetails::InitFileTypeLimits(int commonLimit)
{
  fileTypeMaxLimitMap[FileType::BINARY] = commonLimit;
  fileTypeMaxLimitMap[FileType::TEXT] = commonLimit;
  fileTypeActiveCountMap[FileType::BINARY] = commonLimit;
  fileTypeActiveCountMap[FileType::TEXT] = commonLimit;
}

void SlaveDetails::InitFileCategoryLimits(int commonLimit)
{
  fileCategoryMaxLimitMap[FileCategory::DOCUMENT] = commonLimit;
  fileCategoryMaxLimitMap[FileCategory::IMAGE] = commonLimit;
  fileCategoryMaxLimitMap[FileCategory::MOVIE] = commonLimit;
  fileCategoryMaxLimitMap[FileCategory::REPORT] = commonLimit;

  fileCategoryActiveCountMap[FileCategory::DOCUMENT] = commonLimit;
  fileCategoryActiveCountMap[FileCategory::IMAGE] = commonLimit;
  fileCategoryActiveCountMap[FileCategory::MOVIE] = commonLimit;
  fileCategoryActiveCountMap[FileCategory::REPORT] = commonLimit;
}

void SlaveDetails::GetFreeTypes(FileTypeList &freeTypeList)
{
  freeTypeList = freeTypes;
}

void SlaveDetails::GetFreeCategories(FileCategoryList &freeCategories)
{
  freeCategories = freeCategories;
}

 //FileType::Type SlaveDetails::GetFreeType()
 //{
 //  FileTypeToIntMap::iterator fileTypeIterator = fileTypeActiveCountMap.begin();
 //  for(; fileTypeIterator != fileTypeActiveCountMap.end(); fileTypeIterator++)
 //  {
 //    if(fileTypeIterator->second < (fileTypeMaxLimitMap.find(fileTypeIterator->first))->second)
 //    {
 //      return fileTypeIterator->first;
 //    }
 //  }

 //  return FileType::NONE;
 //}
 //
 //FileCategory::Category SlaveDetails::GetFreeCategory()
 //{
 //  FileCategoryToIntMap::iterator fileCategoryIterator = fileCategoryActiveCountMap.begin();
 //  for(; fileCategoryIterator != fileCategoryActiveCountMap.end(); fileCategoryIterator++)
 //  {
 //    if(fileCategoryIterator->second < (fileCategoryMaxLimitMap.find(fileCategoryIterator->first))->second)
 //    {
 //      return fileCategoryIterator->first;
 //    }
 //  }

 //  return FileCategory::NONE;
 //}
