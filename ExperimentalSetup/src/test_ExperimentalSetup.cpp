//test program

#include <assert.h>
#include <iostream>

//#include "PadDevice.h"
//#include "BifaceStripDevice.h"
//#include "TricotDevice.h"
#include "ExperimentalSetup.h"

void testExperimentalSetup()
{
  std::cout << "testing experimental setup" << std::endl;
  ExperimentalSetup aSetup;
  assert(aSetup.nBIFs()==0);
  aSetup.addBIF(2);
  aSetup.addOneDIFPadDevice(14);
  aSetup.addOneDIFPadDevice(15);
  aSetup.addSDHCALPlan(300,301,302);
  aSetup.addCMSstrip(215,216);
  aSetup.addTricot(888);
  
  assert(aSetup.hasBIF());
  assert(aSetup.nBIFs()==1);
  assert(aSetup.getBIF(0)==2);
  assert(aSetup.DIFnumberIsBIF(2));
  assert(aSetup.DIFnumberIsKnown(2));
  assert(aSetup.DIFnumberIsKnown(14));
  assert(aSetup.DIFnumberIsKnown(300));
  assert(aSetup.DIFnumberIsKnown(215));
  assert(aSetup.DIFnumberIsKnown(888));
  assert(aSetup.DIFnumberIsStrip(215));
  assert(aSetup.DIFnumberIsStrip(216));
  assert(aSetup.DIFnumberIsPad(14));
  assert(aSetup.DIFnumberIsPad(15));
  assert(aSetup.DIFnumberIsPad(300));
  assert(aSetup.DIFnumberIsPad(301));
  assert(aSetup.DIFnumberIsPad(302));
  assert(aSetup.DIFnumberIsTricot(888));
  
  assert(aSetup.PlanIsPad(0));
  assert(aSetup.PlanIsPad(1));
  assert(aSetup.PlanIsPad(2));
  assert(aSetup.PlanIsStrip(3));
  assert(aSetup.PlanIsTricot(4));

  std::vector<unsigned int> vec=aSetup.getStripDevice_DIFnumber();
  assert(vec.size()==2);
  assert(vec[0]==215); 
  assert(vec[1]==216);
  vec=aSetup.getPadDevice_DIFnumber();
  std::vector<unsigned int> vecAttendu{14,15,300,301,302};
  assert(vec==vecAttendu);
  vec=aSetup.getTricotDevice_DIFnumber();
  assert(vec.size()==1);
  assert(vec[0]==888); 

  assert (aSetup.getPadDevice(1) == NULL);
  assert (aSetup.getPadDevice(14) != NULL);
  assert (aSetup.getPadDevice(300) != NULL);
  assert (aSetup.getPadDevice(215) == NULL);
  assert (aSetup.getPadDevice(888) == NULL);
  assert (aSetup.getStripDevice(1) == NULL);
  assert (aSetup.getStripDevice(14) == NULL);
  assert (aSetup.getStripDevice(300) == NULL);
  assert (aSetup.getStripDevice(215) != NULL);
  assert (aSetup.getStripDevice(888) == NULL);
  assert (aSetup.getTricotDevice(1) == NULL);
  assert (aSetup.getTricotDevice(14) == NULL);
  assert (aSetup.getTricotDevice(300) == NULL);
  assert (aSetup.getTricotDevice(215) == NULL);
  assert (aSetup.getTricotDevice(888) != NULL);

  //test if there are more than one BIFs
  aSetup.addBIF(3);
  assert(aSetup.hasBIF());
  assert(aSetup.nBIFs()==2);
  assert(aSetup.getBIF(0)==2);
  assert(aSetup.getBIF(1)==3);
  assert(aSetup.getBIF(2)==0);
  assert(aSetup.getBIF(14)==0);
  assert(aSetup.DIFnumberIsBIF(2));
  assert(aSetup.DIFnumberIsBIF(3));

  aSetup.addBIF(1);
  assert(aSetup.hasBIF());
  assert(aSetup.nBIFs()==3);
  assert(aSetup.getBIF(0)==1);
  assert(aSetup.getBIF(1)==2);
  assert(aSetup.getBIF(2)==3);
  assert(aSetup.getBIF(14)==0);
  assert(aSetup.DIFnumberIsBIF(1));
  assert(aSetup.DIFnumberIsBIF(2));
  assert(aSetup.DIFnumberIsBIF(3));

}


int main()
{
  testExperimentalSetup();
  return 0;
}
