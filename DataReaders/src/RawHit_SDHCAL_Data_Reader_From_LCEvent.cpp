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
  unsigned int keptKeys[3]={0,0,0};
  for (std::vector<RawCalorimeterHitPointer>::const_iterator it=d.getHitVector().begin(); it != d.getHitVector().end(); ++it)
    {
      unsigned int keys[3]={it->dif(),it->asic(),it->channel()};
      int timeStamp=(*it)->getTimeStamp();
      if (timeStamp<0) {negativeTimeStamp.print(); m_negativeTimeStamp_counter.add(1,keys);continue;}
      unsigned int ts=timeStamp;
      if (ts>AbsoluteBCID) {highValueTimeStamp.print(); m_highValueTimeStamp_counter.add(1,keys);continue;}
      if (ts>maxTimeStamp) {maxTimeStamp=ts; for (int i=0; i<3; ++i) keptKeys[i]=keys[i];}
    }
  m_startSpillUsedTimeStamp_counter.add(1,keptKeys);
  RawHit_SDHCAL_Data::m_spill.max_readoutTimeStamp=maxTimeStamp;
  RawHit_SDHCAL_Data::m_spill.startSpill_BCID=AbsoluteBCID-maxTimeStamp;
  if (maxTimeStamp*200e-9>m_interSpillInSeconds) moreThanOneSpillInReadout.print();
  //std::cout << " New spill info filled " << std::endl;
  m_negativeTimeStamp_counter.newSet();
  m_highValueTimeStamp_counter.newSet();
  m_startSpillUsedTimeStamp_counter.newSet();
}

void RawHit_SDHCAL_Data_Reader_From_LCEvent::writeSpillInfoStatProblemOnly(std::ostream& oflux)
{
  oflux << "RawHit_SDHCAL_Data_Reader_From_LCEvent spill info stats reports" << std::endl;
  oflux << "    negative time stamp found in readout used to compute spill start time " << std::endl;
  m_negativeTimeStamp_counter.write(m_fillInfo_counters_labels,oflux);
  oflux << "    time stamp found bigger than Absolute BCID" << std::endl;
  m_highValueTimeStamp_counter.write(m_fillInfo_counters_labels,oflux);
}

void RawHit_SDHCAL_Data_Reader_From_LCEvent::writeSpillInfoStatExtended(std::ostream& oflux)
{
  writeSpillInfoStatProblemOnly(oflux);
  oflux << "    time stamps used to set the spill start time" <<std::endl;
  m_startSpillUsedTimeStamp_counter.write(m_fillInfo_counters_labels,oflux);
}

void RawHit_SDHCAL_Data_Reader_From_LCEvent::writeSpillInfoStatExtendedInFile(std::string filename)
{
  std::ofstream f(filename);
  writeSpillInfoStatExtended(f);
}

void RawHit_SDHCAL_Data_Reader_From_LCEvent::writeSpillInfoStatShort(std::ostream& oflux)
{
  writeSpillInfoStatProblemOnly(oflux);
  std::set<unsigned int> DIF_with_problems;
  for (auto &m : m_negativeTimeStamp_counter) DIF_with_problems.insert(m.first);
  for (auto &m : m_highValueTimeStamp_counter) DIF_with_problems.insert(m.first);
  unsigned int number_of_DIF_with_problems_used=0;
  for (auto &dif : DIF_with_problems)
    {
      auto it=m_startSpillUsedTimeStamp_counter.find(dif);
	if ( it != m_startSpillUsedTimeStamp_counter.end())
	  {
	    ++number_of_DIF_with_problems_used;
	    oflux << "    DIF " << dif << " used for spill time set : " << std::endl;
	    m_startSpillUsedTimeStamp_counter[dif].write(m_fillInfo_counters_labels+1,oflux);
	  }
    }
  if (0==number_of_DIF_with_problems_used)
    oflux << "  No DIF with timing problem has been used to determine spill start time." << std::endl;  
}

RawHit_SDHCAL_Data_Reader_From_LCEvent::~RawHit_SDHCAL_Data_Reader_From_LCEvent()
{
  std::cout << "RawHit_SDHCAL_Data_Reader_From_LCEvent has processed " << m_nProcessedEvent << " readouts with " 
	    << m_nMissingCollectionEvent << " without the RawHit collection " << m_collectionName << "." << std::endl;
}
