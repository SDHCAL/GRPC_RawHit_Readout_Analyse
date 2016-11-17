%module grpc
%include "std_string.i" 
%include "std_vector.i" 
%include "std_pair.i" 
%include "std_set.i" 
%include "std_map.i" 

%typemap(in) void *ptr_TDirectory {
  $1 = PyCObject_AsVoidPtr($input);
}


%{
#include "DIFdrivenDevice.h"
#include "ExperimentalSetup.h"
#include "ConcreteExperimentalSetup.h"
#include "intervalle.h"
  //LCIO part
#include "IO/ILCFactory.h"
#include "IOIMPL/LCFactory.h"
#include "Exceptions.h"
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"

#include "RawHit_SDHCAL_Data_Reader.h"
#include "RawHit_SDHCAL_Data_Reader_From_LCEvent.h"
#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_Occupancy_Listener.h"
#include "RawHit_SDHCAL_Data_Reader_FromBIF.h"
#include "RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016.h"
#include "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis.h"
#include "RawHit_SDHCAL_Data_Reader_Noise.h"
#include "RawHit_SDHCAL_Data_Reader_Trivent.h" 
#include "RawHit_SDHCAL_Data_Reader_Threshold_Filter.h" 
#include "RawHit_Plan_Occupancy_Listener.h"
#include "ASCIIpersistance.h"
#include "GG_counter.h"
#include "GG_extended_counters.h"

  void debug_message(void *ptr) {std::cout << "DEBUG : pointer passing " << ptr<< std::endl;}
%}

%include "DIFdrivenDevice.h"
%include "ExperimentalSetup.h"
%include "ConcreteExperimentalSetup.h"
%include "intervalle.h"
%include "IO/ILCFactory.h"
%include "IOIMPL/LCFactory.h"
%include "Exceptions.h"
%include "IO/LCReader.h"
%include "IO/LCEventListener.h"



%include "RawHit_SDHCAL_Data_Reader.h"
%include "RawHit_SDHCAL_Data_Reader_From_LCEvent.h"
%include "RawHit_SDHCAL_Data_Listener.h"
%include "RawHit_Occupancy_Listener.h"
%include "GG_messageCounter.h"
%include "RawHit_SDHCAL_Data_Reader_FromBIF.h"
%include "RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016.h"
%include "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis.h"
%include "RawHit_SDHCAL_Data_Reader_Noise.h"
%include "RawHit_SDHCAL_Data_Reader_Trivent.h" 
%include "RawHit_SDHCAL_Data_Reader_Threshold_Filter.h" 
%include "RawHit_Plan_Occupancy_Listener.h"
%include "ASCIIpersistance.h"
%include "GG_counter.h"
%include "GG_extended_counters.h"

%template(std_string_vec) std::vector< std::string >;
%template(uint_pair) std::pair<unsigned int, unsigned int>;
%template(int_pair) std::pair<int, int>;
%template(uint_intervalle) intervalle<unsigned int>; 
%template(int_intervalle) intervalle<int>; 
%template(Singlecounter_map) std::map< unsigned int,SingleCounter >;
%template(MappedCounters_single) MappedCounters<SingleCounter>;
%template(Mappedcounter_leveltwo_map) std::map< unsigned int, MappedCounters<SingleCounter> >;
%template(MappedCounters_leveltwo) MappedCounters<MappedCounters<SingleCounter> >;
%template(Mappedcounter_levelthree_map) std::map< unsigned int, MappedCounters<MappedCounters<SingleCounter> > >;
%template(MappedCounters_levelthree) MappedCounters<MappedCounters<MappedCounters<SingleCounter> > >;
%template(ArrayCounter_single) ArrayCounter<SingleCounter>;
%template(ArrayCounter_MappedCounters_single) ArrayCounter<MappedCounters<SingleCounter> >;
%template(ArrayCounter_MappedCounters_leveltwo) ArrayCounter<MappedCounters<MappedCounters<SingleCounter> > >;
%template(ArrayCounter_MappedCounters_levelthree) ArrayCounter<MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > >;
%template(ArrayUpToCounter_single) ArrayUpToCounter<SingleCounter>;
%template(ArrayUpToCounter_MappedCounters_single) ArrayUpToCounter<MappedCounters<SingleCounter> >;
%template(ArrayUpToCounter_MappedCounters_leveltwo) ArrayUpToCounter<MappedCounters<MappedCounters<SingleCounter> > >;
%template(ArrayUpToCounter_MappedCounters_levelthree) ArrayUpToCounter<MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > >;
%template(RunThresholdCounter_single) RunThresholdCounter<SingleCounter>;
%template(RunThresholdCounter_MappedCounters_single) RunThresholdCounter<MappedCounters<SingleCounter> >;
%template(RunThresholdCounter_MappedCounters_leveltwo) RunThresholdCounter<MappedCounters<MappedCounters<SingleCounter> > >;
%template(RunThresholdCounter_MappedCounters_levelthree) RunThresholdCounter<MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > >;

%extend RawHit_Occupancy_Listener { void saveTo_wrap(void *ptr_TDirectory,ExperimentalSetup * v) {debug_message(ptr_TDirectory);self->saveTo((TDirectory*)ptr_TDirectory,v);} }
%extend RawHit_Occupancy_Listener { void saveToThreshold_wrap(unsigned int i,void *ptr_TDirectory,ExperimentalSetup * v) {debug_message(ptr_TDirectory);self->saveToThreshold(i,(TDirectory*)ptr_TDirectory,v);} }
%extend RawHit_Plan_Occupancy_Listener { void saveTo_wrap(void *ptr_TDirectory) {debug_message(ptr_TDirectory);self->saveTo((TDirectory*)ptr_TDirectory);} }
%extend RawHit_Plan_Occupancy_Listener { void saveToThreshold_wrap(unsigned int threshold,void *ptr_TDirectory) {debug_message(ptr_TDirectory);self->saveToThreshold(threshold,(TDirectory*)ptr_TDirectory);} }
