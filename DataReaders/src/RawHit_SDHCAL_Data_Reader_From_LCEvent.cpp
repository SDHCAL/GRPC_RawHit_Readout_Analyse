#include "RawHit_SDHCAL_Data_Reader_From_LCEvent.h"


#include "EVENT/LCEvent.h"
#include "IMPL/LCCollectionVec.h"
#include "RawHit_SDHCAL_Data.h"

void RawHit_SDHCAL_Data_Reader_From_LCEvent::processEvent(EVENT::LCEvent * evt)
{
  ++m_nProcessedEvent;
  if (0==(m_nProcessedEvent%1000)) 
    {std::cout << "have processed " << m_nProcessedEvent << " readouts\r"; std::cout.flush();} // \r = return to line start  
  //std::cout << "RawHit_SDHCAL_Data_Reader_From_LCEvent got called " << m_nProcessedEvent << std::endl;
  try
    {
      IMPL::LCCollectionVec* col = (IMPL::LCCollectionVec*) evt->getCollection(m_collectionName);
      if (NULL==col) std::cout << "ERROR collection " << m_collectionName << " not found" << std::endl; //should never happened
      RawHit_SDHCAL_Data d(*col, evt->getRunNumber(), evt->getEventNumber(), evt->getTimeStamp()); //this constructor fills the RawHit_SDHCAL_Data DIFtimeInfo
      if (m_fillSpillInfo) fillSpillInfo(d);
      notifyListeners(d);
    }
  catch(EVENT::DataNotAvailableException &e)
    {
      ++m_nMissingCollectionEvent;
    }     
}

void RawHit_SDHCAL_Data_Reader_From_LCEvent::fillSpillInfo(const RawHit_SDHCAL_Data& d)
{
  std::map<unsigned int,DIF_timeInfo> diftime=d.DIFtimeInfo();
  if (diftime.empty()) return;
  uint64_t AbsoluteBCID=diftime.begin()->second.AbsBCID;
  bool keepSpillInfo=RawHit_SDHCAL_Data::m_spill.filled() && (AbsoluteBCID-RawHit_SDHCAL_Data::m_spill.ref_absBCID)*200e-9<m_interSpillInSeconds;
  if (keepSpillInfo) return;
  ++RawHit_SDHCAL_Data::m_spill.spillNumber;
  RawHit_SDHCAL_Data::m_spill.ref_absBCID=AbsoluteBCID;
  uint64_t maxTimeStamp=0;
  for (std::vector<RawCalorimeterHitPointer>::const_iterator it=d.getHitVector().begin(); it != d.getHitVector().end(); ++it)
    {
      int timeStamp=(*it)->getTimeStamp();
      if (timeStamp<0) {negativeTimeStamp.print();continue;}
      unsigned int ts=timeStamp;
      if (ts>AbsoluteBCID) {highValueTimeStamp.print();continue;}
      if (ts>maxTimeStamp) maxTimeStamp=ts;
    }
  RawHit_SDHCAL_Data::m_spill.max_readoutTimeStamp=maxTimeStamp;
  RawHit_SDHCAL_Data::m_spill.startSpill_BCID=AbsoluteBCID-maxTimeStamp;
  //std::cout << " New spill info filled " << std::endl;
}

RawHit_SDHCAL_Data_Reader_From_LCEvent::~RawHit_SDHCAL_Data_Reader_From_LCEvent()
{
  std::cout << "RawHit_SDHCAL_Data_Reader_From_LCEvent has processed " << m_nProcessedEvent << " readouts with " 
	    << m_nMissingCollectionEvent << " without the RawHit collection " << m_collectionName << "." << std::endl;
}
