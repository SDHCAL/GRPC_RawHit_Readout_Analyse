#ifndef RawHit_SDHCAL_Data_Reader_From_LCEvent_HH
#define RawHit_SDHCAL_Data_Reader_From_LCEvent_HH

#include "RawHit_SDHCAL_Data_Reader.h"
#include "IO/LCEventListener.h"

#include <string>

class RawHit_SDHCAL_Data_Reader_From_LCEvent : public RawHit_SDHCAL_Data_Reader, public IO::LCEventListener
{
 public:
  RawHit_SDHCAL_Data_Reader_From_LCEvent(std::string cname="DHCALRawHits")
    : m_nProcessedEvent(0),m_collectionName(cname){;}
  ~RawHit_SDHCAL_Data_Reader_From_LCEvent();
  void processEvent(EVENT::LCEvent * evt);
  void modifyEvent(EVENT::LCEvent * evt) {;}
 private:
  unsigned int m_nProcessedEvent; 
  std::string m_collectionName;
};

#endif

