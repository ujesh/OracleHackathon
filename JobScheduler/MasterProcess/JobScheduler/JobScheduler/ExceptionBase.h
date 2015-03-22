 /*
 * ExceptionBase.cpp - header of class ExceptionBase
 *
 * @author :    SANOJ THOMAS
 */
#include <exception>
#include "string"
class ExceptionBase : public std::exception
  {

  public: //FUNCTIONS
      /*!
      * \brief Constructor.
      *
      * \param exceptionName Name of the exception being thrown.
      * \param file Character array containing the file calling this exception.
      *             This is the result of the __FILE__ macro.
      * \param function Character array containing the function calling this
      *             exception. This is the result of the __FUNCTION__ macro.
      * \param line Integer of the line number where the exception
      *             was called. This is the result of the __LINE__ macro.
      * \param what String explaining why the exception was thrown.
      * \param logException Automatically add this exception to the log file
      *             when the object is constructed.
      */
      ExceptionBase(const std::string& exceptionName,
                    const char * file,
                    const char * function,
                    int line,
                    const std::string& what = "",
                    bool logException = true);

      /*!
      * \brief Copy constructor.
      *
      * \param exceptionBase Reference to an object of ExceptionBase.
      */
      ExceptionBase(const ExceptionBase& exceptionBase);

      /*!
      * \brief Assignment operator.
      *
      * \param exceptionBase Reference to an object of ExceptionBase.
      *
      * \return Reference to the ExceptionBase object.
      */
      ExceptionBase & operator=(const ExceptionBase& exceptionBase);

      /*!
      * \brief Destructor.
      */
    ~ExceptionBase();

      /*!
      * \brief Get the file that threw the exception.
      *
      * \return String containing the file that threw the exception.
      */
    std::string GetFile();

      /*!
      * \brief Get the line that threw the exception.
      *
      * \return Integer containing the line that threw the exception.
      */
    int GetLine();

      /*!
      * \brief Get the function that threw the exception.
      *
      * \return String containing the function that threw the exception.
      */
    std::string GetFunction();
      
      /*!
      * \brief Get the exception name of this exception.
      *
      * \return String containing the name of this exception.
      */
    std::string GetExceptionName();

  public: //VARIABLES
  protected: //FUNCTIONS
  protected: //VARIABLES
  private:  //FUNCTIONS
  private: //VARIABLES
    /*! The file where the exception was created. */
    std::string mFile;
    /*! The line where the exception was created. */
    int mLine;
    /*! The function where the exception was created. */
    std::string mFunction;
    /*! The name of the exception class created. */
    std::string mExceptionName;
  };

  
  
  

  
  