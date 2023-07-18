#ifndef RawHit_SDHCAL_Data_Reader_HorizontalTrack_HH
#define RawHit_SDHCAL_Data_Reader_HorizontalTrack_HH


#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "RawCalorimeterHitPointer.h"
#include "intervalle.h"
#include "ExperimentalSetup.h"


//better to run it after Trivent like processor
class RawHit_SDHCAL_Data_Reader_HorizontalTrack  : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
 RawHit_SDHCAL_Data_Reader_HorizontalTrack(ExperimentalSetup& setup, bool verbose=false) :  m_setup(setup), m_minNumberOfPlan(3), m_maxClusterSpan(3), m_expandClusterTime(1), m_trackEvents(0), m_nevent(0), m_verbose(verbose)  {;}
  ~RawHit_SDHCAL_Data_Reader_HorizontalTrack();
  
  unsigned int minNumberOfPlan() const {return m_minNumberOfPlan;}
  void setMinNumberOfPlan(unsigned int val) {m_minNumberOfPlan=val;}

  unsigned int maxClusterSpan() const {return m_maxClusterSpan;}
  void setMaxClusterSpan(unsigned int val) {m_maxClusterSpan=val;}

  unsigned int expandClusterTime() const {return m_expandClusterTime;}
  void setExpandClusterTime(unsigned int val) {m_expandClusterTime=val;}

  void setBIFtimeWindow(unsigned int BIF, intervalle<int> timeWindow) {m_BIFtimeWindow_per_BIF_DIF_number[BIF]=timeWindow;}
  void setBIFtimeWindow(unsigned int BIF, UI_intervalle timeWindow) {setBIFtimeWindow(BIF,intervalle<int>(timeWindow.first,timeWindow.second));}
 
  void process(const RawHit_SDHCAL_Data&);
 private:
  ExperimentalSetup& m_setup;
  unsigned int m_minNumberOfPlan;
  unsigned int m_maxClusterSpan;
  unsigned int m_expandClusterTime;
  std::map<unsigned int,intervalle<int> > m_BIFtimeWindow_per_BIF_DIF_number;
  unsigned int m_trackEvents;
  unsigned int m_nevent;
  bool m_verbose;
  
  bool clusterIsOK(std::vector<RawCalorimeterHitPointer>::iterator &begin,std::vector<RawCalorimeterHitPointer>::iterator &end);
  void addBIFhit(std::vector<RawCalorimeterHitPointer>& event,std::vector<RawCalorimeterHitPointer>& trackEvent,UI_intervalle &timeSpan);

};

#endif
