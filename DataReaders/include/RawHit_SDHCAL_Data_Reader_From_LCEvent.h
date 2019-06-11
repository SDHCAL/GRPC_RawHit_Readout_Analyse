#ifndef RawHit_SDHCAL_Data_Reader_From_LCEvent_HH
#define RawHit_SDHCAL_Data_Reader_From_LCEvent_HH

#include "RawHit_SDHCAL_Data_Reader.h"
#include "IO/LCEventListener.h"
#include <string>

class RawHit_SDHCAL_Data;


class RawHit_SDHCAL_Data_Reader_From_LCEvent : public RawHit_SDHCAL_Data_Reader, public IO::LCEventListener
{
 public:
 RawHit_SDHCAL_Data_Reader_From_LCEvent(std::string cname="DHCALRawHits",bool fillSpillInfo=false, float interSpillInSeconds=10)
   : m_nProcessedEvent(0),m_nMissingCollectionEvent(0),m_collectionName(cname),m_fillSpillInfo(fillSpillInfo),m_interSpillInSeconds(interSpillInSeconds){;}
  ~RawHit_SDHCAL_Data_Reader_From_LCEvent();
  void processEvent(EVENT::LCEvent * evt);
  void modifyEvent(EVENT::LCEvent * evt) {;}
 private:
  unsigned int m_nProcessedEvent; 
  unsigned int m_nMissingCollectionEvent; 
  std::string m_collectionName;
  bool m_fillSpillInfo;
  float m_interSpillInSeconds;
  void fillSpillInfo(const RawHit_SDHCAL_Data&);
};

#endif

