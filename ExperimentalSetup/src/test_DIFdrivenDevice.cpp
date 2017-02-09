//test program

#include <assert.h>
#include <iostream>

#include "PadDevice.h"
#include "BifaceStripDevice.h"

void testDevice(DIFdrivenDevice &device)
{
  std::vector<unsigned int> DIFs=device.DIFnumbers();
  for (unsigned int idif=0; idif<DIFs.size(); ++idif)
    for (unsigned int igap=1; igap<=device.maxNumberOfASICs(); ++igap)
      for (unsigned int ichan=0; ichan<64; ++ichan)
	{
	  DIFdrivenDevice::I_J ij=device.getIJ(DIFs[idif],igap,ichan);
	  DIFdrivenDevice::dif_asic_channel d_a_c=device.getDifAsicChannel(ij.first,ij.second);
	  assert (DIFs[idif]==d_a_c.dif);
	  assert (igap==d_a_c.asic);
	  assert (ichan==d_a_c.channel);
	}
}

int main()
{
  std::cout << "testing single pad" << std::endl;
  PadDevice single(34,3);
  testDevice(single);
  std::cout << "testing square meter pad" << std::endl;
  PadDevice squareMeter(76,21,89,5);
  testDevice(squareMeter);
  std::cout << "testing CMS strip" << std::endl;
  BifaceStripDevice CMS(69,37,9);
  testDevice(CMS);
  return 0;
}

