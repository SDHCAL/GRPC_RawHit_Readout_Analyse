#include "RawHit_standard_merge_predicate.h"

bool RawHit_standard_merge_predicate::operator()(const RawCalorimeterHitPointer &A,const RawCalorimeterHitPointer &B)
{
  if (abs(A->getTimeStamp()-B->getTimeStamp())>m_neighbourTimeDistance) return false;
  if (m_setup.DIFnumberIsBIF(A.dif()) && m_setup.DIFnumberIsBIF(B.dif()) ) return true;
  if (m_setup.DIFnumberIsBIF(A.dif()) || m_setup.DIFnumberIsBIF(B.dif()) ) return false;
  if (m_setup.getPlanNumber(A.dif()) != m_setup.getPlanNumber(B.dif()) ) return false; 
  if (m_setup.DIFnumberIsPad(A.dif()))
    {
      unsigned int I1,I2,J1,J2,K;
      m_setup.getCoord3D(A.dif(),A.asic(),A.channel(),I1,J1,K);
      m_setup.getCoord3D(B.dif(),B.asic(),B.channel(),I2,J2,K);
      return IJ_connect(I1,J1,I2,J2);
    }
  if (m_setup.DIFnumberIsStrip(A.dif()))
    {
      if (A.asic() != B.asic()) return false;
      unsigned int strip1, strip2, gap, K;
      m_setup.getCoord3D(A.dif(),A.asic(),A.channel(),strip1,gap,K);
      m_setup.getCoord3D(B.dif(),A.asic(),A.channel(),strip2,gap,K);
      return abs(int(strip1)-int(strip2)) <= m_neighbourStripDistance;
    }
  return A.dif()==B.dif() && A.asic()==B.asic() && A.channel()==B.channel();
}

bool RawHit_standard_merge_predicate::IJ_connect(int I1, int J1, int I2, int J2)
{
  unsigned int distI=abs(I1-I2);
  unsigned int distJ=abs(J1-J2);
  if (m_padJunctionByCorner) return distI <= m_neighbourPadDistance && distJ <= m_neighbourPadDistance;
  else return distI+distJ <= m_neighbourPadDistance;
}
