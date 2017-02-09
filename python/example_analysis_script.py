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

BIFtriggerWindow=ROOT.intervalle('unsigned int')(6,8)


#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()

#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

allHitOccupancy=ROOT.RawHit_Occupancy_Listener()
masterReader.registerDataListener(allHitOccupancy)
BIF_splitter=ROOT.RawHit_SDHCAL_Data_Reader_FromBIF(numeroBIF,BIFtriggerWindow)
masterReader.registerDataListener(BIF_splitter)
triggeredHitOccupancy=ROOT.RawHit_Occupancy_Listener()
BIF_splitter.registerDataListener(triggeredHitOccupancy)
BIFtrigger_subdata=ROOT.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIF_splitter.registerDataListener(BIFtrigger_subdata)
triggeredHitOccupancyForGIFoct2016efficiency=ROOT.RawHit_Occupancy_Listener()
BIFtrigger_subdata.registerDataListener(triggeredHitOccupancyForGIFoct2016efficiency)
effPlanOccupancy=ROOT.RawHit_Plan_Occupancy_Listener(experience)
BIFtrigger_subdata.registerDataListener(effPlanOccupancy)



#open file and event loop
lcReader.open( inputFileNames )
lcReader.readStream()

#end of event loop
rootFile=ROOT.TFile("test.root"  , "RECREATE")
#load ROOT library missing
ROOT.TH1F
d=rootFile.mkdir("AllData")
allHitOccupancy.saveTo(d,experience)
triggeredHitOccupancy.saveTo(rootFile.mkdir("BIFtriggedData"),experience)
triggeredHitOccupancyForGIFoct2016efficiency.saveTo(rootFile.mkdir("BIFtriggedDataTimeAndSpace"),experience)
effPlanOccupancy.saveTo(rootFile.mkdir("GapPlaneEfficiencies"))
effPlanOccupancy.efficiencyReport()

rootFile.Close()

lcReader.close()
