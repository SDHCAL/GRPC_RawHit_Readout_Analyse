#!/usr/bin/python

#to be executed in the lib directory where all the needed libraries are installed 
# usage python <name>.py  list_of_slcio_files
import grpc
import sys
import ROOT
ROOT.gROOT.Reset()
if ROOT.gROOT.GetVersion()[0]=='6':
  #ROOT 6.08.02 don't understand non template dictionnary without it (don't know why)
  dummy=ROOT.intervalle('unsigned int')()

inputFileNames=grpc.std_string_vec()
for f in sys.argv[1:]:
    inputFileNames.push_back(f);

for file in inputFileNames:
    print file

experience=grpc.GIF_oct2016_ExperimentalSetup()
numeroBIF=experience.getBIF();

print numeroBIF

BIFtriggerWindow=grpc.int_intervalle(-8,-6)
NoiseWindowLength=50; #10 microsecond

#start LCIO reader
lcReader=grpc.LCFactory_getInstance().createLCReader()

#create architecture of listeners
masterReader=grpc.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

noiseReader=grpc.RawHit_SDHCAL_Data_Reader_Noise(experience,NoiseWindowLength)
noiseReader.setVetoOnBIF()
noiseReader.setBIFtimeWindow(BIFtriggerWindow)
masterReader.registerDataListener(noiseReader)


hitOccupancy=grpc.RawHit_Occupancy_Listener()
noiseReader.registerDataListener(hitOccupancy)



#open file and event loop
lcReader.open( inputFileNames )
lcReader.readStream(1000)

#end of event loop
rootFile=ROOT.TFile("noise.root"  , "RECREATE")
d=rootFile.mkdir("Noise_10microsec")
hitOccupancy.saveTo_wrap(ROOT.AsCObject(d),experience)

rootFile.Close()

lcReader.close()
