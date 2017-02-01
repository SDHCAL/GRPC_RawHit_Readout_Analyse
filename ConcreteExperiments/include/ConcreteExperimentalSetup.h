#ifndef ConcreteExperimentalSetup_HH
#define ConcreteExperimentalSetup_HH

#include "ExperimentalSetup.h"

class GIF_oct2016_ExperimentalSetup : public ExperimentalSetup
{
 public:
  GIF_oct2016_ExperimentalSetup();
 private:
  void setConfig();
  void setRunList();
  void setRunQuality();
  void setGIFconditions();
};


class CERN_904_CosmicTestBench_Early2017_ExperimentalSetup : public ExperimentalSetup
{
 public:
  CERN_904_CosmicTestBench_Early2017_ExperimentalSetup()
    {
      addCMSstrip(66,69); //verre
      addCMSstrip(26,23); //bakelite
    }
};


#endif
