#ifndef RawHit_horizontal_track_predicate_HH
#define RawHit_horizontal_track_predicate_HH

#include "ExperimentalSetup.h"
#include "RawCalorimeterHitPointer.h"

class RawHit_horizontal_track_predicate
{
 public:
 RawHit_horizontal_track_predicate(ExperimentalSetup& setup) : m_setup(setup), m_neighbourTimeDistance(1), m_neighbourPadDistance(1) {}
  unsigned int getNeighbourTimeDistance() const {return m_neighbourTimeDistance;}
  unsigned int getNeighbourPadDistance() const {return m_neighbourPadDistance;}

  void setNeighbourTimeDistance(unsigned int val) {m_neighbourTimeDistance=val;}
  void setNeighbourPadDistance(unsigned int val) {m_neighbourPadDistance=val;}

  bool operator()(const RawCalorimeterHitPointer &A,const RawCalorimeterHitPointer &B);

 private:
  ExperimentalSetup &m_setup; 
  unsigned int m_neighbourTimeDistance;
  unsigned int m_neighbourPadDistance;
  bool m_padJunctionByCorner;
};

#endif
