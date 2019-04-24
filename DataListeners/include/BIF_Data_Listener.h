#ifndef BIF_Data_Listener_HH
#define BIF_Data_Listener_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "intervalle.h"

#include <map>
#include <iostream>
#include <set>
#include <string>

class TDirectory;

class BIF_Data_Listener : public RawHit_SDHCAL_Data_Listener
{
public:
  BIF_Data_Listener(unsigned int BIF)
    : m_DIFnumber_of_the_BIF(BIF) {;}
  void process(const RawHit_SDHCAL_Data&);
  void saveTo(TDirectory* d);
  void printMaxDelay(std::ostream& out=std::cout);
 private:
  unsigned int m_DIFnumber_of_the_BIF; 
  std::map<int,int> map_HitBIFDeltaTime_NumberOfHits[3];
  void makeTimeIntervals(std::set<unsigned int>&, std::map<unsigned int,UI_intervalle>&);
  void makeHisto(std::string,std::map<int,int>&);
};

inline bool PairComparisonFunctionOnSecondMember(const std::pair<int,int> &left, const std::pair<int,int> &right)
{
  return left.second < right.second;
}


#endif
