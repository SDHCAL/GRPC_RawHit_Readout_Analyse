#!/usr/bin/python

#to be executed in the lib directory where all the needed libraries are installed 
# usage python <name>.py  list_of_slcio_files

import sys
import os
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


    
outputFileName=os.path.splitext(os.path.basename(inputFileNames[0]))[0]+'_TriventSplit.slcio'
print "output file name is ", outputFileName


    
experience=ROOT.CERN_SPS_Sept2018_SDHCAL_ExperimentalSetup()
numeroBIF=experience.getBIF()

print numeroBIF
BIFtriggerWindow=ROOT.intervalle('int')(-7,-6)

    
#start LCIO reader
lcReader=ROOT.IOIMPL.LCFactory.getInstance().createLCReader()

#create architecture of listeners
masterReader=ROOT.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

BIFListener=ROOT.BIF_Data_Listener(numeroBIF)
BIFListener_timer=ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(BIFListener,"BIFListener")
masterReader.registerDataListener(BIFListener_timer)

trivent=ROOT.RawHit_SDHCAL_Data_Reader_Trivent(2,7) # 2=event windows half size, 7=threshold number for hits
trivent.setSkipIfBIFisOutsideReadout(False)
trivent.setBIFparameters(numeroBIF,BIFtriggerWindow) # event window = [-2,2] so BIF window = [-9,-4]
#trivent.setUltraVerboseDebugOutput(True)
trivent_timer=ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(trivent,"trivent")
masterReader.registerDataListener(trivent_timer)


BIFListener_check=ROOT.BIF_Data_Listener(numeroBIF)
BIFListener_check_timer=ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(BIFListener_check,"BIFListener_check")
trivent.registerDataListener(BIFListener_check_timer)

LCIOoutputWriter=ROOT.RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit()
LCIOoutputWriter.open(outputFileName)
LCIOoutputWriter_timer=ROOT.Time_Decorator_For_RawHit_SDHCAL_Data_Listener(LCIOoutputWriter,"LCIOoutputWriter")
trivent.registerDataListener(LCIOoutputWriter_timer)



LCIOoutputWriter_extra=ROOT.RawHit_SDHCAL_Data_LCWriter_CalorimeterHit(experience)
LCIOoutputWriter.addLCWriter(LCIOoutputWriter_extra)

#open file and event loop
lcReader.open( inputFileNames )
#lcReader.skipNEvents(63)
lcReader.readStream(2)
#lcReader.readStream()

#end of event loop
BIFListener_check.printMaxDelay()

rootFile=ROOT.TFile("noisetrivent_check.root"  , "RECREATE")
#load ROOT library missing
ROOT.TH1F

d=rootFile.mkdir("BIFDelay")
BIFListener.saveTo(d)

dd=rootFile.mkdir("BIFDelay_check")
BIFListener_check.saveTo(dd)

rootFile.Close()

lcReader.close()
LCIOoutputWriter.close()
