#ifndef RawHit_Plan_Cluster_Occupancy_Listener_HH
#define RawHit_Plan_Cluster_Occupancy_Listener_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "ExperimentalSetup.h"

#include "GG_counter.h"
class TDirectory;

class RawHit_Plan_Cluster_Occupancy_Listener : public RawHit_SDHCAL_Data_Listener
{
 public:
  typedef MappedCounters<MappedCounters<MappedCounters<SingleMapCounter,SingleMapCounter>,SingleMapCounter>,SingleMapCounter> SetupCounter;
  RawHit_Plan_Cluster_Occupancy_Listener(ExperimentalSetup& setup);
  void process(const RawHit_SDHCAL_Data&);
  
  void saveTo(TDirectory* ROOTdir);
  SetupCounter& runSummary() {return m_setupCounter;}
 private:

  ExperimentalSetup& m_setup;   

  SetupCounter m_setupCounter;
  //std::string padLabel[4]={"setup","plane","J","I"};
  //std::string stripLabel[4]={"setup","plane","gap","strip"};


};

#endif


