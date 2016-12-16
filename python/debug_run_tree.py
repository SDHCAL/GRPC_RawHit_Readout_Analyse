#!/usr/bin/python

#use the tree produced by produce_run_tree_GIF.py

import os
from array import array
import ROOT
ROOT.gROOT.Reset()

ROOT.gSystem.Load('liblcio')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')

f = ROOT.TFile( 'GIF_oct2016.root')
tree = ROOT.gDirectory.Get( 'results' )
entries = tree.GetEntriesFast()

debugFile=ROOT.std.ofstream("debug_file_bis.txt")

for ientry in xrange( entries ):
    nb=tree.GetEntry( ientry )
    if nb <= 0:
        continue
    tree.Noise.write(debugFile)
    tree.Efficiency.write(debugFile)
    tree.FakeEfficiency.write(debugFile)

f.Close()
