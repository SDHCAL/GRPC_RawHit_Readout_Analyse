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
  aSetup.addBIF(2);
  aSetup.addOneDIFPadDevice(14);
  aSetup.addOneDIFPadDevice(15);
  aSetup.addSDHCALPlan(300,301,302);
  aSetup.addCMSstrip(215,216);
  assert(aSetup.hasBIF());
  //assert(aSetup.
}


int main()
{
  testExperimentalSetup();
  return 0;
}
