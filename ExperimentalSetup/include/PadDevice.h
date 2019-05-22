#ifndef PadDevice_HH
#define PadDevice_HH

#include "DIFdrivenDevice.h"

class PadDevice : public DIFdrivenDevice
{
 public:
  PadDevice(unsigned int layer=0) : DIFdrivenDevice(layer) {;}
  PadDevice(unsigned int singleDIF,unsigned int layer) : DIFdrivenDevice(layer) {addDIF(singleDIF);}
  PadDevice(unsigned int J0_DIF,unsigned int J32_DIF,unsigned int J64_DIF,unsigned int layer) : DIFdrivenDevice(layer) 
  {addDIF(J0_DIF); addDIF(J32_DIF); addDIF(J64_DIF);}
  unsigned int maxNumberOfASICs() const {return 48;}
  std::pair<unsigned int, unsigned int> getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const;
  dif_asic_channel getDifAsicChannel(unsigned int I, unsigned int J) const;
  bool planPositionIsImplemented() const {return true;}
  std::pair<double,double> getPositionInPlanIn_mm(unsigned int I, unsigned int J, std::vector<RawCalorimeterHitPointer>& hits) const  {return std::make_pair(I*10.408f,J*10.408f);}
};

#endif
