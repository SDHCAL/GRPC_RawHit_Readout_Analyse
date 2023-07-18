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
#numeroBIF=experience.getBIF()
#print numeroBIF

BIFtriggerWindow=ROOT.intervalle('unsigned int')(6,8)
BIFtriggerWindowNoise=ROOT.intervalle('int')(-BIFtriggerWindow.second,-BIFtriggerWindow.first)
NoiseWindowLength=50; #10 microsecond
decalage=10
BIFtriggerWindowShifted=ROOT.intervalle('unsigned int')(BIFtriggerWindow.first+decalage,BIFtriggerWindow.second+decalage)

#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()

######################################################
##            master
##   _________^   ^___________________________________________________________
##   |                  |                    |                |                  |
##   Occupancy_Plots    Clusterer
##                      ^
##                      |
##                      ClusterCopier
##                      ^
##                      |                 
##                      Occupancy_Plots   

#create defaut merge predicate
mergeCondition=ROOT.RawHit_standard_merge_predicate(experience)

#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

allHitOccupancy=ROOT.RawHit_Occupancy_Listener()
masterReader.registerDataListener(allHitOccupancy)

allClusterer=ROOT.RawHit_SDHCAL_Data_Reader_Clusterer('RawHit_standard_merge_predicate')(mergeCondition,True)
WrapTime=ROOT.Wrap_TimeCount_RawHit_SDHCAL_Data_Listener(allClusterer,"clusterer")
masterReader.registerDataListener(WrapTime)


clusterReader=ROOT.RawHit_SDHCAL_Data_Reader_FromClusters(experience)
allClusterer.registerDataListener(clusterReader)

allOccupancy=ROOT.RawHit_Occupancy_Listener()
clusterReader.registerDataListener(allOccupancy)

allClusterOccupancy=ROOT.RawHit_Plan_Cluster_Occupancy_Listener(experience)
allClusterer.registerDataListener(allClusterOccupancy)

#open file and event loop
lcReader.open( inputFileNames )
lcReader.readStream(10)

#end of event loop
rootFileName="clusters"
for run in set(runNumbers):
  rootFileName=rootFileName+"_"+run
rootFileName=rootFileName+".root"
print rootFileName

rootFile=ROOT.TFile(rootFileName  , "RECREATE")

#load ROOT library missing
ROOT.TH1F
allHitOccupancy.saveTo(rootFile.mkdir("AllData"),experience)
allOccupancy.saveTo(rootFile.mkdir("AllCluster"),experience)
allClusterOccupancy.saveTo(rootFile.mkdir("AllClusterSize"))

rootFile.Close()

lcReader.close()
