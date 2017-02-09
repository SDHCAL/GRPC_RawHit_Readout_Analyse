#ifndef BifaceStripDevice_HH
#define BifaceStripDevice_HH

#include "DIFdrivenDevice.h"

class BifaceStripDevice : public DIFdrivenDevice
{
 public:
  BifaceStripDevice(unsigned int layer=0) : DIFdrivenDevice(layer) {;}
  BifaceStripDevice(unsigned int EvenStripDIF, unsigned int OddStripDIF,unsigned int layer) : DIFdrivenDevice(layer) 
  {addDIF(EvenStripDIF); addDIF(OddStripDIF);}
  unsigned int maxNumberOfASICs() const {return 2;}
  std::pair<unsigned int, unsigned int> getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const;
  dif_asic_channel getDifAsicChannel(unsigned int Strip, unsigned int Gap) const;
};


#endif
