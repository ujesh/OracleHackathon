//  
//
//  @ File Name : JobData.cpp
//  @ Date : 3/21/2015
//  @ Author : SANOJ THOMAS
//
#include "JobData.h"
    
    JobData::JobData()
    {
    }
    JobData::JobData( int nFileId_i, string strFilePath_i, FileType::Type au_fileType, FileCategory::Category au_fileCategory, string strDestinationPath_i, JobType::Type jobType_E)
    {
        FileId = nFileId_i;
        FilePath = strFilePath_i;
        FileFtype = au_fileType;
        FileCategory = au_fileCategory;
        DestinationPath = strDestinationPath_i;
        jobType = jobType_E;
    }
    JobData::~JobData()
    {
    }
    void JobData::setFileId(int nFileId_i) 
    {
        FileId = nFileId_i;
    }
    void JobData::setFilePath(string strFilePath_i) 
    {
        FilePath = strFilePath_i;
    }
    void JobData::setFileType( FileType::Type lu_fileType) 
    {
        FileFtype = lu_fileType;
    }
    void JobData::setFileCategory(FileCategory::Category au_fileCategory) 
    {
        FileCategory = au_fileCategory;
    }
    void JobData::setDestinationPath( string strDestinationPath_i ) 
    {
        DestinationPath = strDestinationPath_i;
    }
    void JobData::setJobType( JobType::Type jobType_E) 
    {
        jobType = jobType_E;
    }
    int JobData::getFileId() const
    {
        return FileId;
    }
    string JobData::getFilePath() const
    {
        return FilePath;
    }
    FileType::Type JobData::getFileType() const
    {
        return FileFtype;
    }
    FileCategory::Category JobData::getFileCategory() const
    {
        return FileCategory;
    }
    string JobData::getDestinationPath() const
    {
        return DestinationPath;
    }
    JobType::Type JobData::getJobType() const
    {
        return jobType;
    }