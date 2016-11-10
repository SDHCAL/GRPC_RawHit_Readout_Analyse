#!/usr/bin/python

#to be executed in the lib directory where all the needed libraries are installed 
# usage python <name>.py  list_of_slcio_files
import grpc
import sys
import ROOT
ROOT.gROOT.Reset()

inputFileNames=grpc.std_string_vec()
for f in sys.argv[1:]:
    inputFileNames.push_back(f);

for file in inputFileNames:
    print file

experience=grpc.GIF_oct2016_ExperimentalSetup()
numeroBIF=experience.getBIF();

print numeroBIF

BIFtriggerWindow=grpc.uint_intervalle(6,8)


#start LCIO reader
lcReader=grpc.LCFactory_getInstance().createLCReader()

#create architecture of listeners
masterReader=grpc.RawHit_SDHCAL_Data_Reader_From_LCEvent()
lcReader.registerLCEventListener(masterReader)

allHitOccupancy=grpc.RawHit_Occupancy_Listener()
masterReader.registerDataListener(allHitOccupancy)
BIF_splitter=grpc.RawHit_SDHCAL_Data_Reader_FromBIF(numeroBIF,BIFtriggerWindow)
masterReader.registerDataListener(BIF_splitter)
triggeredHitOccupancy=grpc.RawHit_Occupancy_Listener()
BIF_splitter.registerDataListener(triggeredHitOccupancy)
BIFtrigger_subdata=grpc.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIF_splitter.registerDataListener(BIFtrigger_subdata)
triggeredHitOccupancyForGIFoct2016efficiency=grpc.RawHit_Occupancy_Listener()
BIFtrigger_subdata.registerDataListener(triggeredHitOccupancyForGIFoct2016efficiency)

BIFtrigger_subdata_thresh2=grpc.RawHit_SDHCAL_Data_Reader_Threshold_Filter(2)
BIFtrigger_subdata_thresh3=grpc.RawHit_SDHCAL_Data_Reader_Threshold_Filter(3)
BIFtrigger_subdata.registerDataListener(BIFtrigger_subdata_thresh2)
BIFtrigger_subdata.registerDataListener(BIFtrigger_subdata_thresh3)
BIFtrigger_subdata_thresh2_print=grpc.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIFtrigger_subdata_thresh3_print=grpc.RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
BIFtrigger_subdata_thresh2.registerDataListener(BIFtrigger_subdata_thresh2_print)
BIFtrigger_subdata_thresh3.registerDataListener(BIFtrigger_subdata_thresh3_print)


#open file and event loop
lcReader.open( inputFileNames )
lcReader.readStream()

#end of event loop
rootFile=ROOT.TFile("test.root"  , "RECREATE")
d=rootFile.mkdir("AllData")
allHitOccupancy.saveTo_wrap(ROOT.AsCObject(d),experience)
triggeredHitOccupancy.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("BIFtriggedData")),experience)
triggeredHitOccupancyForGIFoct2016efficiency.saveTo_wrap(ROOT.AsCObject(rootFile.mkdir("BIFtriggedDataTimeAndSpace")),experience)
BIFtrigger_subdata.reportAndSaveTo_wrap(ROOT.AsCObject(rootFile.mkdir("GapPlaneEfficiencies")))
print "threshold 2"
BIFtrigger_subdata_thresh2_print.reportAndSaveTo_wrap(ROOT.AsCObject(rootFile.mkdir("GapPlaneEfficiencies_2")))  #faut que je trouve comment passer un pointeur nul
print "threshold 3"
BIFtrigger_subdata_thresh3_print.reportAndSaveTo_wrap(ROOT.AsCObject(rootFile.mkdir("GapPlaneEfficiencies_3")))

rootFile.Close()

lcReader.close()
