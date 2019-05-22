#include "RawHit_SDHCAL_Data_LCWriter.h"


#include "IOIMPL/LCFactory.h"
#include "IMPL/LCEventImpl.h"
#include "RawHit_SDHCAL_Data.h"
#include "RawHit_SDHCAL_Data.h"
#include <algorithm>

RawHit_SDHCAL_Data_LCWriter::RawHit_SDHCAL_Data_LCWriter(std::string collectionName) : m_eventCount(0), m_collectionName(collectionName)
{
  m_lcWriter=IOIMPL::LCFactory::getInstance()->createLCWriter();
}

void RawHit_SDHCAL_Data_LCWriter::process(const RawHit_SDHCAL_Data& d)
{
  ++m_eventCount;
  IMPL::LCEventImpl*  evt = new IMPL::LCEventImpl() ;
  evt->setRunNumber(d.getRunNumber());
  evt->setEventNumber(m_eventCount);
  evt->setTimeStamp(d.getEventTimeStamp());

  IMPL::LCCollectionVec* col=createAndFillCollection(d);
  setCollectionParameters(col,d);
  evt->addCollection(col,m_collectionName);

  for (auto lcw : m_otherLCWriters)
    {
       col=lcw->createAndFillCollection(d);
       setCollectionParameters(col,d);
       evt->addCollection(col,lcw->getCollectionName());
    }
  
  m_lcWriter->writeEvent( evt ) ;
  delete evt;
}

void RawHit_SDHCAL_Data_LCWriter::setCollectionParameters(IMPL::LCCollectionVec* col,const RawHit_SDHCAL_Data& d)
{
  copyLCParameters(*(d.getCollectionParameters()),col->parameters());
  setEventtNumbersHistoryKey(col,d.getEventNumber());
  finalizeCollection(col);
}

void RawHit_SDHCAL_Data_LCWriter::setEventtNumbersHistoryKey(IMPL::LCCollectionVec* col,int eventNumber)
{
  EVENT::IntVec EventNumbersHistory;
  std::string EventNumbersHistoryKey="Readout_Event_Number_History";
  EVENT::StringVec intKeys;
  col->parameters().getIntKeys( intKeys );
  if ( std::find(intKeys.begin(), intKeys.end(), EventNumbersHistoryKey) !=  intKeys.end()) col->parameters().getIntVals(EventNumbersHistoryKey, EventNumbersHistory);
  EventNumbersHistory.push_back(eventNumber);
  col->parameters().setValues(EventNumbersHistoryKey,EventNumbersHistory);
}

void RawHit_SDHCAL_Data_LCWriter::copyLCParameters( const EVENT::LCParameters &inputParameters , EVENT::LCParameters &targetParameters )
{
  	EVENT::StringVec intKeys, floatKeys, stringKeys;

	inputParameters.getIntKeys( intKeys );
	inputParameters.getFloatKeys( floatKeys );
	inputParameters.getStringKeys( stringKeys );

	for(unsigned int i=0 ; i<intKeys.size() ; i++)
	{
		EVENT::IntVec dummyInts;
		targetParameters.setValues( intKeys.at(i), inputParameters.getIntVals( intKeys.at(i) , dummyInts ) );
	}

	for(unsigned int i=0 ; i<floatKeys.size() ; i++)
	{
		EVENT::FloatVec dummyFloats;
		targetParameters.setValues( floatKeys.at(i), inputParameters.getFloatVals( floatKeys.at(i) , dummyFloats ) );
	}

	for(unsigned int i=0 ; i<stringKeys.size() ; i++)
	{
		EVENT::StringVec dummyStrings;
		targetParameters.setValues( stringKeys.at(i), inputParameters.getStringVals( stringKeys.at(i) , dummyStrings ) );
	}

}

