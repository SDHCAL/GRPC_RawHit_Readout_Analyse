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
if len(filelist)==0 :
    sys.exit("No file for this run number "+str(runNumbers))


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
decalage=10
BIFtriggerWindowShifted=grpc.uint_intervalle(BIFtriggerWindow.first+decalage,BIFtriggerWindow.second+decalage)

#start LCIO reader
lcReader=grpc.LCFactory_getInstance().createLCReader()

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
masterReader=grpc.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

allHitOccupancy=grpc.RawHit_Occupancy_Listener()
masterReader.registerDataListener(allHitOccupancy)
BIF_splitter=grpc.RawHit_SDHCAL_Data_Reader_FromBIF(numeroBIF,BIFtriggerWindow)
masterReader.registerDataListener(BIF_splitter)
BIF_splitter_shift=grpc.RawHit_SDHCAL_Data_Reader_FromBIF(numeroBIF,BIFtriggerWindowShifted)
masterReader.registerDataListener(BIF_splitter_shift)
noiseReader=grpc.RawHit_SDHCAL_Data_Reader_Noise(experience,NoiseWindowLength)
noiseReader.setVetoOnBIF()
noiseReader.setBIFtimeWindow(BIFtriggerWindowNoise)
noiseReader.setMaxEventsToSend(1000000)
masterReader.registerDataListener(noiseReader)

BIFtrigger_subdata=grpc.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIF_splitter.registerDataListener(BIFtrigger_subdata)
triggeredHitOccupancy=grpc.RawHit_Occupancy_Listener()
BIF_splitter.registerDataListener(triggeredHitOccupancy)
BIFtrigger_subdata_shift=grpc.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIF_splitter_shift.registerDataListener(BIFtrigger_subdata_shift)
triggeredHitOccupancy_shift=grpc.RawHit_Occupancy_Listener()
BIF_splitter_shift.registerDataListener(triggeredHitOccupancy_shift)
noisePlanOccupancy=grpc.RawHit_Plan_Occupancy_Listener(experience)
noiseReader.registerDataListener(noisePlanOccupancy)

effPlanOccupancy=grpc.RawHit_Plan_Occupancy_Listener(experience)
BIFtrigger_subdata.registerDataListener(effPlanOccupancy)
effPlanOccupancy_shift=grpc.RawHit_Plan_Occupancy_Listener(experience)
BIFtrigger_subdata_shift.registerDataListener(effPlanOccupancy_shift)

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
triggeredHitOccupancy_shift.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("BIFtriggedData_shift")),experience)
noisePlanOccupancy.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("GapPlaneNoise")))
effPlanOccupancy.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("GapPlaneEfficiencies")))
effPlanOccupancy_shift.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("GapPlaneEfficiencies_shift")))

noisePlanOccupancy.noiseReport()
effPlanOccupancy.efficiencyReport()
effPlanOccupancy_shift.efficiencyReport()
outputReport='report_'+runNumbers[0]+'.txt'
noisePlanOccupancy.runSummary().ASCIIfilewrite(outputReport)
effPlanOccupancy.runSummary().ASCIIfileappend(outputReport)
effPlanOccupancy_shift.runSummary().ASCIIfileappend(outputReport)

rootFile.Close()

lcReader.close()
