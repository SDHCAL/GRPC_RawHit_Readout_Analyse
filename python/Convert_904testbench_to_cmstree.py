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

runListText=""
for run in set(runNumbers):
    runListText=runListText+"{}_".format(run)
runListText=runListText[:-1] #remove last character to the string
    
filelist=set(filelist)
if len(filelist)==0 :
    sys.exit("No file for this run number "+str(runNumbers))


inputFileNames=ROOT.vector("string")()
for file in filelist:
    inputFileNames.push_back(file)

for file in inputFileNames:
    print (file)

experience=ROOT.ExperimentalSetup()
experience.addCMSstrip(66,69); #glass
experience.addCMSstrip(26,23); #bakelite

triggerStartWindow=12
triggerWindowDataLength=6


maxEvt=-1

#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()


#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

trig_reader=ROOT.RawHit_SDHCAL_Data_Reader_TriggeredMode(triggerWindowDataLength,triggerStartWindow)
masterReader.registerDataListener(trig_reader);

treeWriter=ROOT.CMSTree_Writer_NoBIF(experience,0)
treeWriter.setAddEntryForEventsWithNoHits(True)
trig_reader.registerDataListener(treeWriter)

planOccupancy=ROOT.RawHit_Plan_Occupancy_Listener(experience)
trig_reader.registerDataListener(planOccupancy)

hitOccupancy=ROOT.RawHit_Occupancy_Listener()
trig_reader.registerDataListener(hitOccupancy)

#open file and init loop
lcReader.open( inputFileNames )
cmsrootfile=ROOT.TFile("Scan{}HV1_DAQ.root".format(runListText), "RECREATE") 
treeWriter.createTree()

#event loop
if maxEvt>0:
    lcReader.readStream(maxEvt)
else:
    lcReader.readStream()

#end of event loop
treeWriter.getTree().Write();
#load ROOT library missing
ROOT.TH1F
planOccupancy.saveTo(cmsrootfile.mkdir("occupanciesPlan"))
hitOccupancy.saveTo(cmsrootfile.mkdir("occupanciesHit"),experience)
cmsrootfile.Close() ;

planOccupancy.noiseReport()
planOccupancy.efficiencyReport()

    
lcReader.close()
