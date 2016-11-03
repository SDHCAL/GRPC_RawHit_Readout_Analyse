#include "RawCalorimeterHitUtilities.h"

IMPL::RawCalorimeterHitImpl* createRawHit(int amplitude, int timestamp, unsigned int dif, unsigned int asic, unsigned int channel)
{
  IMPL::RawCalorimeterHitImpl* r=new IMPL::RawCalorimeterHitImpl();
  r->setAmplitude(amplitude);
  r->setTimeStamp(timestamp);
  unsigned int id=((channel&0x3F)<<16)|((asic&0xFF)<<8)|(dif&0xFF);
  r->setCellID0(id);
  return r;
}

