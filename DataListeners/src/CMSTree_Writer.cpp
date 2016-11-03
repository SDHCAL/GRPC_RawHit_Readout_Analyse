#include "CMSTree_Writer.h"

#include "TTree.h"
#include "RawCalorimeterHitPointerTools.h"


void CMSTree_Writer::createTree()
{
  CMSTree= new TTree("RAWData","RAWData");
  CMSTree->Branch("BIF_GTC",        &BIFHITGTC,  "BIF_GTC/i");
  CMSTree->Branch("BIF_TS",         &BIFHITts,   "BIF_TS/i");
  CMSTree->Branch("EventNumber",    &EventCount, "EventNumber/I");
  CMSTree->Branch("number_of_hits", &nHits,      "number_of_hits/I");
  CMSTree->Branch("TDC_channel",    &HITch);
  CMSTree->Branch("TDC_TimeStamp",  &HITts);
  CMSTree->Branch("ASICThreshold",  &HITthreshold);
} 

void CMSTree_Writer::process(const RawHit_SDHCAL_Data& d) 
{
  std::vector<RawCalorimeterHitPointer> BIF_hitvector=extract(d.getHitVector(),m_BIFnumber,rawHit_DIF());
  if (BIF_hitvector.empty()) return;
  if (BIF_hitvector.size()!=1)
    {
      multiBIFmessage.print();
      if (m_allowsDuplicate) duplicatemessage.print();
      else BIF_hitvector.erase(BIF_hitvector.begin()+1);
			       
    }
  BIFHITGTC=d.DIFtimeInfo()[m_BIFnumber].GTC;
  for (std::vector<RawCalorimeterHitPointer>::iterator itBIFhit=BIF_hitvector.begin(); itBIFhit!= BIF_hitvector.end(); ++itBIFhit)
    {
      ++EventCount;
      BIFHITts=(*itBIFhit)->getTimeStamp();
      processHits( d.getHitVector() );
      CMSTree->Fill();
    }
    
}


void CMSTree_Writer::processHits(const std::vector<RawCalorimeterHitPointer>& hitVec)
{
  std::map<unsigned int, std::map<unsigned int, const RawCalorimeterHitPointer*> > PadHitsJ, PadHitsI;
  clearDataTree();  
  nHits=0;
  for (std::vector<RawCalorimeterHitPointer>::const_iterator itHit=hitVec.begin(); itHit !=hitVec.end(); ++itHit)
    {
      if (m_setup.DIFnumberIsBIF(itHit->dif())) {addHit(itHit->dif(),*itHit); continue;}
      if (m_setup.DIFnumberIsStrip(itHit->dif()))
	{
	  unsigned int stripNumber,gap,plan;
	  m_setup.getCoord3D(itHit->dif(),itHit->asic(),itHit->channel(),stripNumber,gap,plan);
	  addHit(1000*(plan+1)+500*(gap-1)+stripNumber,*itHit);
	  continue;
	}
      if (m_setup.DIFnumberIsPad(itHit->dif()))
	{
	  unsigned int I,J,plan;
	  m_setup.getCoord3D(itHit->dif(),itHit->asic(),itHit->channel(),I,J,plan);
	  appendPadHit(PadHitsJ,1000*(plan+1)+J,*itHit); 
	  appendPadHit(PadHitsI,1000*(plan+1)+500+I,*itHit); 
	  continue;
	}
      std::cout << "CMSTree_Writer should never reached here" << std::endl;
    }
  addHit(PadHitsJ);
  addHit(PadHitsI);
 }

void CMSTree_Writer::appendPadHit( std::map<unsigned int, std::map<unsigned int, const RawCalorimeterHitPointer*> >& PadHits, unsigned int channel, const RawCalorimeterHitPointer& hit)
{
  if (PadHits.find(channel) == PadHits.end()) {PadHits[channel][hit->getTimeStamp()]=&hit; return;}
  if (PadHits[channel].find(hit->getTimeStamp()) == PadHits[channel].end()) {PadHits[channel][hit->getTimeStamp()]=&hit; return;}
  if ((*(PadHits[channel][hit->getTimeStamp()]))->getAmplitude()<hit->getAmplitude()) PadHits[channel][hit->getTimeStamp()]=&hit;
}

void CMSTree_Writer::addHit(std::map<unsigned int, std::map<unsigned int, const RawCalorimeterHitPointer*> >& PadHits)
{
  for (std::map<unsigned int, std::map<unsigned int, const RawCalorimeterHitPointer*> >::iterator itChannel=PadHits.begin(); itChannel!=PadHits.end(); ++itChannel)
    for (std::map<unsigned int, const RawCalorimeterHitPointer*>::iterator itTime=itChannel->second.begin(); itTime!=itChannel->second.end(); ++itTime)
      addHit(itChannel->first,*(itTime->second));
}


void CMSTree_Writer::addHit(int channel,const RawCalorimeterHitPointer& r)
{
  ++nHits; 
  HITch.push_back(channel); 
  HITts.push_back(int(r->getTimeStamp())-BIFHITts+m_decalage);
  HITthreshold.push_back(r->getAmplitude());
}
