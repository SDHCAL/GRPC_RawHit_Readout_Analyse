#ifndef TimeCounter_HH
#define TimeCounter_HH

#include <ctime>
#include <iostream>

#include <map>
#include <string>

class TimeCounter
{
 public:
 TimeCounter() : m_start(0), m_duration(0), m_NumberOfCall(0) {}
  void start() {m_start=std::clock();}
  void stop() {std::clock_t end=std::clock(); m_duration+=end-m_start; ++m_NumberOfCall;}
  double elapsedTime() const {return m_duration/CLOCKS_PER_SEC;}
  double elapsedTimePerCall() const {return m_NumberOfCall==0 ? m_NumberOfCall : elapsedTime()/m_NumberOfCall;}
 private:
  std::clock_t m_start;
  double m_duration;
  unsigned int m_NumberOfCall;
};

std::ostream& operator<<(std::ostream& flux, const TimeCounter& t)
{
  flux << t.elapsedTimePerCall() << " s (total=" << t.elapsedTime() << " s)";
  return flux;
}

class TimeCounterMap : public std::map<std::string,TimeCounter>
{
 public:
  ~TimeCounterMap()
    {
      for (std::map<std::string,TimeCounter>::iterator it=this->begin(); it != this->end(); ++it)
	std::cout << it->first << " : " << it->second << std::endl; 
    }
};

TimeCounterMap TimeCounters;

#endif
