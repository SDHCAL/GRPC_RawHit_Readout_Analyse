#ifndef TricotDevice_HH
#define TricotDevice_HH

#include "DIFdrivenDevice.h"

class TricotDevice : public DIFdrivenDevice
{
 public:
  TricotDevice(unsigned int layer=0, unsigned int nAngles=3) : DIFdrivenDevice(layer), m_numberOfStripAngles(nAngles) {;}
  TricotDevice(unsigned int DIFnumber,unsigned int layer, unsigned int nAngles=3) : DIFdrivenDevice(layer), m_numberOfStripAngles(nAngles)
  {addDIF(DIFnumber);}
  unsigned int maxNumberOfASICs() const {return m_numberOfStripAngles;}
  std::pair<unsigned int, unsigned int> getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const;
  dif_asic_channel getDifAsicChannel(unsigned int Strip, unsigned int Angle) const;
 private:
  unsigned int m_numberOfStripAngles;
};

#endif
