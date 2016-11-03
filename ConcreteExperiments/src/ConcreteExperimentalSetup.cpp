#include "ConcreteExperimentalSetup.h"

GIF_oct2016_ExperimentalSetup::GIF_oct2016_ExperimentalSetup()
{
  //Counting from source 
  addBIF(3);
  addOneDIFPadDevice(125);
  addOneDIFPadDevice(32);
  addOneDIFPadDevice(28);
  addOneDIFPadDevice(6);
  addCMSstrip(142,26); //verre
  addCMSstrip(23,2); //bakelite
}
