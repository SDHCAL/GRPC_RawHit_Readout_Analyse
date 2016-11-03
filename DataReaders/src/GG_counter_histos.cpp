#include "GG_counter_histos.h"

#include <sstream>

std::pair<TH2F*,TH2F*>* convert(MappedCounters<MappedCounters<MappedCounters<SingleCounter> > >& DifAsicChannelCounters,std::string hNameBase, std::string MappedCounterHistoName, std::string MappedCountersNameX, std::string MappedCountersNameY, float scale , std::string InsteadOfYInTitle, bool secondHist)
{
  std::pair<TH2F*,TH2F*>* toReturn=NULL;
  if (DifAsicChannelCounters.empty()) return toReturn;
  toReturn=new std::pair<TH2F*,TH2F*>[DifAsicChannelCounters.size()];

  std::pair<TH2F*,TH2F*>* currentHisto=toReturn;
  for (std::map<unsigned int, MappedCounters<MappedCounters<SingleCounter> > >::iterator itHisto=DifAsicChannelCounters.begin(); 
       itHisto!=DifAsicChannelCounters.end(); ++itHisto)
    {
      std::stringstream ss;
      ss << hNameBase << "_" << MappedCounterHistoName << "_" << itHisto->first;
      (*currentHisto)=convert(itHisto->second,ss.str(),MappedCountersNameX,MappedCountersNameY,scale,InsteadOfYInTitle,secondHist);
      ++currentHisto;
    }

  return toReturn;
}
