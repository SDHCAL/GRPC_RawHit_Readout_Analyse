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

#experience=ROOT.CERN_SPS_Sept2018_SDHCAL_ExperimentalSetup()
experience=ROOT.CERN_SPS_H2_Sept2022_part1_SDHCAL_ExperimentalSetup()
numeroBIFs=[experience.getBIF(x) for x in range(experience.nBIFs())]

print numeroBIFs

    
#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()

#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

hitOccupancy=ROOT.RawHit_Occupancy_Listener()
masterReader.registerDataListener(hitOccupancy)

BIFListener=[ROOT.BIF_Data_Listener(x) for x in numeroBIFs]
for BIFListen in BIFListener:
  masterReader.registerDataListener(BIFListen)

#open file and event loop
lcReader.open( inputFileNames )
#lcReader.readStream(1000)
lcReader.readStream()

#end of event loop
for BIFListen in BIFListener:
  BIFListen.printMaxDelay()

rootFile=ROOT.TFile("noise.root"  , "RECREATE")
#load ROOT library missing
ROOT.TH1F
d=rootFile.mkdir("RawHits")
hitOccupancy.saveTo(d,experience)

dd=rootFile.mkdir("BIFDelay")
for BIFListen in BIFListener:
  sub_dd=dd.mkdir("BIF_{}".format(BIFListen.BIF_number()))
  BIFListen.saveTo(sub_dd)

rootFile.Close()

lcReader.close()
