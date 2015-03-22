
 /*
 * ExceptionBase.cpp - implementation of class ExceptionBase
 *
 * @author :    SANOJ THOMAS
 */
#include"ExceptionBase.h"

ExceptionBase::ExceptionBase(const std::string& exceptionName,
                             const char * file,
                             const char * function,
                             int line,
                             const std::string& what,
                             bool logException)
  : std::exception(what.c_str()),
  mFile(file),
  mLine(line),
  mFunction(function),
  mExceptionName(exceptionName)
 {
  if (logException)
  {
    std::string logStr = exceptionName + std::string(" [from %s in %s: %d ] : %s");
    //Exception thrown from here
  }
}

ExceptionBase::ExceptionBase(const ExceptionBase& exceptionBase)
  :std::exception(exceptionBase.what())
{
  mFile = exceptionBase.mFile;
  mLine = exceptionBase.mLine;
  mFunction = exceptionBase.mFunction;
  mExceptionName = exceptionBase.mExceptionName;
}

ExceptionBase & ExceptionBase:: operator = (const ExceptionBase& exceptionBase)
{
  mFile = exceptionBase.mFile;
  mLine = exceptionBase.mLine;
  mFunction = exceptionBase.mFunction;
  mExceptionName = exceptionBase.mExceptionName;
  return *this;
}

ExceptionBase::~ExceptionBase(void)
{
}


std::string ExceptionBase::GetFile()
{
  return mFile;
}


std::string ExceptionBase::GetFunction()
{
  return mFunction;
}


int ExceptionBase::GetLine()
{
  return mLine;
}

std::string ExceptionBase::GetExceptionName()
{
  return mExceptionName;
}