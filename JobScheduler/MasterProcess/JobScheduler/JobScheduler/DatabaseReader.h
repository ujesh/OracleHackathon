#if !defined(_DATABASEREADER_H)
#define _DATABASEREADER_H
#include "JobDataReader.h"
#include "mysql.h"
#include "JobData.h"
#include "queue"
#include "JobType.h"
#include "ExceptionBase.h"
#include "FileSpecifications.h"
using namespace std;

class DatabaseReader : public JobDataReader 
{
public:
    DatabaseReader();
    ~DatabaseReader();
	void Read( queue< JobData* >& JobQueue);
    void initialize();
private:
    MYSQL *mysql;
    MYSQL_RES *result;
    MYSQL_ROW row;
};

class  DatabaseReaderException : public ExceptionBase
  {
  public:
	    DatabaseReaderException(const char * file, const char * function,
                                    int line, const std::string& what);
};

#endif  //_DATABASEREADER_H
