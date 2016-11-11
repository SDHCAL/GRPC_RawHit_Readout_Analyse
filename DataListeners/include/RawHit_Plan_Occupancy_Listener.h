#ifndef RawHit_Plan_Occupancy_Listener_HH
#define RawHit_Plan_Occupancy_Listener_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "domain.h"
#include "ExperimentalSetup.h"

#include "GG_counter.h"
class TDirectory;

class RawHit_Plan_Occupancy_Listener : public RawHit_SDHCAL_Data_Listener
{
 public:
  RawHit_Plan_Occupancy_Listener(ExperimentalSetup& setup);
  void process(const RawHit_SDHCAL_Data&);
  
  void setNoiseScale(float scale) {m_noiseScale=scale;}
  void setUnitString(std::string unit) {m_unit=unit;}

  void efficiencyReport();
  void noiseReport();
  void saveTo(TDirectory* ROOTdir);
 private:
  unsigned int m_DIFnumber_of_the_BIF; 
  UI_domain m_padDIFs, m_stripEvenDifs, m_stripOddDifs;

  ExperimentalSetup& m_setup;   

  unsigned int m_total;
  MappedCounters<MappedCounters<SingleCounter> > m_PlaneAsicCounters;

  float m_noiseScale;
  std::string m_unit;

};

#endif

