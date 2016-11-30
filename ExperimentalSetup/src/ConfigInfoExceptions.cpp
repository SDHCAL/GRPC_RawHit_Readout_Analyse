#include "ConfigInfoExceptions.h"

#include <sstream>


const char* UnknownDIFexception::what() const throw()
{
  std::stringstream ss;
  ss <<"Setup_ConfigInfo: DIF " << m_dif <<"not found." <<std::endl;
  return ss.str().c_str();
}

const char* RunNotFound_ConfigException::what() const throw()
{
  std::stringstream ss;
  ss << m_header << " : run " << m_run << " not found." <<std::endl;
  return ss.str().c_str();
}

const char* UnknownConfigException::what() const throw()
{
  std::string s="Unknown configuration : ";
  s+=m_state;
  return s.c_str();
}
