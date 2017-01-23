#!/usr/bin/python

#results are assumed to be in the directory given as argument to the script 
#the directory should have the following structure :
# "argv[1]"/RunNumber/report_RunNumber.txt
#  argv[1] should only contains subdirectory whose names corresponds to run numbers and contains the expected report file

import sys
import glob
import os.path
import ROOT
from array import array
ROOT.gROOT.Reset()

ROOT.gSystem.Load('liblcio')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')
if ROOT.gROOT.GetVersion()[0]=='6':
  #ROOT 6.08.02 don't understand non template dictionnary without it (don't know why)
  dummy=ROOT.intervalle('unsigned int')()

directory=sys.argv[1]
resultsDirectory=glob.glob(directory+'/*')

experience=ROOT.GIF_oct2016_ExperimentalSetup()
all=ROOT.all_ConfigInfo.instance()


runNumBranchObject = array( 'I', [ 0 ] )
hasConfBranchObject = array( 'I', [ 0 ] )
hasGIFCondBranchObject = array( 'I', [ 0 ] )
Noise = ROOT.RunThresholdCounter('MappedCounters<MappedCounters<SingleCounter> >')() 
Eff = ROOT.RunThresholdCounter('MappedCounters<MappedCounters<SingleCounter> >')() 
FakeEff = ROOT.RunThresholdCounter('MappedCounters<MappedCounters<SingleCounter> >')() 
ConfigInfo = ROOT.Setup_ConfigInfo()
GIFconditions = ROOT.GIF_Conditions()
ConfSetup=ROOT.std.string()

f = ROOT.TFile( 'GIF_oct2016.root', 'RECREATE' )
tree = ROOT.TTree( 'results', 'Run analysis tree' )

tree.Branch( 'runNumber',runNumBranchObject , 'runNumber/i' )
tree.Branch( 'Noise', Noise, 32000, 0 )
tree.Branch( 'Efficiency', Eff, 32000, 0 )
tree.Branch( 'FakeEfficiency', FakeEff, 32000, 0 )
tree.Branch( 'hasConf', hasConfBranchObject, 'hasConf/i' )
tree.Branch( 'ConfigName', ConfSetup )
tree.Branch( 'ConfigInfo', ConfigInfo, 32000, 0 )
tree.Branch( 'hasGIFCond', hasGIFCondBranchObject, 'hasGIFCond/i' )
tree.Branch( 'GIFcond', GIFconditions)

produceDebugFile=False
if (produceDebugFile):
  debugFile=ROOT.std.ofstream("debug_file.txt")

for dir in sorted(resultsDirectory) :
  runNum=os.path.basename(dir)
  filename=dir+'/report_'+runNum+'.txt'
  runNumber=int(runNum)
  print runNumber
  runNumBranchObject[0]=runNumber
  ConfSetup.replace(0, ROOT.std.string.npos, all.getConfigName(runNumber) )
  file=ROOT.std.ifstream(filename)
  Noise.ASCIIread(file)
  Eff.ASCIIread(file)
  FakeEff.ASCIIread(file)
  if (produceDebugFile):
    Noise.write(debugFile)
    Eff.write(debugFile)
    FakeEff.write(debugFile)
  hasConfBranchObject[0]=ConfigInfo.setToRun(runNumber)
  hasGIFCondBranchObject[0]=GIFconditions.setToRun(runNumber)
  tree.Fill()


f.Write()
f.Close()
