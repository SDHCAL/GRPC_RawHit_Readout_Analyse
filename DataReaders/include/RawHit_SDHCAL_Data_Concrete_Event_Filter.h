#ifndef RawHit_SDHCAL_Data_Concrete_Event_Filter_HH
#define RawHit_SDHCAL_Data_Concrete_Event_Filter_HH

#include "RawHit_SDHCAL_Data_Event_Filter.h"

class RamFull_Filter : public RawHit_SDHCAL_Data_Event_Filter
{
 public:
  RamFull_Filter(unsigned int MinNumberOfRamFullHitToReject, float minRamFullHitFractionToReject)
    : m_MinNumberOfRamFullHitToReject(MinNumberOfRamFullHitToReject),m_minRamFullHitFractionToReject(minRamFullHitFractionToReject) {;}
  bool reject(const RawHit_SDHCAL_Data& d);
  std::string name();
 private:
  unsigned int m_MinNumberOfRamFullHitToReject;
  float m_minRamFullHitFractionToReject;
};

#endif
