#include "TimeCounter.h"

TimeCounterMap TimeCounters;

std::ostream& operator<<(std::ostream& flux, const TimeCounter& t)
{
  flux << t.elapsedTimePerCall() << " s (total=" << t.elapsedTime() << " s)";
  return flux;
}
