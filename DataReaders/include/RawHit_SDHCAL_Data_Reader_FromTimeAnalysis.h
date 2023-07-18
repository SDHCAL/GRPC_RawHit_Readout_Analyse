#ifndef RawHit_SDHCAL_Data_Reader_FromTimeAnalysis_HH
#define RawHit_SDHCAL_Data_Reader_FromTimeAnalysis_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "RawCalorimeterHitPointer.h"
#include "intervalle.h"
#include "GG_messageCounter.h"
#include <map>
#include <vector>
#include <list>
#include <limits>

class RawHit_SDHCAL_Data_Reader_FromTimeAnalysis : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
  RawHit_SDHCAL_Data_Reader_FromTimeAnalysis(intervalle<int> timeWindow) 
    :  m_SelectEventTimeWindow(timeWindow), m_nEventSeen(0), m_nHitWithNegativeTimeStampSeen(0), m_discardNegativeTimeStamp(true), m_skipIfBIFisOutsideReadout(true), m_splitEventForListeners(true), m_vetoOnBIF(false) {setMaxEventsToSendDefault();}
  RawHit_SDHCAL_Data_Reader_FromTimeAnalysis(UI_intervalle timeWindow) 
    : m_SelectEventTimeWindow(intervalle<int>(timeWindow.first,timeWindow.second)), m_nEventSeen(0), m_nHitWithNegativeTimeStampSeen(0), m_discardNegativeTimeStamp(true), m_skipIfBIFisOutsideReadout(true), m_splitEventForListeners(true), m_vetoOnBIF(false)  {setMaxEventsToSendDefault();}
  virtual ~RawHit_SDHCAL_Data_Reader_FromTimeAnalysis();


  void setDiscardNegativeTimeStamp(bool value=true)  {m_discardNegativeTimeStamp=value;}
  void setSkipIfBIFisOutsideReadout(bool value=true) {m_skipIfBIFisOutsideReadout=value;}
  void setSplitEventForListeners(bool value=true) {m_splitEventForListeners=value;}
  void setBIFparameters(unsigned int BIF,intervalle<int> timeWindow) {m_BIFtimeWindow_per_BIF_DIFnumber[BIF]=timeWindow;}
  void setBIFparameters(unsigned int BIF,UI_intervalle timeWindow) {setBIFparameters(BIF,intervalle<int>(timeWindow.first,timeWindow.second));}
  void setBIFtimeWindow(unsigned int BIF,intervalle<int> timeWindow) {m_BIFtimeWindow_per_BIF_DIFnumber.at(BIF)=timeWindow;}
  void setBIFtimeWindow(unsigned int BIF,UI_intervalle timeWindow) {setBIFtimeWindow(BIF,intervalle<int>(timeWindow.first,timeWindow.second));}
  bool hasBIFnumber() {return !m_BIFtimeWindow_per_BIF_DIFnumber.empty() ;}

  void setEventTimeWindow(intervalle<int> timeWindow) {m_SelectEventTimeWindow=timeWindow;}
  void setEventTimeWindow(UI_intervalle timeWindow) {setEventTimeWindow(intervalle<int>(timeWindow.first,timeWindow.second));}
  void setVetoOnBIF(bool value=true) {m_vetoOnBIF=value;}
  void setMaxEventsToSend(unsigned int maxEventsToSend) {m_maxEventsToSend=maxEventsToSend;}
  void setMaxEventsToSendDefault() {m_maxEventsToSend=std::numeric_limits<unsigned int>::max()-1000000;}

  void process(const RawHit_SDHCAL_Data&);

  //messages (public)
  GG_messageCounter outOfTimeReadout=GG_messageCounter("WARNING : RawHit_SDHCAL_Data_Reader_fromTimeAnalysis : requested intervalle after readout"); //C++11
  GG_messageCounter outOfTimeBIF=GG_messageCounter("WARNING : RawHit_SDHCAL_Data_Reader_fromTimeAnalysis : requested intervalle for BIF after readout"); //C++11
  GG_messageCounter tooMuchEvent=GG_messageCounter("INFORMATION : RawHit_SDHCAL_Data_Reader_fromTimeAnalysis : above max events"); //C++11

 private:
  intervalle<int> m_SelectEventTimeWindow;
  std::map<unsigned int, intervalle<int> > m_BIFtimeWindow_per_BIF_DIFnumber;

  //counters;
  unsigned int m_nEventSeen;
  unsigned int m_nHitWithNegativeTimeStampSeen;
  unsigned int m_maxEventsToSend;

 protected:
  std::map<unsigned int,unsigned int> m_readoutTimeDistribution;
 private:
  //control flags
  bool m_discardNegativeTimeStamp;
  bool m_skipIfBIFisOutsideReadout;
  bool m_splitEventForListeners;
  bool m_vetoOnBIF;

  //processing event steps
  void FillReadoutTimeDistribution(const RawHit_SDHCAL_Data&);
  virtual void translateToEventTimeIntervalle(std::list<unsigned int>& eventsTimes, const RawHit_SDHCAL_Data& d)=0;
  void BIFveto(std::list<unsigned int>& eventsTimes, const RawHit_SDHCAL_Data& d);

  //internal needed data for process
  std::map<unsigned int,std::vector<RawCalorimeterHitPointer> > BIF_hitvector_per_BIF_DIFnumber;

};

#endif


