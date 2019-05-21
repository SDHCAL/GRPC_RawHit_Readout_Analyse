#ifndef Time_Decorator_For_RawHit_SDHCAL_Data_Listener_HH
#define Time_Decorator_For_RawHit_SDHCAL_Data_Listener_HH

#include <string>
#include "TimeCounter.h"
#include "RawHit_SDHCAL_Data_Listener.h"

class Time_Decorator_For_RawHit_SDHCAL_Data_Listener : public RawHit_SDHCAL_Data_Listener
{
 public:
  Time_Decorator_For_RawHit_SDHCAL_Data_Listener(RawHit_SDHCAL_Data_Listener& decoratee, std::string name);
  void process(const RawHit_SDHCAL_Data& d);
 private:
  RawHit_SDHCAL_Data_Listener& m_decoratee;
  TimeCounter& m_counter;
};


#endif
