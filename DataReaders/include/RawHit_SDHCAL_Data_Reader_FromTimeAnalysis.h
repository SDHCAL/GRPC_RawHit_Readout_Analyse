#ifndef RawHit_SDHCAL_Data_Reader_FromTimeAnalysis_HH
#define RawHit_SDHCAL_Data_Reader_FromTimeAnalysis_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "intervalle.h"
#include "GG_messageCounter.h"
#include <map>
#include <vector>

class RawHit_SDHCAL_Data_Reader_FromTimeAnalysis : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
  RawHit_SDHCAL_Data_Reader_FromTimeAnalysis(intervalle<int> timeWindow) 
    :  m_SelectEventTimeWindow(timeWindow), m_DIFnumber_of_the_BIF(0), m_nEventSeen(0), m_nHitWithNegativeTimeStampSeen(0), m_discardNegativeTimeStamp(true), m_skipIfBIFisOutsideReadout(true), m_splitEventForListeners(true) {;}
  RawHit_SDHCAL_Data_Reader_FromTimeAnalysis(UI_intervalle timeWindow) 
    : m_SelectEventTimeWindow(intervalle<int>(timeWindow.first,timeWindow.second)), m_DIFnumber_of_the_BIF(0), m_nEventSeen(0), m_nHitWithNegativeTimeStampSeen(0), m_discardNegativeTimeStamp(true), m_skipIfBIFisOutsideReadout(true), m_splitEventForListeners(true) {;}
  virtual ~RawHit_SDHCAL_Data_Reader_FromTimeAnalysis();


  void setDiscardNegativeTimeStamp(bool value=true)  {m_discardNegativeTimeStamp=value;}
  void setSkipIfBIFisOutsideReadout(bool value=true) {m_skipIfBIFisOutsideReadout=value;}
  void setSplitEventForListeners(bool value=true) {m_splitEventForListeners=value;}
  void setBIFparameters(unsigned int BIF,intervalle<int> timeWindow) {m_DIFnumber_of_the_BIF=BIF; m_BIFtimeWindow=timeWindow;}
  void setBIFparameters(unsigned int BIF,UI_intervalle timeWindow) {setBIFparameters(BIF,intervalle<int>(timeWindow.first,timeWindow.second));}
  void setBIFtimeWindow(intervalle<int> timeWindow) {m_BIFtimeWindow=timeWindow;}
  void setBIFtimeWindow(UI_intervalle timeWindow) {setBIFtimeWindow(intervalle<int>(timeWindow.first,timeWindow.second));}
  bool hasBIFnumber() {return m_DIFnumber_of_the_BIF!=0;}

  void setEventTimeWindow(intervalle<int> timeWindow) {m_SelectEventTimeWindow=timeWindow;}
  void setEventTimeWindow(UI_intervalle timeWindow) {setEventTimeWindow(intervalle<int>(timeWindow.first,timeWindow.second));}

  void process(const RawHit_SDHCAL_Data&);

  //messages (public)
  GG_messageCounter outOfTimeReadout=GG_messageCounter("WARNING : RawHit_SDHCAL_Data_Reader_fromTimeAnalysis : requested intervalle after readout"); //C++11
  GG_messageCounter outOfTimeBIF=GG_messageCounter("WARNING : RawHit_SDHCAL_Data_Reader_fromTimeAnalysis : requested intervalle for BIF after readout"); //C++11


 private:
  intervalle<int> m_SelectEventTimeWindow;
  unsigned int m_DIFnumber_of_the_BIF; 
  intervalle<int> m_BIFtimeWindow;

  //counters;
  unsigned int m_nEventSeen;
  unsigned int m_nHitWithNegativeTimeStampSeen;


 protected:
  std::map<unsigned int,unsigned int> m_readoutTimeDistribution;
 private:
  //control flags
  bool m_discardNegativeTimeStamp;
  bool m_skipIfBIFisOutsideReadout;
  bool m_splitEventForListeners;

  //processing event steps
  void FillReadoutTimeDistribution(const RawHit_SDHCAL_Data&);
  virtual void translateToEventTimeIntervalle(std::vector<unsigned int>& eventsTimes)=0;
  virtual bool veto(const RawHit_SDHCAL_Data&) {return false;}

};

#endif


