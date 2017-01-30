#ifndef RawHit_SDHCAL_Data_Reader_TriggeredMode_HH
#define RawHit_SDHCAL_Data_Reader_TriggeredMode_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "intervalle.h"

class RawHit_SDHCAL_Data_Reader_TriggeredMode  : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
 RawHit_SDHCAL_Data_Reader_TriggeredMode(unsigned int timeWindowSize,unsigned int window_start=0)
    : m_timeWindow(window_start,window_start+timeWindowSize) {;}

  void process(const RawHit_SDHCAL_Data&);
 private:
  UI_intervalle m_timeWindow;
};

#endif
