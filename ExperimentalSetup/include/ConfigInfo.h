#ifndef ConfigInfo_HH
#define ConfigInfo_HH

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include "ConfigInfoExceptions.h"

class ASIC_HR_ConfigInfo
{
 public:
  ASIC_HR_ConfigInfo(unsigned int thresh1=250, unsigned int thresh2=250, unsigned int thresh3=250, unsigned int default_gain=128) : m_default_gain(default_gain)
  {
    m_threshold[0]=thresh1;
    m_threshold[1]=thresh2;
    m_threshold[2]=thresh3;
  }
  void setGainVec( const std::vector<unsigned int>& vec) { m_gain=vec;}
  unsigned int get_threshold(unsigned int i) const {return m_threshold[i-1];}
  unsigned int get_first_threshold() const {return m_threshold[0];}
  unsigned int get_second_threshold() const {return m_threshold[1];}
  unsigned int get_third_threshold() const {return m_threshold[2];}
  unsigned int getGain(unsigned int channel) const { return (m_gain.empty() ? m_default_gain : m_gain[channel]);}
  
 private:
  unsigned int m_threshold[3];
  unsigned int m_default_gain;
  std::vector<unsigned int> m_gain;
};


class DIF_ASIC_HR_ConfigInfo
{
 public:
  DIF_ASIC_HR_ConfigInfo() : m_default(), m_ASIC_ConfigInfo() {}
  DIF_ASIC_HR_ConfigInfo(ASIC_HR_ConfigInfo a) : m_default(a), m_ASIC_ConfigInfo() {}
  void setDefaultConfig(ASIC_HR_ConfigInfo a) {m_default=a;}
  void setConfig(unsigned int asic,ASIC_HR_ConfigInfo a) {m_ASIC_ConfigInfo[asic]=a;}
  void setConfigSame(ASIC_HR_ConfigInfo a) {m_default=a; m_ASIC_ConfigInfo.clear();}
  const ASIC_HR_ConfigInfo& getConfig(unsigned int asic) const
  {
    std::map <unsigned int,ASIC_HR_ConfigInfo>::const_iterator it=m_ASIC_ConfigInfo.find(asic);
    return (it==m_ASIC_ConfigInfo.end() ? m_default : it->second);
  } 
 private:
  ASIC_HR_ConfigInfo m_default;
  std::map <unsigned int,ASIC_HR_ConfigInfo> m_ASIC_ConfigInfo; 
};

class Setup_ConfigInfo
{
 public:
  Setup_ConfigInfo() : m_setup_ConfigInfo() {}
  void setConfig(unsigned int DIF,ASIC_HR_ConfigInfo a) { m_setup_ConfigInfo[DIF]=DIF_ASIC_HR_ConfigInfo(a);}
  void setConfig(unsigned int DIF,unsigned int asic,ASIC_HR_ConfigInfo a){ m_setup_ConfigInfo[DIF].setConfig(asic,a);}
  const DIF_ASIC_HR_ConfigInfo& getConfig(unsigned int DIF) const; //throw UnknownDIFexception
  // To ease interactive access
  std::vector<unsigned int> getKnownDIFnumbers() const;
  const ASIC_HR_ConfigInfo& getConfig(unsigned int DIF,unsigned int asic) const {return getConfig(DIF).getConfig(asic);}
  unsigned int get_threshold(unsigned int DIF,unsigned int asic,unsigned int threshold) const { return getConfig(DIF,asic).get_threshold(threshold);}
  unsigned int get_first_threshold(unsigned int DIF,unsigned int asic) const {return get_threshold(DIF,asic,1);}
  unsigned int get_second_threshold(unsigned int DIF,unsigned int asic) const {return get_threshold(DIF,asic,2);}
  unsigned int get_third_threshold(unsigned int DIF,unsigned int asic) const {return get_threshold(DIF,asic,3);}
  unsigned int getGain(unsigned int DIF,unsigned int asic,unsigned int channel) const { return getConfig(DIF,asic).getGain(channel);}
  // end to ease interactive access
  void clear() {m_setup_ConfigInfo.clear();}
  bool setToRun(unsigned int run);
 private:
  std::map<unsigned int,DIF_ASIC_HR_ConfigInfo> m_setup_ConfigInfo;
};

class GIF_Conditions
{
 public:
  enum STATE {UNKNOWN,ON,OFF};
  GIF_Conditions() : m_beamState(UNKNOWN), m_sourceState(UNKNOWN), m_sourceAttDown(0), m_sourceAttUp(0), m_scintillator("Not Set") {}
  void setSourceOFF() {  m_sourceState=OFF; m_sourceAttDown=0; m_sourceAttUp=0;}
  void setSourceUnknown() { m_sourceState=UNKNOWN; m_sourceAttDown=0; m_sourceAttUp=0;}
  void setSourceON(unsigned int down, unsigned int up) {m_sourceState=ON; m_sourceAttDown=down; m_sourceAttUp=up;}
  void setBeamON() {m_beamState=ON;}
  void setBeamOFF() {m_beamState=OFF;}
  void setBeamUnknown() {m_beamState=UNKNOWN;}
  void setScintillator(std::string value) {m_scintillator=value;}
  STATE getSourceStatus() const {return m_sourceState;}
  unsigned int getDownAtt() const {return m_sourceAttDown;}
  unsigned int getUpAtt() const {return m_sourceAttUp;}
  STATE getBeamStatus() const {return m_beamState;}
  std::string getScintillator() {return m_scintillator;}
  void clear();
  bool setToRun(unsigned int run);
 private:
  STATE m_beamState;
  STATE m_sourceState;
  unsigned int m_sourceAttDown;
  unsigned int m_sourceAttUp;
  std::string m_scintillator;
};


class  all_ConfigInfo
{
  all_ConfigInfo() {}
  all_ConfigInfo(const all_ConfigInfo&) {}
 public:
  static std::string UnknownConfig;
  static all_ConfigInfo& instance();

  void addConfigInfo(std::string s,Setup_ConfigInfo &conf) {m_configs[s]=conf;}
  const Setup_ConfigInfo& getConfigInfo(std::string s) const; //throw UnknownConfigException

  void addRun(unsigned int run,std::string conf=UnknownConfig) {m_runConfigMap[run]=conf;}
  std::string getConfigName(unsigned int run) const; // throw RunNotFound_ConfigException
  const Setup_ConfigInfo& getConfigInfo(unsigned int run) const {return getConfigInfo(getConfigName(run));}

  void addRun(unsigned int run,GIF_Conditions &cond) {m_runGIFconditionsMap[run]=cond;}
  const GIF_Conditions& getGIFconditions(unsigned int run) const; // throw RunNotFound_ConfigException
 private:
  static all_ConfigInfo* m_instance;
  std::map<std::string,Setup_ConfigInfo> m_configs;
  std::map<unsigned int,std::string> m_runConfigMap;
  std::map<unsigned int,GIF_Conditions> m_runGIFconditionsMap;
};
#endif
