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

float Setup_ConfigInfo::get_threshold_charge(unsigned int threshold,unsigned int DIF,unsigned int asic) const
{
  if (threshold<=1) return get_first_threshold_charge(DIF,asic);
  if (threshold==2) return get_second_threshold_charge(DIF,asic);
  return get_third_threshold_charge(DIF,asic);
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
  m_Yuris_small_scintillator_in_BIF=false;
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


float GIF_Conditions::attenuatorFactorApprox(unsigned int att)
{
  static float plane1app[3]={1,10,100};
  static float plane2app[3]={1,1.5,100};
  static float plane3app[3]={1,2.2,4.6};
  return plane1app[(att%1000)/100-1]*plane2app[(att%100)/10-1]*plane3app[att%10-1];
}

float GIF_Conditions::attenuatorFactor(unsigned int att)
{
  static float plane1[3]={1,10,100};
  static float plane2[3]={1,1.468,100};
  static float plane3[3]={1,2.154,4.642};
  return plane1[(att%1000)/100-1]*plane2[(att%100)/10-1]*plane3[att%10-1];
}


void RunQualityInfo::set_Status(STATUS s,std::string badcause)
{
  m_globalStatus=s;
  m_badReason=badcause;
  bool good=(s!=BAD);
  m_qualityByPlan.assign(m_qualityByPlan.size(),good);
}

void Beam_Conditions::clear()
{
  m_beamState=STATE::UNKNOWN;
  m_beamParticle=PARTICLE::UNKNOWN;
  m_beamParticleCharge=PARTICLE_CHARGE::UNKNOWN;
  m_beamEnergyInGeV=0;
  m_beamLine=LINE::UNKNOWN;
  m_beamPosition=POSITION::UNKNOWN;
  m_cerenkov.clear();
  m_beamIntensityInHzPerSquareCm=0;
  m_beamObstacle.clear();
}

bool Beam_Conditions::setToRun(unsigned int run)
{
  all_ConfigInfo &all=all_ConfigInfo::instance();
  bool success=false;
  clear();
  try 
    {
      const Beam_Conditions &s=all.getBeamConditions(run);
      success=true;
      (*this)=s;
    }
  catch (std::exception &e)
    {
      std::cout << e.what();
    }
  return success;
}

bool DAQ_Object_Info::setToRun(unsigned int run)
{
  all_ConfigInfo &all=all_ConfigInfo::instance();
  bool success=false;
  clear();
  try 
    {
      const DAQ_Object_Info &s=all.getDAQObjectInfo(run);
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

GIF_Conditions& all_ConfigInfo::changeGIFconditions(unsigned int run)
{
  std::map<unsigned int,GIF_Conditions>::iterator  it=m_runGIFconditionsMap.find(run);
  if (it==m_runGIFconditionsMap.end()) throw RunNotFound_ConfigException(run,"all_ConfigInfo::changeGIFconditions");
  return it->second;
}

const RunQualityInfo& all_ConfigInfo::getRunQualityInfo(unsigned int run) const
{
  std::map<unsigned int,RunQualityInfo>::const_iterator  it=m_runQualityInfoMap.find(run);
  if (it==m_runQualityInfoMap.end()) throw RunNotFound_ConfigException(run,"all_ConfigInfo::getRunQualityInfo");
  return it->second;
}

RunQualityInfo& all_ConfigInfo::changeRunQualityInfo(unsigned int run)
{
  std::map<unsigned int,RunQualityInfo>::iterator  it=m_runQualityInfoMap.find(run);
  if (it==m_runQualityInfoMap.end()) throw RunNotFound_ConfigException(run,"all_ConfigInfo::changeRunQualityInfo");
  return it->second;
}

const Beam_Conditions& all_ConfigInfo::getBeamConditions(unsigned int run) const
{
  std::map<unsigned int,Beam_Conditions>::const_iterator  it=m_runBeamConditionsMap.find(run);
  if (it==m_runBeamConditionsMap.end()) throw RunNotFound_ConfigException(run,"all_ConfigInfo::getBeamConditions");
  return it->second;
}
  
const DAQ_Object_Info& all_ConfigInfo::getDAQObjectInfo(unsigned int run) const
{
  std::map<unsigned int,DAQ_Object_Info>::const_iterator  it=m_runDAQ_Object_InfoMap.find(run);
  if (it==m_runDAQ_Object_InfoMap.end()) throw RunNotFound_ConfigException(run,"all_ConfigInfo::getDAQObjectInfo");
  return it->second;
}
  
