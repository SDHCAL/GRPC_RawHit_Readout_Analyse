#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"

#include <string>
#include <vector>

#include "RawHit_SDHCAL_Data_Reader_From_LCEvent.h"
#include "RawHit_SDHCAL_Data_Reader_FromBIF.h"
#include "CMSTree_Writer.h"
#include "ConcreteExperimentalSetup.h"

#include "RawHit_SDHCAL_Data_Reader_Noise.h"
#include "CMSTree_Writer_NoBIF.h"


#include "TFile.h"
#include "TTree.h"

#include "TROOT.h" // to have ROOT knowing std::vector<float> for the CMSTree_Writer

std::string SplitFilenameSDHCAL (const std::string& str)
{
  std::size_t found = str.find_first_of("_");
  std::string a;
  a=str.substr(found+1);
  std::size_t found2 = a.find_first_of("_");
  a=a.substr(0,found2);
  return a;
}


void usage(char *argv[])
{
  std::cout << "Usage :" << std::endl;
  std::cout << argv[0] << " <-n XXX> mode list_of_inpuFiles" << std::endl;
  std::cout << "You should provide at least one input file" << std::endl;
  std::cout << "mode should be set to either BIF, NOISE or TRIGGED" << std::endl;
  std::cout << "Option -n allows to specify the number of LCIO record to read (i.e LCRun and LCEvent)" << std::endl;
  std::cout << "          For standard single lcio files form small setup DAQ, it corresponds to numberOfEvents+1" << std::endl;
  std::cout << "mode description " << std::endl;
  std::cout << "       BIF     : use BIF data to generate entries in the tree, centering BIF time at 50" << std::endl;
  std::cout << "       NOISE   : generate up to 100000 entries with first readout intervals, removing intervals containing BIF data" << std::endl;
  std::cout << "       TRIGGED : generate one entry per readout containing only 500 clock tick before the readout (NOT YET IMPLEMENTED)" << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc==1) {usage(argv); return 1;}
  gROOT->ProcessLine("#include <vector>"); // to have ROOT knowing std::vector<float> for the CMSTree_Writer
  

  //parameters
  int maxEvt=-1;
  if (std::string(argv[1])=="-n") {maxEvt=atoi(argv[2]); argc-=2; argv[2]=argv[0]; argv+=2;}
  if (argc==1 || argc==2) {usage(argv); return 1;}
  std::string mode=argv[1];
  if (mode != "BIF" && mode != "NOISE" && mode !="TRIGGED") {usage(argv); return 1;}
  argc-=1; argv[1]=argv[0]; argv+=1;

  if (mode =="TRIGGED") { std::cout << "mode " << mode << " not yet implemented" << std::endl; return 2;}
  
  std::vector<std::string> inputFileNames;
  for (int i=1; i<argc; ++i) 
    {
      std::string inputFile=argv[i]; 
      if (inputFile.size()<6 || inputFile.substr(inputFile.size()-6)!=".slcio")
	{std::cout << "removing non slcio file " << inputFile << " from input file list" << std::endl; continue;}
      inputFileNames.push_back(inputFile);
    }
  if (inputFileNames.empty()) { std::cout << "no slcio files in input" << std::endl; return 1;}

  //inputFileNames.push_back("../../DHCAL_733694_I0_0.slcio");
  std::string runNumberFromFile=SplitFilenameSDHCAL(basename(inputFileNames[0].c_str()));

  GIF_oct2016_ExperimentalSetup experience;

  unsigned int numeroBIF=experience.getBIF();
  intervalle<int> BIFtriggerWindow(-25,50);

  

  //start LCIO reader
  IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;

  //create architecture of listeners
  RawHit_SDHCAL_Data_Reader_From_LCEvent masterReader; 
  lcReader->registerLCEventListener(&masterReader);
    
  RawHit_SDHCAL_Data_Reader_FromBIF BIF_splitter(numeroBIF,BIFtriggerWindow);
  if (mode=="BIF") masterReader.registerDataListener(BIF_splitter);

  RawHit_SDHCAL_Data_Reader_Noise Noise_splitter(experience,500);
  Noise_splitter.setSplitEventForListeners(true);
  Noise_splitter.setMaxEventsToSend(100000);
  if (mode=="NOISE") masterReader.registerDataListener(Noise_splitter);

  
  
  CMSTree_Writer treeWriterBIF(experience,abs(BIFtriggerWindow.first));
  CMSTree_Writer_NoBIF treeWriterNoBIF(experience,0);
  treeWriterNoBIF.setAddEntryForEventsWithNoHits(true);
  CMSTree_Writer &treeWriter=(mode=="BIF" ? treeWriterBIF : treeWriterNoBIF);
  if (mode=="BIF") BIF_splitter.registerDataListener(treeWriter);
  if (mode=="NOISE") Noise_splitter.registerDataListener(treeWriter);

  //open file and event loop
  lcReader->open( inputFileNames ) ;

  //create root file
  TFile* file = new TFile( ("Scan"+runNumberFromFile+"_HV1_DAQ.root").c_str()  , "RECREATE");    
  treeWriter.createTree();

  //----------- the event loop -----------
  if (maxEvt>0) lcReader->readStream(maxEvt);
  else lcReader->readStream();

  // -------- end of event loop -----------
 
  treeWriter.getTree()->Write();
  file->Close() ;

  lcReader->close() ;  
  delete lcReader ;
}
