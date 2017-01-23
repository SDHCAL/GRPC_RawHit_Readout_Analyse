#ifndef CMSTree_Writer_HH
#define CMSTree_Writer_HH

#include "RawHit_SDHCAL_Data_Listener.h"
#include "ExperimentalSetup.h"
#include "RawHit_SDHCAL_Data.h"
#include "GG_messageCounter.h"

class TTree;

class CMSTree_Writer : public RawHit_SDHCAL_Data_Listener
{
 public:
 CMSTree_Writer(ExperimentalSetup& setup,unsigned int halfdecalage,bool allowsDuplicate=false) : m_setup(setup), m_BIFnumber(setup.getBIF()), m_decalage(2*halfdecalage), m_allowsDuplicate(allowsDuplicate), EventCount(0) {}

  void process(const RawHit_SDHCAL_Data&);

  void createTree();
  TTree* getTree() {return CMSTree;}

  GG_messageCounter multiBIFmessage=GG_messageCounter("Warning CMSTree_Writer expect receiving a data set with one BIF trigger"); //C++11
  GG_messageCounter duplicatemessage=GG_messageCounter("CMSTree_Writer : Events will be duplicated in the Tree"); //C++11

 private:
  void clearDataTree() {HITch.clear(); HITts.clear();HITthreshold.clear();}
  void addHit(int channel,const RawCalorimeterHitPointer& r); 
 protected:
  void processHits(const std::vector<RawCalorimeterHitPointer>&);
 private:
  void appendPadHit( std::map<unsigned int, std::map<unsigned int, const RawCalorimeterHitPointer*> >& PadHits, unsigned int channel, const RawCalorimeterHitPointer& hit);
  void addHit(std::map<unsigned int, std::map<unsigned int, const RawCalorimeterHitPointer*> >& PadHits);

  ExperimentalSetup& m_setup;
  unsigned int m_BIFnumber;
  unsigned int m_decalage;

  bool m_allowsDuplicate;

  //Data for tree
 protected:
  unsigned int BIFHITGTC;  //GTC of the Readout (not in official CMS tree)
  unsigned int BIFHITts;   //timestamp of the BIF hit used to create the Event (not in official CMS tree)
  int EventCount;          //Event Tag
 private:
  int nHits;               //Number of fired channels in event (and in vectors)
  std::vector<int>  HITch; //List of fired channels in Event
  std::vector<float> HITts;//List of fired channels timestamp
  std::vector<int> HITthreshold;//List of fired channels highest crossed thresholds 
 
  //the Tree
  TTree *CMSTree;

};

#endif
