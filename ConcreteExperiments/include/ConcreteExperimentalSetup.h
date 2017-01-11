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



#endif
