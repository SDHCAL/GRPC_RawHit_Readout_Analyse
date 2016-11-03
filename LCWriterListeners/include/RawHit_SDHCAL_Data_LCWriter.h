#ifndef RawHit_SDHCAL_Data_LCWriter_HH
#define RawHit_SDHCAL_Data_LCWriter_HH



#include "RawHit_SDHCAL_Data_Listener.h"
#include "IO/LCWriter.h"
#include "EVENT/LCParameters.h"
#include "IMPL/LCCollectionVec.h"
#include <string>

class RawHit_SDHCAL_Data_LCWriter : public RawHit_SDHCAL_Data_Listener
{
 public:
  RawHit_SDHCAL_Data_LCWriter(std::string collectionName="DUMMY");
  virtual ~RawHit_SDHCAL_Data_LCWriter() {;}
  void open(std::string fileName) {m_lcWriter->open( fileName );}
  void close() {m_lcWriter->close();}
  void setLCWriter(IO::LCWriter* lcWriter) {m_lcWriter=lcWriter;}
  void process(const RawHit_SDHCAL_Data&);
 private:
  IO::LCWriter* m_lcWriter;
  void copyLCParameters( const EVENT::LCParameters &inputParameters , EVENT::LCParameters &targetParameters ); //copied from Remi's Trivent
  unsigned int m_eventCount;
  std::string m_collectionName;
  
  virtual IMPL::LCCollectionVec* createAndFillCollection(const RawHit_SDHCAL_Data&)=0;
  virtual void finalizeCollection(IMPL::LCCollectionVec*)=0;
};

#endif
