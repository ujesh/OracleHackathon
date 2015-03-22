//  
//  @ Project : Untitled
//  @ File Name : DatabaseReader.cpp Purpose : To read data from database
//  @ Date : 3/21/2015
//  @ Author : SANOJ THOMAS
// 
//
#include "stdafx.h"
#include "DatabaseReader.h"


DatabaseReaderException::DatabaseReaderException(const char * file,const char * function,
                                                              int line,
                                                              const std::string& what)
 :ExceptionBase("DatabaseReaderException", file, function, line, what)
{
}

/**
 * To Initialize the mysql database
 * 
 * @param       Nil
 * @return      void
 * @exception   Nil
 * @see         Nil
 * @since       1.0
 */
DatabaseReader::DatabaseReader()
{
    initialize();
}
DatabaseReader::~DatabaseReader()
{
    mysql_close( mysql );
}

/**
 * To Initialize the mysql database
 * 
 * @param       Nil
 * @return      void
 * @exception   Nil
 * @see         Nil
 * @since       1.0
 */
void DatabaseReader::initialize()
{
    try
    {
        int nPort = 3306;
        mysql = mysql_init(0);
        mysql_real_connect( mysql, "localhost", "root", "@mottachi1", "job_scheduler", nPort, NULL, CLIENT_MULTI_RESULTS );
        mysql_set_server_option(mysql,  MYSQL_OPTION_MULTI_STATEMENTS_OFF );
    }
    catch(...)
    {
    }
}


/**
 * To read details from the database
 * 
 * @param       Nil
 * @return      void
 * @exception   Nil
 * @see         Nil
 * @since       1.0
 */
void DatabaseReader::Read(queue< JobData* >& JobQueue)
{
    JobData* ptrJobData;
    int nColumn = 0;
    int index = 1;
    int num_fields;
    string UpdateQuerry;
    try
    {
        mysql_query(mysql, "SELECT * FROM File_Details where File_Status = 0 limit 100");
        result = mysql_store_result(mysql);
        if(result)
        {
            num_fields = mysql_num_fields(result);
            while(row = mysql_fetch_row(result))
            {
                string ch;
                ptrJobData = new JobData;
                nColumn = 0;
                int nID;
                string strId;
                string strFilePath;
                string strFileType;
                string strFileCategory;
                string strDestinationPath;
                //iterate for reading data from database
                for(int nIndex = 0;nIndex < num_fields;nIndex++)
                {             
                    switch(nIndex)
                    {
                    case 0:
                            strId = row[nIndex];
                            nID = atoi( strId.c_str() );
                            break;
                    case 1:
                            strFilePath = row[nIndex];                     
                            break;
                    case 2:
                            strFileType = row[nIndex];                       
                            break;
                    case 3:
                            strFileCategory = row[nIndex];                       
                            break;
                    case 5:
                            strDestinationPath = row[nIndex];                       
                            break;
                    default:
                            break;
                    }
                    
                }
                FileType::Type myType
                if( strFileType == "text")
                {
                    myType = FileType::TEXT;
                }
                else if(strFileType == "binary")
                {
                    myType = FileType::BINARY;
                }
                FileCategory::Category lu_fileCategory;
                if( strFileCategory == "DOCUMENT")
                {
                    lu_fileCategory = FileCategory::DOCUMENT;
                }
                else if(strFileCategory == "IMAGE")
                {
                    lu_fileCategory = FileCategory::IMAGE;
                }
                else if(strFileCategory == "MOVIE")
                {
                    lu_fileCategory = FileCategory::MOVIE;
                }
                else if(strFileCategory == "REPORT")
                {
                    lu_fileCategory = FileCategory::REPORT;
                }
                JobData* ptrJobData = new JobData(nID,strFilePath,myType,lu_fileCategory,
                                                                        strDestinationPath, JobType::COPY_FILE);
                JobQueue.push(ptrJobData);
                UpdateQuerry = "update File_Details set File_Status = 1 where File_Id =";
                UpdateQuerry += strId;
                mysql_query(mysql,UpdateQuerry.c_str());
            }
        }
    }
    catch( ... )
    {
        //throwing exception
    }
}



