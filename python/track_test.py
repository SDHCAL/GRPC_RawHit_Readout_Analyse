#!/usr/bin/python

#to be executed in the lib directory where all the needed libraries are installed or set LD_LIBRARY_PATH to contain the lib directory
#it uses the pyroot ROOT dictionnary interface 
# usage python <name>.py  directory_where_slcio_files_are list_of_run_numbers

import sys
import glob
import ROOT
ROOT.gROOT.Reset()

ROOT.gSystem.Load('liblcio')
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
    print file


experience=ROOT.GIF_oct2016_ExperimentalSetup()
numeroBIF=experience.getBIF()

print numeroBIF

BIFtriggerWindow=ROOT.intervalle('int')(-8,-6)

#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()

######################################################
##            master
##            ^
##            |
##            Horizontal_Track_Selector
##            ^____________________
##            |                   |
##            Occupancy_Plots     Plan_occupancy

#create defaut merge predicate
mergeCondition=ROOT.RawHit_horizontal_track_predicate(experience)

#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

trackReader=ROOT.RawHit_SDHCAL_Data_Reader_HorizontalTrack(experience,True)
trackReader.setBIFtimeWindow(BIFtriggerWindow)
WrapTime=ROOT.Wrap_TimeCount_RawHit_SDHCAL_Data_Listener(trackReader,"tracker")
masterReader.registerDataListener(WrapTime)

trackReaderGIF2016=ROOT.RawHit_SDHCAL_Data_Reader_tracker_GIFoct2016()
trackReader.registerDataListener(trackReaderGIF2016)

Occupancy=ROOT.RawHit_Occupancy_Listener()
WrapTimeB=ROOT.Wrap_TimeCount_RawHit_SDHCAL_Data_Listener(Occupancy,"occupancy")
trackReader.registerDataListener(WrapTimeB)

effPlanOccupancy=ROOT.RawHit_Plan_Occupancy_Listener(experience)
WrapTimeC=ROOT.Wrap_TimeCount_RawHit_SDHCAL_Data_Listener(effPlanOccupancy,"planOccupancy")
trackReader.registerDataListener(WrapTimeC)
effPlanOccupancyGIF2016=ROOT.RawHit_Plan_Occupancy_Listener(experience)
trackReaderGIF2016.registerDataListener(effPlanOccupancyGIF2016)
OccupancyGIF2016=ROOT.RawHit_Occupancy_Listener()
trackReaderGIF2016.registerDataListener(OccupancyGIF2016)



#open file and event loop
lcReader.open( inputFileNames )
lcReader.readStream(100)

#end of event loop
rootFileName="tracks"
for run in set(runNumbers):
  rootFileName=rootFileName+"_"+run
rootFileName=rootFileName+".root"
print rootFileName

rootFile=ROOT.TFile(rootFileName  , "RECREATE")

#load ROOT library missing
ROOT.TH1F
Occupancy.saveTo(rootFile.mkdir("DIF"),experience)
effPlanOccupancy.saveTo(rootFile.mkdir("effPlan"))
effPlanOccupancyGIF2016.saveTo(rootFile.mkdir("effPlanGIF"))
OccupancyGIF2016.saveTo(rootFile.mkdir("DIFiftracks"),experience)

effPlanOccupancy.efficiencyReport()
effPlanOccupancyGIF2016.efficiencyReport()

rootFile.Close()

lcReader.close()
