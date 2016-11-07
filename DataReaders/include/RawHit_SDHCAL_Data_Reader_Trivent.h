#ifndef RawHit_SDHCAL_Data_Reader_Trivent_HH
#define RawHit_SDHCAL_Data_Reader_Trivent_HH

#include "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis.h"

class RawHit_SDHCAL_Data_Reader_Trivent : public RawHit_SDHCAL_Data_Reader_FromTimeAnalysis
{
 public:
 RawHit_SDHCAL_Data_Reader_Trivent(unsigned int windowHalfSize, unsigned int minNhitInClockTick) : RawHit_SDHCAL_Data_Reader_FromTimeAnalysis(intervalle<int>(-windowHalfSize,windowHalfSize)), m_timeWin(windowHalfSize), m_noiseCut(minNhitInClockTick) {;}


 private:
  unsigned int m_timeWin;
  unsigned int m_noiseCut;

  virtual void translateToEventTimeIntervalle(std::list<unsigned int>& eventsTimes, const RawHit_SDHCAL_Data&);
};

#endif


