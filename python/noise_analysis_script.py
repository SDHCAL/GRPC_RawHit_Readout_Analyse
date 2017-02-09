#!/usr/bin/python

#to be executed in the lib directory where all the needed libraries are installed 
# usage python <name>.py  list_of_slcio_files

import sys
import ROOT
ROOT.gROOT.Reset()
ROOT.gSystem.Load('liblcio')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')
if ROOT.gROOT.GetVersion()[0]=='6':
  #ROOT 6.08.02 don't understand non template dictionnary without it (don't know why)
  dummy=ROOT.intervalle('unsigned int')()

inputFileNames=ROOT.vector("string")()
for f in sys.argv[1:]:
    inputFileNames.push_back(f);

for file in inputFileNames:
    print file

experience=ROOT.GIF_oct2016_ExperimentalSetup()
numeroBIF=experience.getBIF();

print numeroBIF

BIFtriggerWindow=ROOT.intervalle('int')(-8,-6)
NoiseWindowLength=50; #10 microsecond

#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()

#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

noiseReader=ROOT.RawHit_SDHCAL_Data_Reader_Noise(experience,NoiseWindowLength)
noiseReader.setVetoOnBIF()
noiseReader.setBIFtimeWindow(BIFtriggerWindow)
masterReader.registerDataListener(noiseReader)


hitOccupancy=ROOT.RawHit_Occupancy_Listener()
noiseReader.registerDataListener(hitOccupancy)



#open file and event loop
lcReader.open( inputFileNames )
lcReader.readStream(1000)

#end of event loop
rootFile=ROOT.TFile("noise.root"  , "RECREATE")
#load ROOT library missing
ROOT.TH1F
d=rootFile.mkdir("Noise_10microsec")
hitOccupancy.saveTo(d,experience)

rootFile.Close()

lcReader.close()
