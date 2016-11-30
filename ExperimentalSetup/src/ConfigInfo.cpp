#include "ConfigInfo.h"


const DIF_ASIC_HR_ConfigInfo& Setup_ConfigInfo::getConfig(unsigned int DIF) const 
{
  std::map<unsigned int,DIF_ASIC_HR_ConfigInfo>::const_iterator it=m_setup_ConfigInfo.find(DIF);
  if (it==m_setup_ConfigInfo.end()) throw UnknownDIFexception(DIF);
  return it->second;
}

std::vector<unsigned int> Setup_ConfigInfo::getKnownDIFnumbers() const
{
  std::vector<unsigned int> DIFnumbers;
  for (std::map<unsigned int,DIF_ASIC_HR_ConfigInfo>::const_iterator it=m_setup_ConfigInfo.begin(); it!=m_setup_ConfigInfo.end(); ++it)
    DIFnumbers.push_back(it->first);
  return DIFnumbers;
}

bool Setup_ConfigInfo::setToRun(unsigned int run)
{
  all_ConfigInfo &all=all_ConfigInfo::instance();
  bool success=false;
  clear();
  try 
    {
      const Setup_ConfigInfo& s=all.getConfigInfo(run);
      success=true;
      (*this)=s;
    }
  catch (std::exception &e)
    {
      std::cout << e.what();
    }
  return success;
}

void GIF_Conditions::clear()
{
  m_beamState=m_sourceState=UNKNOWN;
  m_sourceAttDown=m_sourceAttUp=0;
  m_scintillator="Not Set";
}

bool GIF_Conditions::setToRun(unsigned int run)
{
  all_ConfigInfo &all=all_ConfigInfo::instance();
  bool success=false;
  clear();
  try 
    {
      const GIF_Conditions &s=all.getGIFconditions(run);
      success=true;
      (*this)=s;
    }
  catch (std::exception &e)
    {
      std::cout << e.what();
    }
  return success;
}


std::string all_ConfigInfo::UnknownConfig=std::string("UnknownConfig");

all_ConfigInfo* all_ConfigInfo::m_instance=NULL;

all_ConfigInfo& all_ConfigInfo::instance() 
{
  if (NULL == m_instance ) m_instance=new all_ConfigInfo();
  return *m_instance;
}

const Setup_ConfigInfo& all_ConfigInfo::getConfigInfo(std::string s) const
{
  std::map<std::string,Setup_ConfigInfo>::const_iterator it=m_configs.find(s);
  if (it==m_configs.end()) throw UnknownConfigException(s);
  return it->second;
}

std::string all_ConfigInfo::getConfigName(unsigned int run) const
{
  std::map<unsigned int,std::string>::const_iterator  it=m_runConfigMap.find(run);
  if (it==m_runConfigMap.end()) throw RunNotFound_ConfigException(run,"all_ConfigInfo::getConfigName");
  return it->second;
}

const GIF_Conditions& all_ConfigInfo::getGIFconditions(unsigned int run) const
{
  std::map<unsigned int,GIF_Conditions>::const_iterator  it=m_runGIFconditionsMap.find(run);
  if (it==m_runGIFconditionsMap.end()) throw RunNotFound_ConfigException(run,"all_ConfigInfo::getGIFconditions");
  return it->second;
}


