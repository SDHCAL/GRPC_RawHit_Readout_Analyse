#ifndef TricotDevice_HH
#define TricotDevice_HH

#include "DIFdrivenDevice.h"
#include <map>

class RawCalorimeterHitPointer;

class TricotDevice : public DIFdrivenDevice
{
 public:
 TricotDevice(unsigned int layer=0) : DIFdrivenDevice(layer), m_numberOfStripAngles(3), m_hitsPosition() {;}
  TricotDevice(unsigned int DIFnumber,unsigned int layer, unsigned int nAngles=3) : DIFdrivenDevice(layer), m_numberOfStripAngles(nAngles)
  {addDIF(DIFnumber);}
  unsigned int maxNumberOfASICs() const {return m_numberOfStripAngles;}
  std::pair<unsigned int, unsigned int> getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const;
  dif_asic_channel getDifAsicChannel(unsigned int Strip, unsigned int Angle) const;
  void computeHitPosition(const std::vector<RawCalorimeterHitPointer>&);
  bool planPositionIsImplemented() const {return true;}
  std::pair<float,float> getPositionInPlanIn_mm(unsigned int IorStrip, unsigned int JorAsic) const {return m_hitsPosition.at(map_index(IorStrip,JorAsic));}
 private:
  unsigned int m_numberOfStripAngles;
  std::map<unsigned int, std::pair<float,float> > m_hitsPosition;
  inline unsigned int map_index(unsigned int IorStrip, unsigned int JorAsic) const {return 100*IorStrip+JorAsic;}
};

#endif
