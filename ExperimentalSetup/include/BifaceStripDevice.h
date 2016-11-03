#ifndef BifaceStripDevice_HH
#define BifaceStripDevice_HH

#include "DIFdrivenDevice.h"

class BifaceStripDevice : public DIFdrivenDevice
{
 public:
  BifaceStripDevice(unsigned int layer=0) : DIFdrivenDevice(layer) {;}
  BifaceStripDevice(unsigned int EvenStripDIF, unsigned int OddStripDIF,unsigned int layer) : DIFdrivenDevice(layer) 
  {addDIF(EvenStripDIF); addDIF(OddStripDIF);}
  std::pair<unsigned int, unsigned int> getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const;
};


#endif
