#ifndef GG_counter_histos_HH
#define GG_counter_histos_HH

#include"GG_counter.h"
#include<string>
#include<utility>


//class TH1F;
//class TH2F;

#include "TH1F.h"
#include "TH2F.h"

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

#endif
