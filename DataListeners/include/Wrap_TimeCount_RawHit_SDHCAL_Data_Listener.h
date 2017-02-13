#ifndef Wrap_TimeCount_RawHit_SDHCAL_Data_Listener_HH
#define Wrap_TimeCount_RawHit_SDHCAL_Data_Listener_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "TimeCounter.h"

class Wrap_TimeCount_RawHit_SDHCAL_Data_Listener : public RawHit_SDHCAL_Data_Listener
{
 public:
 Wrap_TimeCount_RawHit_SDHCAL_Data_Listener(RawHit_SDHCAL_Data_Listener& listener, std::string counterName) : m_listener(listener), m_counterName(counterName) {}
  void process(const RawHit_SDHCAL_Data& d) {TimeCounters[m_counterName].start(); m_listener.process(d); TimeCounters[m_counterName].stop();}
 private:
  RawHit_SDHCAL_Data_Listener& m_listener;
  std::string m_counterName;
};
#endif
