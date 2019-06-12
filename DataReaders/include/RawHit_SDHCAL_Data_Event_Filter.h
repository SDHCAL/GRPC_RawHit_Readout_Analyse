#ifndef RawHit_SDHCAL_Data_Event_Filter_HH
#define RawHit_SDHCAL_Data_Event_Filter_HH

#include <string>
#include "RawHit_SDHCAL_Data.h"
#include "ExperimentalSetup.h"

class RawHit_SDHCAL_Data_Event_Filter
{
 public:
  virtual bool reject(const RawHit_SDHCAL_Data& d)=0;
  virtual std::string name()=0;
};

class RawHit_SDHCAL_Data_Event_Filter_Using_ExperimantalSetup : public RawHit_SDHCAL_Data_Event_Filter
{
 public:
 RawHit_SDHCAL_Data_Event_Filter_Using_ExperimantalSetup(ExperimentalSetup& setup) : m_setup(setup) {;}
 protected:
  ExperimentalSetup& m_setup;
};

#endif
