#!/usr/bin/python

#to be executed in the lib directory where all the needed libraries are installed or set LD_LIBRARY_PATH to contain the lib directory
#it uses the pyroot ROOT dictionnary interface
# input files names should be with .slcio extension
# usage python <name>.py  directory list_of_run_numbers

import sys
import glob
import ROOT
ROOT.gROOT.Reset()

ROOT.gSystem.Load('liblcio')
#ROOT.gSystem.Load('liblcioDict')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')
if ROOT.gROOT.GetVersion()[0]=='6':
  #ROOT 6.08.02 don't understand non template dictionnary without it (don't know why)
  dummy=ROOT.intervalle('unsigned int')()



directory=sys.argv[1]
runNumbers=sys.argv[2:]

filelist=[]

for run in runNumbers:
    filelist.extend( glob.glob(directory+'/*'+run+'*.slcio') )

filelist=set(filelist)
if len(filelist)==0 :
    sys.exit("No file for this run number "+str(runNumbers))


inputFileNames=ROOT.vector("string")()
for file in filelist:
    inputFileNames.push_back(file)

for file in inputFileNames:
    print (file)


#In GIF oct2016, there was one BIF serving as trigger signal
experience=ROOT.GIF_oct2016_ExperimentalSetup()
numeroBIF=experience.getBIF(0)

print (numeroBIF)

BIFtriggerWindow=ROOT.intervalle('unsigned int')(6,8)
BIFtriggerWindowNoise=ROOT.intervalle('int')(-BIFtriggerWindow.second,-BIFtriggerWindow.first)
NoiseWindowLength=50; #10 microsecond
decalage=10
BIFtriggerWindowShifted=ROOT.intervalle('unsigned int')(BIFtriggerWindow.first+decalage,BIFtriggerWindow.second+decalage)

#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()

######################################################
##            master
##   _________^   ^_______________________________________
##   |              |                    |                |
##   BIF_trigged    BIF_trigged_shift    NoiseReader      Occupancy_Plots   
##   ^     ^_____    ^       ^           ^_____
##   |           |   |       |                |
##   BIF_spaced  Occupancy   BIF_spaced       Plan_occupancy (noise)
##   ^                       ^
##   |                       |
##   Plan_occupancy (eff)    Plan_occupancy (fake eff)

#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

allHitOccupancy=ROOT.RawHit_Occupancy_Listener()
masterReader.registerDataListener(allHitOccupancy)

BIF_splitter=ROOT.RawHit_SDHCAL_Data_Reader_FromBIF(numeroBIF,BIFtriggerWindow)
masterReader.registerDataListener(BIF_splitter)
BIF_splitter_shift=ROOT.RawHit_SDHCAL_Data_Reader_FromBIF(numeroBIF,BIFtriggerWindowShifted)
masterReader.registerDataListener(BIF_splitter_shift)
noiseReader=ROOT.RawHit_SDHCAL_Data_Reader_Noise(experience,NoiseWindowLength)
noiseReader.setVetoOnBIF()
noiseReader.setBIFtimeWindow(BIFtriggerWindowNoise)
noiseReader.setMaxEventsToSend(1000000)
masterReader.registerDataListener(noiseReader)

BIFtrigger_subdata=ROOT.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIF_splitter.registerDataListener(BIFtrigger_subdata)
triggeredHitOccupancy=ROOT.RawHit_Occupancy_Listener()
BIF_splitter.registerDataListener(triggeredHitOccupancy)
BIFtrigger_subdata_shift=ROOT.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIF_splitter_shift.registerDataListener(BIFtrigger_subdata_shift)
triggeredHitOccupancy_shift=ROOT.RawHit_Occupancy_Listener()
BIF_splitter_shift.registerDataListener(triggeredHitOccupancy_shift)
noisePlanOccupancy=ROOT.RawHit_Plan_Occupancy_Listener(experience)
noiseReader.registerDataListener(noisePlanOccupancy)

effPlanOccupancy=ROOT.RawHit_Plan_Occupancy_Listener(experience)
BIFtrigger_subdata.registerDataListener(effPlanOccupancy)
effPlanOccupancy_shift=ROOT.RawHit_Plan_Occupancy_Listener(experience)
BIFtrigger_subdata_shift.registerDataListener(effPlanOccupancy_shift)

#open file and event loop
lcReader.open( inputFileNames )
lcReader.readStream()

#end of event loop
rootFileName="analyse"
for run in set(runNumbers):
  rootFileName=rootFileName+"_"+run
rootFileName=rootFileName+".root"
print (rootFileName)

rootFile=ROOT.TFile(rootFileName  , "RECREATE")

#load ROOT library missing
ROOT.TH1F
allHitOccupancy.saveTo(rootFile.mkdir("AllData"),experience)
triggeredHitOccupancy.saveTo(rootFile.mkdir("BIFtriggedData"),experience)
triggeredHitOccupancy_shift.saveTo(rootFile.mkdir("BIFtriggedData_shift"),experience)
noisePlanOccupancy.saveTo(rootFile.mkdir("GapPlaneNoise"))
effPlanOccupancy.saveTo(rootFile.mkdir("GapPlaneEfficiencies"))
effPlanOccupancy_shift.saveTo(rootFile.mkdir("GapPlaneEfficiencies_shift"))

noisePlanOccupancy.noiseReport()
effPlanOccupancy.efficiencyReport()
effPlanOccupancy_shift.efficiencyReport()
outputReport='report_'+runNumbers[0]+'.txt'
noisePlanOccupancy.runSummary().ASCIIfilewrite(outputReport)
effPlanOccupancy.runSummary().ASCIIfileappend(outputReport)
effPlanOccupancy_shift.runSummary().ASCIIfileappend(outputReport)

rootFile.Close()

lcReader.close()
