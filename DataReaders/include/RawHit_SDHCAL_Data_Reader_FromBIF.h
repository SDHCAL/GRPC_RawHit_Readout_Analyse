#ifndef RawHit_SDHCAL_Data_Reader_FromBIF_HH
#define RawHit_SDHCAL_Data_Reader_FromBIF_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "GG_messageCounter.h"
#include "intervalle.h"

class RawHit_SDHCAL_Data_Reader_FromBIF : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
  RawHit_SDHCAL_Data_Reader_FromBIF(unsigned int BIF,intervalle<int> timeWindow) 
    : m_DIFnumber_of_the_BIF(BIF), m_timeWindowFromBIF(timeWindow),  m_nBIFseen(0) {;}
  RawHit_SDHCAL_Data_Reader_FromBIF(unsigned int BIF,UI_intervalle timeWindow) 
    : m_DIFnumber_of_the_BIF(BIF), m_timeWindowFromBIF(intervalle<int>(timeWindow.first,timeWindow.second)),  m_nBIFseen(0) {;}
  ~RawHit_SDHCAL_Data_Reader_FromBIF();
  void process(const RawHit_SDHCAL_Data&);

  GG_messageCounter outOfTimeReadout=GG_messageCounter("WARNING : RawHit_SDHCAL_Data_Reader_fromBIF : requested intervalle after readout"); //C++11

 private:
  unsigned int m_DIFnumber_of_the_BIF; 
  intervalle<int> m_timeWindowFromBIF;
  unsigned int m_nBIFseen;
};

#endif


