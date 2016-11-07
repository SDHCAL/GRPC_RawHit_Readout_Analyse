%module grpc
%include "std_string.i" 
%include "std_vector.i" 
%include "std_pair.i" 
%include "std_set.i" 

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

%template(std_string_vec) std::vector< std::string >;
%template(uint_pair) std::pair<unsigned int, unsigned int>;
%template(int_pair) std::pair<int, int>;
%template(uint_intervalle) intervalle<unsigned int>; 
%template(int_intervalle) intervalle<int>; 

%extend RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016 { void reportAndSaveTo_wrap(void *ptr_TDirectory) {debug_message(ptr_TDirectory);self->reportAndSaveTo((TDirectory*)ptr_TDirectory);} }
%extend RawHit_Occupancy_Listener { void saveTo_wrap(void *ptr_TDirectory,ExperimentalSetup * v) {debug_message(ptr_TDirectory);self->saveTo((TDirectory*)ptr_TDirectory,v);} }
