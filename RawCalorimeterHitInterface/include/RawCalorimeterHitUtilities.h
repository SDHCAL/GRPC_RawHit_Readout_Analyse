#ifndef RawCalorimeterHitUtilities_HH
#define RawCalorimeterHitUtilities_HH

//various function to play with RawCalorimeterHits

#include "IMPL/RawCalorimeterHitImpl.h"

IMPL::RawCalorimeterHitImpl* createRawHit(int amplitude, int timestamp, unsigned int dif, unsigned int asic, unsigned int channel);




#endif
