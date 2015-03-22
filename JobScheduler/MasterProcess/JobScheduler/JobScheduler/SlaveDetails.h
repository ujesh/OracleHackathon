#if !defined(_SLAVEDETAILS_H)
#define _SLAVEDETAILS_H

#include <map>
#include <list>
#include "FileSpecifications.h"

typedef std::map<FileType::Type, int> FileTypeToIntMap;
typedef std::map<FileCategory::Category, int> FileCategoryToIntMap;
typedef std::list<FileType::Type> FileTypeList;
typedef std::list<FileCategory::Category> FileCategoryList;

class SlaveDetails
{
public: 
  SlaveDetails(int id, int maxTypeLimit = 0, int maxCategoryLimit = 0);
  void SetId(int id);
  void SetMaxLimit(FileType::Type type, int maxLimit);
  void SetMaxLimit(FileCategory::Category category, int maxLimit);
  void IncrementActiveCount(FileType::Type type);
  void IncrementActiveCount(FileCategory::Category category);
  void DecrementActiveCount(FileCategory::Category category);
  void DecrementActiveCount(FileType::Type type);

  int GetId();
  int GetMaxLimit(FileType::Type type);
  int GetMaxLimit(FileCategory::Category category);
  int GetActiveCount(FileType::Type type);
  int GetActiveCount(FileCategory::Category category);

  void GetFreeTypes(FileTypeList &freeTypeList);
  void GetFreeCategories(FileCategoryList &freeCategories);

private: //Functions
  void InitFileTypeLimits(int commonLimit);
  void InitFileCategoryLimits(int commonLimit);

private: //Variables
  int id;
  FileTypeToIntMap fileTypeMaxLimitMap;
  FileCategoryToIntMap fileCategoryMaxLimitMap;
  FileTypeToIntMap fileTypeActiveCountMap;
  FileCategoryToIntMap fileCategoryActiveCountMap;
  FileTypeList freeTypes; 
  FileCategoryList freeCategories;

};

#endif
