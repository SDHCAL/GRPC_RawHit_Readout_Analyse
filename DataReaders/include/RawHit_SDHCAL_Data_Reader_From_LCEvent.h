#ifndef RawHit_SDHCAL_Data_Reader_From_LCEvent_HH
#define RawHit_SDHCAL_Data_Reader_From_LCEvent_HH

#include "RawHit_SDHCAL_Data_Reader.h"
#include "IO/LCEventListener.h"
#include <string>

#include "GG_counter.h"
#include "GG_messageCounter.h"

class RawHit_SDHCAL_Data;


class RawHit_SDHCAL_Data_Reader_From_LCEvent : public RawHit_SDHCAL_Data_Reader, public IO::LCEventListener
{
 public:
 RawHit_SDHCAL_Data_Reader_From_LCEvent(std::string cname="DHCALRawHits",bool fillSpillInfo=false, float interSpillInSeconds=10)
   : m_nProcessedEvent(0),m_nMissingCollectionEvent(0),m_collectionName(cname),m_fillSpillInfo(fillSpillInfo),m_interSpillInSeconds(interSpillInSeconds){;}
  ~RawHit_SDHCAL_Data_Reader_From_LCEvent();
  void processEvent(EVENT::LCEvent * evt);
  void modifyEvent(EVENT::LCEvent * evt) {;}

  void writeSpillInfoStatShort(std::ostream& oflux=std::cout);
  void writeSpillInfoStatProblemOnly(std::ostream& oflux=std::cout);
  void writeSpillInfoStatExtended(std::ostream& oflux=std::cout);
  void writeSpillInfoStatExtendedInFile(std::string filename="SpillInfoStatOutput.txt");
  
  
  GG_messageCounter negativeTimeStamp=GG_messageCounter("Warning RawHit_SDHCAL_Data_Reader_From_LCEvent negative time stamp seen in event");
  GG_messageCounter highValueTimeStamp=GG_messageCounter("Warning RawHit_SDHCAL_Data_Reader_From_LCEvent time stamp greater than absoluteBCID seen in event");
  GG_messageCounter moreThanOneSpillInReadout=GG_messageCounter("WARNING the readout contains more than one beam spill");
 private:
  unsigned int m_nProcessedEvent; 
  unsigned int m_nMissingCollectionEvent; 
  std::string m_collectionName;
  bool m_fillSpillInfo;
  float m_interSpillInSeconds;
  void fillSpillInfo(const RawHit_SDHCAL_Data&);
  //fillSpillInfo stats
  DIFCounters m_negativeTimeStamp_counter;
  DIFCounters m_highValueTimeStamp_counter;
  DIFCounters m_startSpillUsedTimeStamp_counter;
  std::string m_fillInfo_counters_labels[4]={"Setup","DIF","ASIC","Channel"};
};

#endif

