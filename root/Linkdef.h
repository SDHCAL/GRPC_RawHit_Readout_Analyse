#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

//#pragma link C++ class IO::LCEventListener+;
//#pragma link C++ class IO::LCRunListener+;
#pragma link C++ class IO::LCReader+;
#pragma link C++ class IO::LCWriter+;
#pragma link C++ class IO::ILCFactory+;
#pragma link C++ class IOIMPL::LCFactory+;


#pragma link C++ class ASIC_Coordinate_Translator+;
#pragma link C++ class pair<unsigned int,unsigned int>+;
#pragma link C++ class intervalle<unsigned int>+;
#pragma link C++ class intervalle<int>+;

#pragma link C++ class DIFdrivenDevice+;
#pragma link C++ class ExperimentalSetup+;
#pragma link C++ defined_in "ConfigInfoExceptions.h";
#pragma link C++ defined_in "ConfigInfo.h";
#pragma link C++ defined_in "ConcreteExperimentalSetup.h";

#pragma link C++ class ASCIIpersistance+;
#pragma link C++ class SingleCounter+;
#pragma link C++ class MappedCounters<SingleCounter>+;
#pragma link C++ class MappedCounters<MappedCounters<SingleCounter> >+;
#pragma link C++ class MappedCounters<MappedCounters<MappedCounters<SingleCounter> > >+;

#pragma link C++ defined_in "GG_counter_histos.h";

#pragma link C++ class ArrayCounter<SingleCounter>+;
#pragma link C++ class ArrayCounter<MappedCounters<SingleCounter> >+;
#pragma link C++ class ArrayCounter<MappedCounters<MappedCounters<SingleCounter> > >+;
#pragma link C++ class ArrayCounter<MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > >+;

#pragma link C++ class ArrayUpToCounter<SingleCounter>+;
#pragma link C++ class ArrayUpToCounter<MappedCounters<SingleCounter> >+;
#pragma link C++ class ArrayUpToCounter<MappedCounters<MappedCounters<SingleCounter> > >+;
#pragma link C++ class ArrayUpToCounter<MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > >+;

#pragma link C++ class RunThresholdCounter<SingleCounter>+;
#pragma link C++ class RunThresholdCounter<MappedCounters<SingleCounter> >+;
#pragma link C++ class RunThresholdCounter<MappedCounters<MappedCounters<SingleCounter> > >+;
#pragma link C++ class RunThresholdCounter<MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > >+;

#pragma link C++ class GG_messageCounter;

#pragma link C++ class RawHit_SDHCAL_Data_Listener;
#pragma link C++ class RawHit_SDHCAL_Data_Reader;

#pragma link C++ class RawHit_SDHCAL_Data_Reader_From_LCEvent;
#pragma link C++ class RawHit_SDHCAL_Data_Reader_FromBIF;
#pragma link C++ class RawHit_SDHCAL_Data_Reader_FromTimeAnalysis;
#pragma link C++ class RawHit_SDHCAL_Data_Reader_Noise;
#pragma link C++ class RawHit_SDHCAL_Data_Reader_Trivent;
#pragma link C++ class RawHit_SDHCAL_Data_Reader_Threshold_Filter;
#pragma link C++ class RawHit_SDHCAL_Data_Reader_TriggeredMode;

#pragma link C++ class CMSTree_Writer+;
#pragma link C++ class CMSTree_Writer_NoBIF+;
#pragma link C++ class RawHit_Occupancy_Listener;
#pragma link C++ class RawHit_Plan_Occupancy_Listener;

#pragma link C++ class RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016;

#pragma link C++ class RawHit_SDHCAL_Data_LCWriter;
#pragma link C++ class RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit;
#pragma link C++ class RawHit_SDHCAL_Data_LCWriter_CalorimeterHit;

#endif
