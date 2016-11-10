#ifndef RawCalorimeterHitPointer_HH
#define RawCalorimeterHitPointer_HH

#include "ASIC_Coordinate_Translator.h"
#include "EVENT/RawCalorimeterHit.h"

#include <ostream>
#include <iomanip>

class RawCalorimeterHitPointer 
{
  EVENT::RawCalorimeterHit* m_ptr;
 public:
  RawCalorimeterHitPointer() : m_ptr(NULL) {;}
  RawCalorimeterHitPointer(EVENT::RawCalorimeterHit* ptr) : m_ptr(ptr) {;}

  //pointer overloading
  EVENT::RawCalorimeterHit* operator->() {return m_ptr;}
  const EVENT::RawCalorimeterHit* operator->() const { return m_ptr;}
  EVENT::RawCalorimeterHit& operator*() {return *m_ptr;}
  const EVENT::RawCalorimeterHit& operator*() const {return *m_ptr;}

  //decode cellId0
  unsigned int dif() const {return (m_ptr->getCellID0())&0xFF;}
  unsigned int asic() const {return ((m_ptr->getCellID0())&0xFF00)>>8;}
  unsigned int channel() const {return ((m_ptr->getCellID0())&0x3F0000)>>16;}

  //decode amplitude
  unsigned int thresholdCrossed() const {return (m_ptr->getAmplitude())&0x3;}
  unsigned int amplitudeInfo() const {return (m_ptr->getAmplitude())>>2;}
  bool aboveSecondThreshold() const {return thresholdCrossed()!=2;}
  bool aboveThirdThreshold() const {return thresholdCrossed()==3;}

  //translate stuff for HR2 pad ASU 
  unsigned int I_local() const {return ASIC_Coordinate_Translator::I(asic(),channel());}
  unsigned int J_local() const {return ASIC_Coordinate_Translator::J(asic(),channel());}

  //translate stuff for HR2 strip ASU 
  unsigned int stripNumberEven() const {return ASIC_Coordinate_Translator::stripNumberEven(channel());} //pair
  unsigned int stripNumberOdd()  const {return ASIC_Coordinate_Translator::stripNumberOdd(channel());;} //impair
};


inline std::ostream& operator<<(std::ostream& outFlux, const RawCalorimeterHitPointer& hit)
{
  outFlux<< "("<<std::setw(4)<<hit.dif()<<"/"<<std::setw(4)<<hit.asic()<<"/"<<std::setw(4)<<hit.channel()<<"):"
	 << std::setw(2) << hit->getAmplitude() << " " << hit->getTimeStamp();
  return outFlux;
}

#endif
