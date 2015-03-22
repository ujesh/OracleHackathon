/**
 * .
 *
 * JobData.h. - header of the class JobData
 *
 * @author :    SANOJ THOMAS
 * @info   :    Initial version
 */

#ifndef JOBDATA_H
#define JOBDATA_H

#include "string"
using namespace std;
#include "JobType.h"
#include "FileSpecifications.h"

class JobData {
public:
    JobType::Type jobType;

private:
    string FilePath;
	FileType::Type FileFtype;
	FileCategory::Category FileCategory;
	int FileId;
	string DestinationPath;
public:
    JobData();
    JobData( int nFileId_i, string strFilePath_i, FileType::Type au_fileType, FileCategory::Category au_fileCategory, string strDestinationPath_i, JobType::Type jobType_E);
    ~JobData();
    void setFileId(int nFileId_i);
    void setFilePath(string strFilePath_i) ;
    void setFileType( FileType::Type strFileFtype_i );
    void setFileCategory(FileCategory::Category) ;
    void setDestinationPath( string strDestinationPath_i ) ;
    void setJobType( JobType::Type jobType_E);
    int getFileId() const;
    string getFilePath() const;
    FileType::Type  getFileType() const;
    FileCategory::Category getFileCategory() const;
    string getDestinationPath() const;
    JobType::Type getJobType() const;
};



#endif  //_JOBDATA_H
