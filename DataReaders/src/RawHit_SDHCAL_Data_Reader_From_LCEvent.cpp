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
      RawHit_SDHCAL_Data d(*col, evt->getRunNumber(), evt->getEventNumber(), evt->getTimeStamp());
      notifyListeners(d);
    }
  catch(EVENT::DataNotAvailableException &e)
    {
      ++m_nMissingCollectionEvent;
    }     
}

RawHit_SDHCAL_Data_Reader_From_LCEvent::~RawHit_SDHCAL_Data_Reader_From_LCEvent()
{
  std::cout << "RawHit_SDHCAL_Data_Reader_From_LCEvent has processed " << m_nProcessedEvent << " readouts with " 
	    << m_nMissingCollectionEvent << " without the RawHit collection " << m_collectionName << "." << std::endl;
}
