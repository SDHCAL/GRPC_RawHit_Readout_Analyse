#ifndef RawHit_SDHCAL_Data_Reader_Noise_HH
#define RawHit_SDHCAL_Data_Reader_Noise_HH

#include "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis.h"
#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitPointerTools.h"
#include "ExperimentalSetup.h"

class RawHit_SDHCAL_Data_Reader_Noise : public RawHit_SDHCAL_Data_Reader_FromTimeAnalysis
{
 public:
  RawHit_SDHCAL_Data_Reader_Noise(ExperimentalSetup& setup, unsigned int noiseWindowLength);

  void setMaxTouchedPlans(unsigned int value) {m_maxTouchedPlans=value;}
  void setMaxNumberOfHits(unsigned int value) {m_maxNumberOfHits=value;}

 private:
  ExperimentalSetup& m_setup;
  unsigned int m_noiseWindowLength;
  
  unsigned int m_maxTouchedPlans; 
  unsigned int m_maxNumberOfHits; 

 private:
  static const unsigned int NO_CUTVALUE;

  virtual void translateToEventTimeIntervalle(std::list<unsigned int>& eventsTimes, const RawHit_SDHCAL_Data& d);
  bool veto(UI_intervalle timeWin, const RawHit_SDHCAL_Data& d) {return vetoHits(timeWin,d) && vetoPlans(timeWin,d);}
  bool vetoHits(UI_intervalle timeWin, const RawHit_SDHCAL_Data& d) {return m_maxNumberOfHits<NO_CUTVALUE && std::count_if(d.getHitVector().begin(), d.getHitVector().end(), rawHit_IsInIntervalle<rawHit_TimeStamp>(timeWin))>0;}
  bool vetoPlans(UI_intervalle timeWin, const RawHit_SDHCAL_Data& d);
};

#endif


