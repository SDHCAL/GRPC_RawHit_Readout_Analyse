#!/usr/bin/python

#to be executed in the lib directory where all the needed libraries are installed 
# usage :
#  python <name>.py  list_of_slcio_files
#  or
#  python <name>.py -d directory -r list_of_run_numbers
#

#########################################################################################
##                   Documentation
##
##  experience describes the experimental setup (which DIF is where, ... )
##  BIFtriggerWindow is the clock tick shift between signal of the particle
##                   interaction in the Cerenkov detector on the beam line
##                   and the one of particles interacting in the detector
##  lcReader is the LCIO file event reader
##
##  The DataListener architecture is the following
##     master (directly connected to lcReader)
##     ^  ^ ^
##     |  | |
##     |  | timePlotListener (plot where are negative time stamps, can be omitted in production)
##     |  |
##     |  BIFListener (used to determine the BIFtriggerWindow, can be omitted in production)
##     |
##     trivent (the time clustering algorithm)
##     ^    ^
##     |    |
##     |    BIFListener_check (used to cross check the BIFtriggerWindow, can be omitted in production)
##     |
##     filter (filters events selected by trivent, see below)
##     ^
##     |
##     LCIOoutputWriter (Write LCIO output : both original RawCalorimeterHit ans CalorimeterHit collection
##
##  the filters
##
## Current filters are for september 2022 SDHCAL test beam
##
##   first filter is bigNoiseFilter : reject events that have more than 15000 hits.
##   second filter is CL_Filter : reject events with less than 6 consecutive fired layer (allowing one hole among the consecutive layer count)
##   third filter is RamfullFilter : reject events containing a DIF with more than 36 RAMfull channels (channel 29 and 31) if this DIF represents more than 80% of all hits in the event.
##   fourth filter is singleAsicNoise_Filter : reject events if a single ASIC contains more than 80 percent of all the event hits.
##   fifth filter is multipleAsicNoise_Filter : reject events containing at least 3 ASICs having more than 80 hits each.
## 
## Filter status for GIF like analysis was :
##  first filter is doubleFilter (planFilter,CL_Filter) : reject events rejected by the 2 filters :
##                                planFilter : reject events with no hits in layer 0,1,2
##                                CL_Filter  : reject events with less than 6 consecutive fired layer (allowing one hole among the consecutive layer count)
##  second filter is RamfullFilter : reject events containing a DIF with more than 36 RAMfull channels (channel 29 and 31) if this DIF represents more than 80% of all hits in the event.
##
#########################################################################################

from InputOutputFileNames import generateFileList
import sys
import ROOT
ROOT.gROOT.Reset()
ROOT.gSystem.Load('liblcio')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')
if ROOT.gROOT.GetVersion()[0]=='6':
  #ROOT 6.08.02 don't understand non template dictionnary without it (don't know why)
  dummy=ROOT.intervalle('unsigned int')()

IOnames=generateFileList()
  
inputFileNames=ROOT.vector("string")()

filelist=set(IOnames[0])
if len(filelist)==0:
  sys.exit("No input files to process")
   
for f in filelist:
    inputFileNames.push_back(f);

for file in inputFileNames:
    print (file)


    
outputFileName=IOnames[1]+'.slcio'
print ("output file name is ", outputFileName)

    
#experience=ROOT.CERN_SPS_Sept2018_SDHCAL_ExperimentalSetup()
experience=ROOT.CERN_SPS_H2_Sept2022_part1_SDHCAL_ExperimentalSetup()
numeroBIFs=[experience.getBIF(x) for x in range(experience.nBIFs())]

print (numeroBIFs)
#For the moment, put the same time window for all BIFs
BIFtriggerWindow=ROOT.intervalle('int')(-9,-5)

    
#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()

#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent("DHCALRawHits",True,10)
lcReader.registerLCEventListener(masterReader)

BIFListeners=[ROOT.BIF_Data_Listener(x) for x in numeroBIFs]
BIFListeners_timer=[ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(x,"BIFListener_BIF_{}".format(x.BIF_number())) for x in BIFListeners]
for x in BIFListeners_timer:
  masterReader.registerDataListener(x)

timePlotListener=ROOT.RawHit_TimePlot_Listener(experience)
timePlotListener_timer=ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(timePlotListener,"timePlotListener")
masterReader.registerDataListener(timePlotListener_timer)

trivent=ROOT.RawHit_SDHCAL_Data_Reader_Trivent(2,25) # 2=event windows half size, 25=threshold number for hits
trivent.setSkipIfBIFisOutsideReadout(False)
for x in BIFListeners:
  trivent.setBIFparameters(x.BIF_number(),BIFtriggerWindow) # event window = [-2,2] so BIF window = [-9,-4]
#trivent.setUltraVerboseDebugOutput(True)
trivent_timer=ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(trivent,"trivent")
masterReader.registerDataListener(trivent_timer)


BIFListeners_check=[ROOT.BIF_Data_Listener(x) for x in numeroBIFs]
BIFListeners_check_timer=[ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(x,"BIFListener_check_BIF_{}".format(x.BIF_number())) for x in BIFListeners_check]
for x in BIFListeners_check_timer:
  trivent.registerDataListener(x)

#Filters
filter=ROOT.RawHit_SDHCAL_Data_Reader_Event_Filter()

HighNhitFilter=ROOT.MinNumberOfHits_Filter(15000)
bigNoiseFilter=ROOT.Inverse_Filter(HighNhitFilter)
filter.addRejectConditions(bigNoiseFilter)

#planDomain=ROOT.UI_domain(ROOT.UI_intervalle(0,2))
#planFilter=ROOT.NoHitInLayers_Filter(planDomain,experience)
CL_Filter=ROOT.ConsecutiveLayers_Filter(6,experience)
#doubleFilter=ROOT.Both_Reject_Filter(planFilter,CL_Filter)
#filter.addRejectConditions(doubleFilter)
filter.addRejectConditions(CL_Filter)

RamfullFilter=ROOT.RamFull_Filter(36,0.8)
filter.addRejectConditions(RamfullFilter)

singleAsicNoise_Filter=ROOT.HitFractionInOneAsicAbove_Filter(0.8)
filter.addRejectConditions(singleAsicNoise_Filter)

multipleAsicNoise_Filter=ROOT.MaxNumberASICwithMoreThanHits_Filter(80,3)
filter.addRejectConditions(multipleAsicNoise_Filter)

trivent.registerDataListener(filter)


LCIOoutputWriter=ROOT.RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit()
LCIOoutputWriter.open(outputFileName)
LCIOoutputWriter_timer=ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(LCIOoutputWriter,"LCIOoutputWriter")
filter.registerDataListener(LCIOoutputWriter_timer)



LCIOoutputWriter_extra=ROOT.RawHit_SDHCAL_Data_LCWriter_CalorimeterHit(experience)
LCIOoutputWriter_extra.setCorrectHR2(True)
LCIOoutputWriter.addLCWriter(LCIOoutputWriter_extra)

#open file and event loop
lcReader.open( inputFileNames )
#File DHCAL_744193_I0_0.slcio has Tricot data at least in readout 130 / 179 / 214 / 446 / 45223 / 525 / 575 / 678 / 749 / 778 / 793 / 
#Only one hit per trivent event
#For readout 130, the corresponding event is at event 28 (over 87)
#lcReader.skipNEvents(129)
#lcReader.readStream(1)
lcReader.readStream()

#end of event loop
for x in BIFListeners_check:
  x.printMaxDelay()

rootFile=ROOT.TFile(IOnames[1]+"_check.root"  , "RECREATE")
#load ROOT library missing
ROOT.TH1F

d=rootFile.mkdir("BIFDelay")
for x in BIFListeners:
  sub_d=d.mkdir("BIF_{}".format(x.BIF_number()))
  x.saveTo(sub_d)

dd=rootFile.mkdir("BIFDelay_check")
for x in BIFListeners_check:
  sub_dd=dd.mkdir("BIF_{}".format(x.BIF_number()))
  x.saveTo(sub_dd)

dtime=rootFile.mkdir("TimePlot")
timePlotListener.saveTo(dtime)
#timePlotListener.printReport()
rootFile.Close()

masterReader.writeSpillInfoStatShort()
masterReader.writeSpillInfoStatExtendedInFile()

lcReader.close()
LCIOoutputWriter.close()
