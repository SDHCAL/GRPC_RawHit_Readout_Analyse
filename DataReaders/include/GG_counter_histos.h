#ifndef GG_counter_histos_HH
#define GG_counter_histos_HH

#include"GG_counter.h"
#include<string>
#include<utility>
#include <sstream>
#include <algorithm>

//class TH1F;
//class TH2F;

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include "intervalle.h"

//pair::first is histogram of SingleCounter::sumCount and pair::second is histogram of SingleCounter::flagCount
template <class T> 
std::pair<TH1F*,TH1F*>  convert(MappedCounters<T>& ChannelCounters,std::string hNameBase, std::string MappedCountersName, float scale, bool secondHist=true)
{
  std::pair<TH1F*,TH1F*> toReturn(NULL,NULL);
  if (ChannelCounters.empty()) return toReturn;
  intervalle<unsigned int> range(ChannelCounters.begin()->first,ChannelCounters.rbegin()->first);
  toReturn.first = new TH1F((hNameBase+"_sum").c_str(), "", range.length()+1, range.first, range.second+1);
  toReturn.first->GetXaxis()->SetTitle((MappedCountersName+" number").c_str());
  toReturn.first->GetYaxis()->SetTitle((std::string("mean number of hits per ")+MappedCountersName+" per data unit").c_str());
  if (secondHist)
    { 
      toReturn.second = new TH1F((hNameBase+"_freq").c_str(),"", range.length()+1, range.first, range.second+1);
      toReturn.second->GetXaxis()->SetTitle((MappedCountersName+" number").c_str());
      toReturn.second->GetYaxis()->SetTitle((std::string("Probability of hit occurence per ")+MappedCountersName+" per data unit").c_str());
    }

  for(typename std::map<unsigned int,T>::iterator it=ChannelCounters.begin(); it!=ChannelCounters.end(); it++) 
    {
      toReturn.first->Fill(it->first,it->second.sumcount()*scale);
      if (secondHist) toReturn.second->Fill(it->first,it->second.flagcount()*scale);
    }
  return toReturn;
}


//pair::first is histogram of SingleCounter::sumCount and pair::second is histogram of SingleCounter::flagCount
template <class T>
std::pair<TH2F*,TH2F*>  convert(MappedCounters<MappedCounters<T> >& ChannelCounters,std::string hNameBase, std::string MappedCountersNameX, std::string MappedCountersNameY, float scale, std::string InsteadOfYInTitle="", bool secondHist=true)
{
  std::pair<TH2F*,TH2F*> toReturn(NULL,NULL);
  if (ChannelCounters.empty()) return toReturn;
  intervalle<unsigned int> Xrange(ChannelCounters.begin()->first,ChannelCounters.rbegin()->first);

  intervalle<unsigned int> Yrange(ChannelCounters.begin()->second.begin()->first,ChannelCounters.begin()->second.rbegin()->first);
  for (typename std::map<unsigned int, MappedCounters<T> >::iterator itX=ChannelCounters.begin(); itX!=ChannelCounters.end(); ++itX)
    Yrange.expand(itX->second.begin()->first,itX->second.rbegin()->first);

  std::string forTitle=InsteadOfYInTitle;
  if (forTitle.empty()) forTitle=MappedCountersNameY;

  toReturn.first = new TH2F((hNameBase+"_sum").c_str(), "", Xrange.length()+1, Xrange.first, Xrange.second+1, Yrange.length()+1, Yrange.first, Yrange.second+1);
  toReturn.first->GetXaxis()->SetTitle((MappedCountersNameX+" number").c_str());
  toReturn.first->GetYaxis()->SetTitle((MappedCountersNameY+" number").c_str());
  toReturn.first->SetTitle((std::string("mean number of hits per ")+forTitle+" per data unit").c_str());
  if (secondHist)
    {
      toReturn.second = new TH2F((hNameBase+"_freq").c_str(), "", Xrange.length()+1, Xrange.first, Xrange.second+1, Yrange.length()+1, Yrange.first, Yrange.second+1);
      toReturn.second->GetXaxis()->SetTitle((MappedCountersNameX+" number").c_str());
      toReturn.second->GetYaxis()->SetTitle((MappedCountersNameY+" number").c_str());
      toReturn.second->SetTitle((std::string("Probability of hit occurence per ")+forTitle+" per data unit").c_str());
    }

  for (typename std::map<unsigned int, MappedCounters<T> >::iterator itX=ChannelCounters.begin(); itX!=ChannelCounters.end(); ++itX)
    for (typename std::map<unsigned int, T>::iterator itY=itX->second.begin(); itY!=itX->second.end(); ++itY)
      {
	toReturn.first->Fill(itX->first,itY->first,itY->second.sumcount()*scale);
	if (secondHist) toReturn.second->Fill(itX->first,itY->first,itY->second.flagcount()*scale);
      }
  
  return toReturn;
}

std::pair<TH2F*,TH2F*>* convert(MappedCounters<MappedCounters<MappedCounters<SingleCounter> > >& DifAsicChannelCounters,std::string hNameBase, std::string MappedCounterHistoName, std::string MappedCountersNameX, std::string MappedCountersNameY, float scale , std::string InsteadOfYInTitle="", bool secondHist=true);

//histos from counters with BASECOUNTER=SingleMapCounter
struct OneLevelMappedSingleMapCounterHistos
{
  TH1F* UpperLevelDistribution;
  TH1F** BottomLevelDistribution;
  TProfile* BottomLevelDistributionProfile;
};


template <class valueType>
TH1F* convert(const std::map<unsigned int,valueType>& my_map,std::string hName, std::string hTitle, std::string NameX, std::string NameY, bool minPositiveIsZero=true)
{
  if (my_map.empty()) return NULL;
  valueType minVal=my_map.begin()->first;
  if (minVal>0 && minPositiveIsZero) minVal=0;
  valueType maxVal=my_map.rbegin()->first;
  TH1F* toReturn = new TH1F(hName.c_str(),hTitle.c_str(),int(maxVal-minVal)+1,minVal,maxVal+1);
  toReturn->GetXaxis()->SetTitle(NameX.c_str());
  toReturn->GetYaxis()->SetTitle(NameY.c_str());
  for (typename std::map<unsigned int,valueType>::const_iterator it=my_map.begin(); it != my_map.end(); ++it)
    toReturn->Fill(it->first,it->second);
  return toReturn;
}

template <class T>
OneLevelMappedSingleMapCounterHistos convert(MappedCounters<T,SingleMapCounter> &allPlanDistr,std::string hNameBase, std::string hTitleBase,std::string NameX, std::string NameY)
{
  OneLevelMappedSingleMapCounterHistos toReturn;
  toReturn.UpperLevelDistribution = convert(allPlanDistr.distribution(),hNameBase,hTitleBase,NameY,"Number of entries");
  toReturn.BottomLevelDistribution = new TH1F*[allPlanDistr.size()];
  unsigned int maxXval=allPlanDistr.rbegin()->first+1;
  toReturn.BottomLevelDistributionProfile = new TProfile((hNameBase+"_prof1D").c_str(),(hTitleBase+" vs "+NameX).c_str(),maxXval,0,maxXval);
  toReturn.BottomLevelDistributionProfile->GetXaxis()->SetTitle(NameX.c_str());
  toReturn.BottomLevelDistributionProfile->GetYaxis()->SetTitle(NameY.c_str());
  unsigned int i=0;
  for (typename MappedCounters<T,SingleMapCounter>::iterator it=allPlanDistr.begin(); it != allPlanDistr.end(); ++it)
    {
      std::string Xname=NameX;
      std::replace(Xname.begin(),Xname.end(),' ','_');
      std::stringstream ssName;
      ssName << hNameBase << "_" << Xname << "_" << it->first;
      std::stringstream ssTitle;
      ssTitle << hTitleBase << " : " << NameX; 
      toReturn.BottomLevelDistribution[i]=convert(it->second.distribution(),ssName.str(),ssTitle.str(),NameY,"Number of entries");
      ++i;
      for ( std::map<unsigned int,unsigned int>::const_iterator itdistri=it->second.distribution().begin(); itdistri != it->second.distribution().end(); ++itdistri)
	for (unsigned int i=0; i<itdistri->second;++i)
	  toReturn.BottomLevelDistributionProfile->Fill(it->first,itdistri->first);
    }
  return toReturn;
}

template <class T>
TProfile2D** convert2D(MappedCounters<MappedCounters<MappedCounters<T,SingleMapCounter>,SingleMapCounter>,SingleMapCounter> &allPlanDistr,std::string hNameBase, std::string hTitleBase,std::string arrayBaseName, std::string XaxisName, std::string YaxisName)
{
  TProfile2D** toReturn=new TProfile2D*[allPlanDistr.size()];
  unsigned int i=0;
  for (typename MappedCounters<MappedCounters<MappedCounters<T,SingleMapCounter>,SingleMapCounter>,SingleMapCounter>::iterator it=allPlanDistr.begin(); it != allPlanDistr.end(); ++it)
    {
      std::string arrayname=arrayBaseName;
      std::replace(arrayname.begin(),arrayname.end(),' ','_');
      std::stringstream ssName;
      ssName << hNameBase << "_" << arrayname << "_" << it->first;
      std::stringstream ssTitle;
      ssTitle << hTitleBase << " : " << arrayBaseName << "_" << it->first; 
      MappedCounters<MappedCounters<T,SingleMapCounter>,SingleMapCounter>& YX_map=it->second;
      intervalle<unsigned int> Yrange(YX_map.begin()->first,YX_map.rbegin()->first);
      intervalle<unsigned int> Xrange(YX_map.begin()->second.begin()->first,YX_map.begin()->second.rbegin()->first);
      for (typename MappedCounters<MappedCounters<T,SingleMapCounter>,SingleMapCounter>::iterator itY=YX_map.begin(); itY!=YX_map.end(); ++itY)
	Xrange.expand(itY->second.begin()->first,itY->second.rbegin()->first);
      toReturn[i]=new TProfile2D(ssName.str().c_str(),ssTitle.str().c_str(),Xrange.length()+1,Xrange.first,Xrange.second+1,Yrange.length()+1,Yrange.first,Yrange.second+1);
      toReturn[i]->GetXaxis()->SetTitle(XaxisName.c_str());
      toReturn[i]->GetYaxis()->SetTitle(YaxisName.c_str());
      toReturn[i]->GetZaxis()->SetTitle(hTitleBase.c_str());
      for (typename MappedCounters<MappedCounters<T,SingleMapCounter>,SingleMapCounter>::iterator itY=YX_map.begin(); itY!=YX_map.end(); ++itY)
	for (typename MappedCounters<T,SingleMapCounter>::iterator itX=itY->second.begin(); itX!=itY->second.end(); ++itX)
	  for (std::map<unsigned int,unsigned int>::const_iterator itdistri=itX->second.distribution().begin(); itdistri != itX->second.distribution().end(); ++itdistri)
	    for (unsigned int n=0; n<itdistri->second; ++n)
	      toReturn[i]->Fill(itX->first,itY->first,itdistri->first);
      ++i;
    }
  return toReturn;
}

#endif
