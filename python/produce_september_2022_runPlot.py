#!/usr/bin/python

import array
import ROOT
ROOT.gROOT.Reset()
ROOT.gSystem.Load('liblcio')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')
if ROOT.gROOT.GetVersion()[0]=='6':
  #ROOT 6.26.04 don't understand non template dictionnary without it (don't know why) and can't get even non template dict without this line
  dummy=ROOT.intervalle('unsigned int')()

exp=ROOT.CERN_SPS_H2_Sept2022_part1_SDHCAL_ExperimentalSetup()
all=ROOT.all_ConfigInfo.instance()
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)
palette=array.array('i',[4,1,3,2,6])

ROOT.gStyle.SetPalette(5,palette)

a=all.getRunNumberList()
runMin=min(a)
runMax=max(a)
histoXmin=runMin
histoXmax=runMax+1
histoNbins=histoXmax-histoXmin


grRunBeam=ROOT.TGraph()
grRunBeam.SetMarkerStyle(20)
grRunBeam.SetMarkerSize(0.5)
grRunPos=[ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph()]
for gr in grRunPos:
    gr.SetMarkerStyle(20)
    gr.SetMarkerSize(0.5)
    gr.SetLineColor(0)
grRunPos[0].SetTitle("unknown")
grRunPos[1].SetTitle("test run")
grRunPos[2].SetTitle("electron")
grRunPos[3].SetTitle("muon")
grRunPos[4].SetTitle("pion")
for run in a:
    beam=all.getBeamConditions(run)
    grRunBeam.AddPoint(run,beam.getParticle()+0.5)
    grRunPos[beam.getParticle()].AddPoint(run,beam.getBeamPosition()+0.5)


hRunBeamParticle=ROOT.TH2F("particle","Particle beam",histoNbins,histoXmin,histoXmax,5,0,5)
hRunBeamParticle.SetMarkerStyle(20)
hRunBeamParticle.SetMarkerSize(0.5)
hRunBeamParticle.GetXaxis().SetTitle("run number")
hRunBeamParticle.GetYaxis().SetBinLabel(1,"unknown"); 
hRunBeamParticle.GetYaxis().SetBinLabel(2,"test"); 
hRunBeamParticle.GetYaxis().SetBinLabel(3,"electron"); 
hRunBeamParticle.GetYaxis().SetBinLabel(4,"muon"); 
hRunBeamParticle.GetYaxis().SetBinLabel(5,"pion"); 
canvas=ROOT.TCanvas()
hRunBeamParticle.Draw()
grRunBeam.Draw("p")
canvas.Update()

hRunBeamPosition=ROOT.TH2F("BeamPosition","Beam particle ",histoNbins,histoXmin,histoXmax,7,0,7)
hRunBeamPosition.SetLineColor(0)
hRunBeamPosition.SetMarkerColor(0)
hRunBeamPosition.GetXaxis().SetTitle("run number")
hRunBeamPosition.GetYaxis().SetTitle("Beam position")
hRunBeamPosition.GetYaxis().CenterTitle(True)
hRunBeamPosition.GetYaxis().SetBinLabel(1,"unknown"); 
hRunBeamPosition.GetYaxis().SetBinLabel(2,"center"); 
hRunBeamPosition.GetYaxis().SetBinLabel(3,"right"); 
hRunBeamPosition.GetYaxis().SetBinLabel(4,"top right"); 
hRunBeamPosition.GetYaxis().SetBinLabel(5,"top"); 
hRunBeamPosition.GetYaxis().SetBinLabel(6,"bottom left"); 
hRunBeamPosition.GetYaxis().SetBinLabel(7,"bottom right"); 
canvasB=ROOT.TCanvas()
canvasB.SetLeftMargin(0.15);
hRunBeamPosition.Draw()
grRunPos[0].Draw("P PMC")
grRunPos[1].Draw("P PMC")
grRunPos[3].Draw("P PMC")
grRunPos[4].Draw("P PMC")
canvasB.BuildLegend()
canvasB.Update()




print (min(a),max(a))
a=input("press")

