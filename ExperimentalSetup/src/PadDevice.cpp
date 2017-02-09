#include "PadDevice.h"

#include "ASIC_Coordinate_Translator.h"

std::pair<unsigned int, unsigned int> PadDevice::getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const
  {
    //it is assumed that the 3 DIFs of a plan are registered in the order J=0, J=32, J=64
    unsigned int DIF_Index;
    for (DIF_Index=0; DIF_Index<m_DIFnumbers.size(); ++DIF_Index)
      if (m_DIFnumbers[DIF_Index]==dif) break;
    if (DIF_Index==m_DIFnumbers.size()) return std::make_pair(1200u,1200u);
    return std::make_pair(ASIC_Coordinate_Translator::I(asic,channel),ASIC_Coordinate_Translator::J(asic,channel)+32*DIF_Index);
  }

DIFdrivenDevice::dif_asic_channel PadDevice::getDifAsicChannel(unsigned int I, unsigned int J) const
{
  unsigned int Jlocal=(J-1)%32+1;
  return dif_asic_channel(m_DIFnumbers[(J-1)/32],ASIC_Coordinate_Translator::asic(I,Jlocal),ASIC_Coordinate_Translator::channel(I,Jlocal));
}
