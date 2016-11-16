#include "RawHit_Occupancy_Listener.h"

#include "RawHit_SDHCAL_Data.h"
#include "intervalle.h"
#include "ExperimentalSetup.h"

#include "TDirectory.h"
#include "TH2F.h"
#include "TH1F.h"

#include "GG_counter_histos.h"
#include <sstream>

RawHit_Occupancy_Listener::RawHit_Occupancy_Listener()
{
  m_countBy_DifAsicChannel.n_event=0;
}

void RawHit_Occupancy_Listener::process(const RawHit_SDHCAL_Data& d) 
{
  m_countBy_DifAsicChannel.n_event+=d.getNumberOfEventInThisData();
  //std::cout << "Got called " << m_total << " et " << d.getHitVector().size() << std::endl;
  //unsigned int value=1;
  for (std::vector<RawCalorimeterHitPointer>::const_iterator it=d.getHitVector().begin(); it !=d.getHitVector().end(); ++it)
    {
      unsigned int keys[3]={it->dif(),it->asic(),it->channel()};
      unsigned int upTo=0;
      if (it->aboveSecondThreshold()) upTo=1;
      if (it->aboveThirdThreshold())  upTo=2;
      m_countBy_DifAsicChannel.m_thresholdCounters.add(1,upTo,keys);
    }
  m_countBy_DifAsicChannel.m_thresholdCounters.newSet();
}

void RawHit_Occupancy_Listener::saveToThreshold(unsigned int threshold,TDirectory* ROOTdir,ExperimentalSetup* e)
{
  unsigned int index=threshold-1;
  if (index>=3) return;
  if (NULL==ROOTdir) return;
  ROOTdir->cd();
  if (m_countBy_DifAsicChannel.n_event==0) 
    {
      std::cout << "RawHit_Occupancy_Listener : no data to save to ROOT directory " << ROOTdir->GetName() << std::endl;
      return;
    }

  std::pair<TH1F*,TH1F*> difHisto=convert(m_countBy_DifAsicChannel.m_thresholdCounters.getCounter(index),std::string("DIF_occupancy"),std::string("DIF"),1/float(m_countBy_DifAsicChannel.n_event));
  difHisto.first->Write();
  difHisto.second->Write();
  std::cout << "1D DIF histo saved" << std::endl;

  std::pair<TH2F*,TH2F*> difAsicHisto=convert(m_countBy_DifAsicChannel.m_thresholdCounters.getCounter(index),"ASIC_occupancy","DIF","ASIC",1/float(m_countBy_DifAsicChannel.n_event));
  difAsicHisto.first->Write();
  difAsicHisto.second->Write();

  std::pair<TH2F*,TH2F*>* difAsicChannelHistos=convert(m_countBy_DifAsicChannel.m_thresholdCounters.getCounter(index),"CHANNEL_occupancy","DIF","ASIC","CHANNEL",1/float(m_countBy_DifAsicChannel.n_event));
  for (unsigned int i=0; i<m_countBy_DifAsicChannel.m_thresholdCounters.getCounter(index).size(); ++i)
    {
      difAsicChannelHistos[i].first->Write();
      difAsicChannelHistos[i].second->Write();
    }
  std::cout << "2D DIF-ASIC-CHANNEL histo saved" << std::endl;


  if (e==NULL) return;

  PlaneCounters Plane_IJ_counters;
  for (std::map<unsigned int,MappedCounters<MappedCounters<SingleCounter> > >::iterator itdif=m_countBy_DifAsicChannel.m_thresholdCounters.getCounter(index).begin(); itdif!=m_countBy_DifAsicChannel.m_thresholdCounters.getCounter(index).end(); ++itdif)
    {
      if (e->DIFnumberIsBIF(itdif->first)) continue;
      for (std::map<unsigned int, MappedCounters<SingleCounter> >::iterator itasic=itdif->second.begin(); itasic!=itdif->second.end(); ++itasic)
	for (std::map<unsigned int,SingleCounter>::iterator itchannel=itasic->second.begin(); itchannel!=itasic->second.end(); ++itchannel)
	  {
	    unsigned int keys[3];
	    unsigned int &I=keys[1];
	    unsigned int &J=keys[2];
	    unsigned int &K=keys[0];
	    e->getCoord3D(itdif->first,itasic->first,itchannel->first,I,J,K);
	    unsigned int val=itchannel->second.sumcount();
	    Plane_IJ_counters.add(val,keys);
	  }
    }
  difHisto=convert(Plane_IJ_counters,"Plane_occupancy","Layer",1/float(m_countBy_DifAsicChannel.n_event)); //second histo is meaningless here 
  difHisto.first->Write(); 
  delete [] difAsicChannelHistos;
  difAsicChannelHistos=convert(Plane_IJ_counters,"CHANNEL_occupancy","PLANE","I or strip","J or Gap",1/float(m_countBy_DifAsicChannel.n_event), "CHANNEL", false);
  for (unsigned int i=0; i<Plane_IJ_counters.size(); ++i) difAsicChannelHistos[i].first->Write();
  delete [] difAsicChannelHistos;
}

void RawHit_Occupancy_Listener::saveTo(TDirectory* ROOTdir,ExperimentalSetup* e)
{
  std::string dirname="aboveThresh_";
  char c='1';
  for (unsigned int i=1; i<=3; ++i) 
    {
      TDirectory* d=ROOTdir->mkdir((dirname+c).c_str());
      saveToThreshold(i,d,e);
      ++c;
    }
}

