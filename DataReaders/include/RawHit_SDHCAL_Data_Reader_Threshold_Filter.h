#ifndef RawHit_SDHCAL_Data_Reader_Threshold_Filter_HH
#define RawHit_SDHCAL_Data_Reader_Threshold_Filter_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "GG_messageCounter.h"
#include "intervalle.h"

class RawHit_SDHCAL_Data_Reader_Threshold_Filter : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
 RawHit_SDHCAL_Data_Reader_Threshold_Filter(unsigned int thresholdLevel, bool exclusive=false, bool invert=false) : m_thresholdLevel(thresholdLevel), m_exclusive(exclusive), m_notInverted(! invert)   {;}
  void process(const RawHit_SDHCAL_Data&);
 private:
  unsigned int m_thresholdLevel;
  bool m_exclusive;
  bool m_notInverted;
};

#endif


