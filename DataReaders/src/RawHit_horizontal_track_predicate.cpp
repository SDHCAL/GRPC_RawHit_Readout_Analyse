#include "RawHit_horizontal_track_predicate.h"

#include <iostream>


bool RawHit_horizontal_track_predicate::operator()(const RawCalorimeterHitPointer &A,const RawCalorimeterHitPointer &B)
{
  if (abs(A->getTimeStamp()-B->getTimeStamp())>m_neighbourTimeDistance) return false;
  PadDevice* padA=m_setup.getPadDevice(A.dif());
  PadDevice* padB=m_setup.getPadDevice(B.dif());
  if (padA==NULL || padB==NULL) return false;
  std::pair<unsigned int, unsigned int> IJA=padA->getIJ(A.dif(),A.asic(),A.channel());
  std::pair<unsigned int, unsigned int> IJB=padB->getIJ(B.dif(),B.asic(),B.channel());
  return abs(int(IJA.first)-int(IJB.first))<m_neighbourPadDistance && abs(int(IJA.second)-int(IJB.second))<m_neighbourPadDistance;
}
