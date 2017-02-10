#ifndef RawHit_SDHCAL_Data_Reader_FromClusters_HH
#define RawHit_SDHCAL_Data_Reader_FromClusters_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "ExperimentalSetup.h"



class RawHit_SDHCAL_Data_Reader_FromClusters  : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
 RawHit_SDHCAL_Data_Reader_FromClusters(ExperimentalSetup& setup) : m_setup(setup) {;}

  void process(const RawHit_SDHCAL_Data&);
 private:
  ExperimentalSetup& m_setup;
};

#endif
