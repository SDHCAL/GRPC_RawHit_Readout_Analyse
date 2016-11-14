#ifndef RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016_HH
#define RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"
#include "domain.h"
#include "ConcreteExperimentalSetup.h"

#include "GG_counter.h"
class TDirectory;

class RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016 : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
  RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016();
  void process(const RawHit_SDHCAL_Data&);
 private:
  unsigned int m_DIFnumber_of_the_BIF; 
  UI_domain m_padDIFs, m_stripEvenDifs, m_stripOddDifs;

  GIF_oct2016_ExperimentalSetup m_setup;   
};

#endif


