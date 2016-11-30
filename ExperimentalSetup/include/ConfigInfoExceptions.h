#ifndef ConfigInfoExceptions_HH
#define ConfigInfoExceptions_HH

#include <exception>
#include <string>

class UnknownDIFexception : public std::exception
{
 public:
 UnknownDIFexception(unsigned int dif) : m_dif(dif) {}
  virtual ~UnknownDIFexception() throw() {}
  virtual const char* what() const throw();
 private:
  unsigned int m_dif;
};

class RunNotFound_ConfigException : public std::exception
{
 public:
  RunNotFound_ConfigException (unsigned int run,std::string header) : m_run(run), m_header(header) {}
  virtual ~RunNotFound_ConfigException() throw() {}
  virtual const char* what() const throw();
 private:
  unsigned int m_run;
  std::string m_header;
};

class UnknownConfigException : public std::exception
{
 public:
  UnknownConfigException(std::string value) : m_state(value) {}
  virtual ~UnknownConfigException()throw() {}
  virtual const char* what() const throw();
 private: 
  std::string m_state;
};



#endif
