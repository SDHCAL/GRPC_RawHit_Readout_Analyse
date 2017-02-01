#include "RawCalorimeterHitClustering.h"


unsigned int mostFrequentValue(std::map<unsigned int, unsigned int> &m)
{
  if (m.empty()) return 0;
  std::map<unsigned int, unsigned int>::iterator itmax=m.begin();
  for (std::map<unsigned int, unsigned int>::iterator it=m.begin(); it!= m.end(); ++it)
    if (it->second > itmax->second)
      itmax=it;
  return itmax->first;
}
