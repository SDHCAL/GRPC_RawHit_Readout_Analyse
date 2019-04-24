#include "BIF_Data_Listener.h"

#include "RawCalorimeterHitPointerTools.h"
#include "RawHit_SDHCAL_Data.h"

#include <limits>
#include <algorithm>

#include "TDirectory.h"
#include "TH1I.h"

void BIF_Data_Listener::process(const RawHit_SDHCAL_Data& d)
{
  std::vector<RawCalorimeterHitPointer> BIF_hitvector=extract(d.getHitVector(),m_DIFnumber_of_the_BIF,rawHit_DIF());
  if (BIF_hitvector.empty()) return;
  std::set<unsigned int> BIFtimes[3];
  for (std::vector<RawCalorimeterHitPointer>::iterator it=BIF_hitvector.begin(); it != BIF_hitvector.end(); ++it)
    BIFtimes[(it->thresholdCrossed())-1].insert((*it)->getTimeStamp());
  std::map<unsigned int,UI_intervalle> timeIntervals[3];
  for (unsigned int ithresh=0; ithresh<3; ++ithresh)
    {
      makeTimeIntervals(BIFtimes[ithresh],timeIntervals[ithresh]); //Warning BIFtimes is changed
      for (std::map<unsigned int,UI_intervalle>::iterator itIntervalle=timeIntervals[ithresh].begin(); itIntervalle!= timeIntervals[ithresh].end(); ++itIntervalle)
	{
	  std::vector<RawCalorimeterHitPointer> BIFevent=extract(d.getHitVector(),itIntervalle->second,rawHit_TimeStamp());
	  for (std::vector<RawCalorimeterHitPointer>::iterator itHit=BIFevent.begin(); itHit!=BIFevent.end(); ++itHit)
	    map_HitBIFDeltaTime_NumberOfHits[ithresh][((int) (*itHit)->getTimeStamp()) - ((int) itIntervalle->first)]++;
	}
    }
}


void BIF_Data_Listener::saveTo(TDirectory* d)
{
  if (NULL==d) return;
  d->cd();

  char c='1';
  for (unsigned int ithresh=0; ithresh<3; ++ithresh)
    makeHisto(std::string(1,char(c+ithresh)),map_HitBIFDeltaTime_NumberOfHits[ithresh]);
  //make gloabal map
  std::map<int,int> map_all=map_HitBIFDeltaTime_NumberOfHits[0];
  for (int ithresh=1; ithresh<3; ++ithresh)
    for (std::map<int,int>::iterator it=map_HitBIFDeltaTime_NumberOfHits[ithresh].begin(); it!=map_HitBIFDeltaTime_NumberOfHits[ithresh].end(); ++it)
      map_all[it->first]+=it->second;
  makeHisto(std::string("all"),map_all);
}

void  BIF_Data_Listener::makeHisto(std::string label,std::map<int,int>& amap)
{
  if (amap.empty()) return;
  int minbinval=amap.begin()->first;
  int maxbinval=amap.rbegin()->first+1;
  int nbins=maxbinval-minbinval;
  int maxval=std::max_element(amap.begin(),amap.end(),PairComparisonFunctionOnSecondMember)->first;
  TH1I *h_all=new TH1I((std::string("h_BIFtimeShift_thesh_")+label).c_str(),(std::string("Difference between hit time and BIF time for threshold ")+label).c_str(),nbins,minbinval,maxbinval);
  TH1I *h_zoom=new TH1I((std::string("h_BIFtimeShift_zoom_thesh_")+label).c_str(),(std::string("Difference between hit time and BIF time for threshold ")+label).c_str(),100,maxval-50,maxval+50);
  for (std::map<int,int>::iterator it=amap.begin(); it!=amap.end();++it)
    {
      h_all->Fill(it->first,it->second);
      h_zoom->Fill(it->first,it->second);
    }
  h_all->Write();
  h_zoom->Write();
}

void BIF_Data_Listener::printMaxDelay(std::ostream& out)
{
  for (unsigned int ithresh=0; ithresh<3; ++ithresh)
    {
      if (map_HitBIFDeltaTime_NumberOfHits[ithresh].empty())
	{
	  out << "threshold " << ithresh+1 << " : empty" << std::endl;
	}
      else
	{
	  out << "threshold " << ithresh+1 << " : timeBIF difference range from "
	      << map_HitBIFDeltaTime_NumberOfHits[ithresh].begin()->first << " to "
	      << map_HitBIFDeltaTime_NumberOfHits[ithresh].rbegin()->first << " and max is at "
	      << std::max_element(map_HitBIFDeltaTime_NumberOfHits[ithresh].begin(),map_HitBIFDeltaTime_NumberOfHits[ithresh].end(),PairComparisonFunctionOnSecondMember)->first << std::endl;
	}
    }
}

void BIF_Data_Listener::makeTimeIntervals(std::set<unsigned int>& BIFtimes,std::map<unsigned int,UI_intervalle>& timeIntervals)
{
  timeIntervals.clear();
  if (BIFtimes.empty()) return;
  bool firstTimeIsZero=( (*(BIFtimes.begin()))==0);
  BIFtimes.insert(0);
  BIFtimes.insert(std::numeric_limits<unsigned int>::max());
  std::set<unsigned int>::iterator iterTimesLow=BIFtimes.begin();
  std::set<unsigned int>::iterator iterTimesCenter=iterTimesLow; ++iterTimesCenter;
  std::set<unsigned int>::iterator iterTimesUp=iterTimesCenter; ++iterTimesUp;
  if (firstTimeIsZero)
    {
       timeIntervals[*iterTimesLow]=UI_intervalle(*iterTimesLow,*iterTimesCenter);
       if (BIFtimes.size()==2) return; //There was only one value of zero in the BIFtimes set
    }
  do
    {
      timeIntervals[*iterTimesCenter]=UI_intervalle(*iterTimesLow,*iterTimesUp);
      ++iterTimesLow; ++iterTimesCenter; ++iterTimesUp;
    } while (iterTimesUp != BIFtimes.end());
}
