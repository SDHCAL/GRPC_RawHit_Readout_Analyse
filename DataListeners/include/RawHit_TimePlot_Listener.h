#ifndef RawHit_TimePlot_Listener_HH
#define RawHit_TimePlot_Listener_HH


#include "RawHit_SDHCAL_Data_Listener.h"
#include "ExperimentalSetup.h"
#include "GG_counter.h"
#include "GG_experimentalsetup_counters.h"

#include <iostream>

class TDirectory;


class RawHit_TimePlot_Listener : public RawHit_SDHCAL_Data_Listener
{
 public:
  RawHit_TimePlot_Listener(ExperimentalSetup& setup);
  void process(const RawHit_SDHCAL_Data&);

  void printReport(std::ostream& oflux=std::cout);
  void saveTo(TDirectory* ROOTdir);
 private:
  ExperimentalSetup& m_setup;
  GG_ExperimentalSetup_Counter m_negativeTimeStampCounters;
};


#endif
