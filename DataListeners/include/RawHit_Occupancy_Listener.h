#ifndef RawHit_Occupancy_Listener_HH
#define RawHit_Occupancy_Listener_HH

#include "RawHit_SDHCAL_Data_Listener.h"

#include "GG_counter.h"
#include <map>


class TDirectory;
class ExperimentalSetup;

class RawHit_Occupancy_Listener : public RawHit_SDHCAL_Data_Listener
{
 public:
  typedef unsigned int DIFNUMBER;
  typedef unsigned int ASICNUMBER;
  typedef unsigned int CHANNELNUMBER;
  void process(const RawHit_SDHCAL_Data&);
  void saveTo(TDirectory* d,ExperimentalSetup* e=NULL);
 private:
  unsigned int m_total=0; //C++11
  DIFCounters m_countBy_DifAsicChannel;
};

#endif
