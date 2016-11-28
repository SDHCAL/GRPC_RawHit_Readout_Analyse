#include "ILCConfDB.h"

#include <iostream>
#include <fstream>
#include <set>
#include <string>

int main(int argc, char *argv[])
{
  const char* setupName="";
  if (argc==1) 
    {
      setupName="GIFPP_STRIP_13";
      std::cout << "No setup name provided : will use " << setupName << std::endl;
    }
  else
    setupName=argv[1];
  std::string outfileName=setupName;
  outfileName+=".csv";
  DBInit::init();
  std::cout << "dumping setup " << setupName << std::endl;
  Setup *mySetup=Setup::getSetup(setupName);
  std::cout << mySetup<< std::endl;
  std::vector<State*> vstate=mySetup->getStates();
  std::cout << vstate.size() << " states found." << std::endl;
  State* dhcal_state=vstate[0];
  AsicConfiguration* AsicConf=dhcal_state->getAsicConfiguration();
  std::cout << "ASIC conf pointer=" << AsicConf << std::endl;
  std::vector<ConfigObject*>& ASICconfVector=AsicConf->getVector();
  std::cout <<"found " << ASICconfVector.size() << " ASICs in configuration. " << std::endl;
  std::ofstream outfile(outfileName);
  outfile << "IS_ASIC;DIF_ID;ASIC_ID;THRESH1;THRESH2;THRESH3;NCHANNELS";
  for (unsigned int i=0; i<64; ++i) outfile << ";GAIN"<<i;
  outfile<<std::endl;
  for (std::vector<ConfigObject*>::iterator it=ASICconfVector.begin(); it!=ASICconfVector.end();++it)
    {
      ConfigObject* obj=*it;
      std::vector<int> gain=obj->getIntVector("PAGAIN");
      std::set<int> gainset;
      for (std::vector<int>::iterator itgain=gain.begin(); itgain != gain.end(); ++itgain) gainset.insert(*itgain);
      std::cout << obj->getObjectName() << " : DIF_ID=" << obj->getInt("DIF_ID") 
		<< " ASIC_ID=" <<  obj->getInt("HEADER") 
		<< " THRESHOLDS=" << obj->getInt("B0") << "/" << obj->getInt("B1")<<"/" << obj->getInt("B2") 
		<< " number of gains=" << gain.size() << " gain set=";
      outfile<<int(obj->getObjectName()=="ASIC")<<";"<<obj->getInt("DIF_ID")<<";"<<obj->getInt("HEADER")<<";"
	     <<obj->getInt("B0") << ";" << obj->getInt("B1")<<";" << obj->getInt("B2")<<";"<<gain.size();
      for (std::set<int>::iterator itg=gainset.begin(); itg!=gainset.end(); ++itg) std::cout << *itg << " ";
      for (std::vector<int>::iterator itgain=gain.begin(); itgain != gain.end(); ++itgain) outfile << ";" << (*itgain);
      std::cout << std::endl;
      outfile << std::endl;
    }
  delete mySetup;
  return 0;
}
