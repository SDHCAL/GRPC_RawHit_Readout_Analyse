#include "GG_experimentalsetup_counters.h"

#include "TDirectory.h"
#include "GG_counter_histos.h"

void GG_ExperimentalSetup_Counter::add(unsigned int val, unsigned int dif, unsigned int asic, unsigned int channel)
{
  unsigned int keys[3]={dif,asic,channel};
  ArrayCounter<DIFCounters>::add(val,0,keys);
  if (NULL == m_setup) return;
  unsigned int s_keys[3]={0,0,0};
  unsigned int &J=s_keys[1];
  unsigned int &I=s_keys[2];
  unsigned int &K=s_keys[0];
  m_setup->getCoord3D(dif,asic,channel,I,J,K);
  ArrayCounter<DIFCounters>::add(val,1,s_keys);
}

void GG_ExperimentalSetup_Counter::write(std::ostream& oflux)
{
  std::string labels[4]={"Setup","DIF","ASIC","Channel"};
  getCounter(0).write(labels,oflux);
  if (NULL == m_setup) return;
  std::string s_labels[4]={"Setup","Layer","JorGaporAngle","IorStrip"};
  getCounter(1).write(s_labels,oflux);  
}

void GG_ExperimentalSetup_Counter::saveTo(TDirectory* ROOTdir,std::string histoBaseName,bool secondHist)
{
  if (getCounter(0).sumcount()==0) return;
  if (NULL==ROOTdir) return;
  ROOTdir->cd();
  TDirectory *byDifDir=ROOTdir->mkdir("ByDIF","ByDIF");
  byDifDir->cd();
  std::pair<TH2F*,TH2F*> difAsicHisto=convert(getCounter(0),histoBaseName,"DIF","ASIC",1,"",secondHist);
  if (difAsicHisto.first) difAsicHisto.first->Write();
  if (difAsicHisto.second) difAsicHisto.second->Write();
  std::pair<TH2F*,TH2F*>* difAsicChannelHistos=convert(getCounter(0),histoBaseName,"DIF","ASIC","CHANNEL",1,"",secondHist);
  for (unsigned int i=0; i<getCounter(0).size(); ++i)
    {
      if (difAsicChannelHistos[i].first) difAsicChannelHistos[i].first->Write();
      if (difAsicChannelHistos[i].second) difAsicChannelHistos[i].second->Write();
    }
  
  if (NULL==m_setup) return;
  ROOTdir->cd();
  TDirectory *byLayerDir=ROOTdir->mkdir("ByLayer","ByLayer");
  byLayerDir->cd();
  delete [] difAsicChannelHistos;
  difAsicChannelHistos=convert(getCounter(1),histoBaseName,"Plan","J","I",1,"CHANNEL",secondHist);
  auto itmap=getCounter(1).begin();
  for (unsigned int i=0; i<getCounter(1).size(); ++i)
    {
      unsigned int numeroPlan=itmap->first;
      if (m_setup->PlanIsStrip(numeroPlan)) setAxisTitle(difAsicChannelHistos[i],"Gap","Strip");
      if (m_setup->PlanIsTricot(numeroPlan))   setAxisTitle(difAsicChannelHistos[i],"Angle","Strip");
      if (difAsicChannelHistos[i].first) difAsicChannelHistos[i].first->Write();
      if (difAsicChannelHistos[i].second) difAsicChannelHistos[i].second->Write();
	++itmap;
    }
}

void GG_ExperimentalSetup_Counter::setAxisTitle(TH2F* Histo, std::string Yname, std::string Xname)
{
  if (NULL==Histo) return;
  Histo->GetXaxis()->SetTitle(Xname.c_str());
  Histo->GetYaxis()->SetTitle(Yname.c_str());
}

