#!/usr/bin/python

#to be executed in the lib directory where all the needed libraries are installed 
# usage python <name>.py  list_of_slcio_files
import grpc
import sys
import glob
import ROOT
ROOT.gROOT.Reset()

directory=sys.argv[1]
runNumbers=sys.argv[2:]

filelist=[]

for run in runNumbers:
    filelist.extend( glob.glob(directory+'/*'+run+'*.slcio') )

filelist=set(filelist)

inputFileNames=grpc.std_string_vec()
for file in filelist:
    inputFileNames.push_back(file)

for file in inputFileNames:
    print file


experience=grpc.GIF_oct2016_ExperimentalSetup()
numeroBIF=experience.getBIF();

print numeroBIF

BIFtriggerWindow=grpc.uint_intervalle(6,8)
BIFtriggerWindowNoise=grpc.int_intervalle(-BIFtriggerWindow.second,-BIFtriggerWindow.first)
NoiseWindowLength=50; #10 microsecond


#start LCIO reader
lcReader=grpc.LCFactory_getInstance().createLCReader()

######################################################
##            master
##   _________^   ^____________________
##   |               |                |
##   BIF_trigged     NoiseReader      Occupancy_Plots   
##   ^     ^_____         ^_____
##   |           |             |
##   BIF_spaced  Occupancy     Plan_occupancy (noise)
##   ^
##   |
##   Plan_occupancy (eff)

#create architecture of listeners
masterReader=grpc.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

allHitOccupancy=grpc.RawHit_Occupancy_Listener()
masterReader.registerDataListener(allHitOccupancy)
BIF_splitter=grpc.RawHit_SDHCAL_Data_Reader_FromBIF(numeroBIF,BIFtriggerWindow)
masterReader.registerDataListener(BIF_splitter)
noiseReader=grpc.RawHit_SDHCAL_Data_Reader_Noise(experience,NoiseWindowLength)
noiseReader.setVetoOnBIF()
noiseReader.setBIFtimeWindow(BIFtriggerWindowNoise)
noiseReader.setMaxEventsToSend(1000000)
masterReader.registerDataListener(noiseReader)

BIFtrigger_subdata=grpc.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIF_splitter.registerDataListener(BIFtrigger_subdata)
triggeredHitOccupancy=grpc.RawHit_Occupancy_Listener()
BIF_splitter.registerDataListener(triggeredHitOccupancy)
noisePlanOccupancy=grpc.RawHit_Plan_Occupancy_Listener(experience)
noiseReader.registerDataListener(noisePlanOccupancy)

effPlanOccupancy=grpc.RawHit_Plan_Occupancy_Listener(experience)
BIFtrigger_subdata.registerDataListener(effPlanOccupancy)

#open file and event loop
lcReader.open( inputFileNames )
lcReader.readStream()

#end of event loop
rootFileName="analyse"
for run in set(runNumbers):
  rootFileName=rootFileName+"_"+run
rootFileName=rootFileName+".root"
print rootFileName

rootFile=ROOT.TFile(rootFileName  , "RECREATE")

allHitOccupancy.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("AllData")),experience)
triggeredHitOccupancy.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("BIFtriggedData")),experience)
noisePlanOccupancy.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("GapPlaneNoise")))
effPlanOccupancy.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("GapPlaneEfficiencies")))

noisePlanOccupancy.noiseReport()
effPlanOccupancy.efficiencyReport()

rootFile.Close()

lcReader.close()
