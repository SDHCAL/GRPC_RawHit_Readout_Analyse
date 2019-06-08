#ifndef RawHit_SDHCAL_Data_Reader_Event_Filter_HH
#define RawHit_SDHCAL_Data_Reader_Event_Filter_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "RawHit_SDHCAL_Data_Event_Filter.h"
#include <utility>
#include <vector>

class RawHit_SDHCAL_Data_Reader_Event_Filter :  public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
  RawHit_SDHCAL_Data_Reader_Event_Filter() : m_keptEvents(0), m_seenEvents(0) {;}
  virtual ~RawHit_SDHCAL_Data_Reader_Event_Filter();
  void addRejectConditions(RawHit_SDHCAL_Data_Event_Filter& cond) {m_RejectConditions.push_back(std::make_pair(std::make_pair(cond.name(),&cond),0));}
  void process(const RawHit_SDHCAL_Data&);
 private:
 int m_keptEvents, m_seenEvents;
 std::vector< std::pair<std::pair<std::string,RawHit_SDHCAL_Data_Event_Filter*>,int> > m_RejectConditions;
};

#endif

