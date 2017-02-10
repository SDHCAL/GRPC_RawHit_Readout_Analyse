#ifndef RawCalorimeterHitPointerExperimentalSetupTools_HH
#define RawCalorimeterHitPointerExperimentalSetupTools_HH


#include "ExperimentalSetup.h"
#include "RawCalorimeterHitPointer.h"

class BaseExperimentalSetupGetter
{
 public:
 BaseExperimentalSetupGetter(ExperimentalSetup& setup,unsigned int level) : m_setup(setup),m_level(level) {}
  enum {ERRORCODE=9999};
  unsigned int get(const RawCalorimeterHitPointer &h)
  {
    unsigned int coord[3]={ERRORCODE,ERRORCODE,ERRORCODE};
    m_setup.getCoord3D(h.dif(),h.asic(),h.channel(),coord[0],coord[1],coord[2]);
    return coord[m_level];
  }
  
 private:
  ExperimentalSetup& m_setup;
  unsigned int m_level;
};

class rawHit_PlanNumber : public BaseExperimentalSetupGetter
{
 public:
 rawHit_PlanNumber(ExperimentalSetup& setup) : BaseExperimentalSetupGetter(setup,2) {}
};

class rawHit_IorStrip : public BaseExperimentalSetupGetter
{
 public:
 rawHit_IorStrip(ExperimentalSetup& setup) : BaseExperimentalSetupGetter(setup,0) {}
};

class rawHit_JorGap : public BaseExperimentalSetupGetter
{
 public:
 rawHit_JorGap(ExperimentalSetup& setup) : BaseExperimentalSetupGetter(setup,1) {}
};

#endif
