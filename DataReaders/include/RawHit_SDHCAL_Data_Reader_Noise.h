#ifndef RawHit_SDHCAL_Data_Reader_Noise_HH
#define RawHit_SDHCAL_Data_Reader_Noise_HH

#include "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis.h"
#include "ExperimentalSetup.h"

class RawHit_SDHCAL_Data_Reader_Noise : public RawHit_SDHCAL_Data_Reader_FromTimeAnalysis
{
 public:
  RawHit_SDHCAL_Data_Reader_Noise(ExperimentalSetup& setup, unsigned int noiseWindowLength);

  void setMaxTouchedPlans(unsigned int value) {m_maxTouchedPlans=value;}
  void setMaxNumberOfHits(unsigned int value) {m_maxNumberOfHits=value;}
  void setVetoOnBIF(bool value=true) {m_vetoOnBIF=value;}

 private:
  ExperimentalSetup& m_setup;
  unsigned int m_noiseWindowLength;
  
  unsigned int m_maxTouchedPlans; 
  unsigned int m_maxNumberOfHits; 
  bool m_vetoOnBIF;

 private:
  static const unsigned int NO_CUTVALUE;

  virtual void translateToEventTimeIntervalle(std::vector<unsigned int>& eventsTimes);
  virtual bool veto(const RawHit_SDHCAL_Data&);
};

#endif


