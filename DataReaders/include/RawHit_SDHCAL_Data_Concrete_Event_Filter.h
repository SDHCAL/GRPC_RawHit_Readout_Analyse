#ifndef RawHit_SDHCAL_Data_Concrete_Event_Filter_HH
#define RawHit_SDHCAL_Data_Concrete_Event_Filter_HH

#include "RawHit_SDHCAL_Data_Event_Filter.h"
#include "domain.h"


class Inverse_Filter : public RawHit_SDHCAL_Data_Event_Filter
{
public:
  Inverse_Filter(RawHit_SDHCAL_Data_Event_Filter& theFilter)
    : m_theFilter(theFilter) {;}

  bool reject(const RawHit_SDHCAL_Data& d) { return not m_theFilter.reject(d);} 
  std::string name();
  
private:
  RawHit_SDHCAL_Data_Event_Filter& m_theFilter;
};

class Both_Reject_Filter : public RawHit_SDHCAL_Data_Event_Filter
{
 public:
 Both_Reject_Filter(RawHit_SDHCAL_Data_Event_Filter& filterOne, RawHit_SDHCAL_Data_Event_Filter& filter2)
   : m_filterOne(filterOne), m_filterTwo(filter2) {;}
  
  bool reject(const RawHit_SDHCAL_Data& d);
  std::string name();
 private:
  RawHit_SDHCAL_Data_Event_Filter& m_filterOne;
  RawHit_SDHCAL_Data_Event_Filter& m_filterTwo;
};


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


class NoHitInLayers_Filter : public RawHit_SDHCAL_Data_Event_Filter_Using_ExperimantalSetup
{
 public:
 NoHitInLayers_Filter(UI_domain layers,ExperimentalSetup& setup)
   : RawHit_SDHCAL_Data_Event_Filter_Using_ExperimantalSetup(setup), m_layersToCheck(layers) {;}
  bool reject(const RawHit_SDHCAL_Data& d);
  std::string name();
 private:
  UI_domain m_layersToCheck;
};


class ConsecutiveLayers_Filter : public RawHit_SDHCAL_Data_Event_Filter_Using_ExperimantalSetup
{
 public:
  ConsecutiveLayers_Filter(unsigned int nPlan,ExperimentalSetup& setup)
    : RawHit_SDHCAL_Data_Event_Filter_Using_ExperimantalSetup(setup),m_minNumberOfPlanToKeep(nPlan){;}
  bool reject(const RawHit_SDHCAL_Data& d);
  std::string name();
 private:
  unsigned int m_minNumberOfPlanToKeep;
};


class MinNumberOfHits_Filter : public RawHit_SDHCAL_Data_Event_Filter
{
 public:
  MinNumberOfHits_Filter(unsigned int MinNumberOfRamFullHitToKeep)
    : m_MinNumberOfRamFullHitToKeep(MinNumberOfRamFullHitToKeep) {;}
  bool reject(const RawHit_SDHCAL_Data& d) {return d.getHitVector().size()<m_MinNumberOfRamFullHitToKeep;}
  std::string name();
 private:
  unsigned int m_MinNumberOfRamFullHitToKeep;
};


#endif
