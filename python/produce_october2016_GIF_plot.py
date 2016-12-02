#!/usr/bin/python

#use the tree produced by produce_run_tree_GIF.py
       

import os
import ROOT
ROOT.gROOT.Reset()

ROOT.gSystem.Load('liblcio')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')


experience=ROOT.GIF_oct2016_ExperimentalSetup()
all=ROOT.all_ConfigInfo.instance()

f = ROOT.TFile( 'GIF_oct2016.root')
tree = ROOT.gDirectory.Get( 'results' )
entries = tree.GetEntriesFast()

minrun=tree.GetMinimum('runNumber')
maxrun=tree.GetMaximum('runNumber')

print minrun,maxrun

hSetupName=ROOT.TH1F("hSetupName","Config Setup Used",2,0,2)
hSetupName.SetBit(ROOT.TH1.kCanRebin)
hSetupVsRun=ROOT.TH2F("hSetupVsRun","Config vs run number",int(maxrun-minrun+1),minrun,maxrun+1,2,0,2)
hSetupVsRun.SetBit(ROOT.TH1.kCanRebin)

hSourceStatus=ROOT.TH1F("hSourceStatus","Source Status",2,0,2)
hSourceStatus.SetBit(ROOT.TH1.kCanRebin)
hSetupVsRunSourceStatusWeight=ROOT.TH2F("hSetupVsRunSourceStatusWeight","Config vs run number",int(maxrun-minrun+1),minrun,maxrun+1,2,0,2)
hSetupVsRunSourceStatusWeight.SetBit(ROOT.TH1.kCanRebin)
hBeamStatus=ROOT.TH1F("hBeamStatus","Beam Status",2,0,2)
hBeamStatus.SetBit(ROOT.TH1.kCanRebin)
hSetupVsRunBeamStatusWeight=ROOT.TH2F("hSetupVsRunBeamStatusWeight","Config vs run number",int(maxrun-minrun+1),minrun,maxrun+1,2,0,2)
hSetupVsRunBeamStatusWeight.SetBit(ROOT.TH1.kCanRebin)

hStatusBeamVsSource=ROOT.TH2F("hStatusBeamVsSource","GIF Beam status Vs Source",2,0,2,2,0,2)
hStatusBeamVsSource.SetBit(ROOT.TH1.kCanRebin)

hGIFSourceAttenuator=ROOT.TH1F("hGIFSourceAttenuator","GIF++ source attenuation",2,0,2)
hGIFSourceAttenuator.SetBit(ROOT.TH1.kCanRebin)


statusLists = ['ON', 'OFF', 'Unknown from GIF elog', 'Unknown from M3 elog']


for ientry in xrange( entries ):
    nb=tree.GetEntry( ientry )
    if nb <= 0:
        continue
    runNumber=tree.runNumber
    setup=all.getConfigName(runNumber)
    hSetupName.Fill(setup,1)
    hSetupVsRun.Fill(runNumber,setup,1)
    if (setup != ROOT.all_ConfigInfo.UnknownConfig):
        cond=ROOT.GIF_Conditions()
        if (cond.setToRun(runNumber)==True):
            if (cond.getSourceStatus()==ROOT.GIF_Conditions.ON):
                sourceindex=1
                hGIFSourceAttenuator.Fill(str(cond.getUpAttValueApprox()),1)
                print runNumber," ",cond.getUpAttValueApprox()," ",cond.getUpAtt()
            if (cond.getSourceStatus()==ROOT.GIF_Conditions.OFF):
                sourceindex=2
                hGIFSourceAttenuator.Fill('infinite',1)
            if (cond.getSourceStatus()==ROOT.GIF_Conditions.UNKNOWN):
                sourceindex=3
            if (cond.getBeamStatus()==ROOT.GIF_Conditions.ON):
                beamindex=1
            if (cond.getBeamStatus()==ROOT.GIF_Conditions.OFF):
                beamindex=2
            if (cond.getBeamStatus()==ROOT.GIF_Conditions.UNKNOWN):
                beamindex=3
        else:
            sourceindex=4
            beamindex=4
        hSourceStatus.Fill(statusLists[sourceindex-1],1)
        hSetupVsRunSourceStatusWeight.Fill(runNumber,setup,sourceindex)
        hBeamStatus.Fill(statusLists[beamindex-1],1)
        hSetupVsRunBeamStatusWeight.Fill(runNumber,setup,beamindex)
        hStatusBeamVsSource.Fill(statusLists[sourceindex-1],statusLists[beamindex-1],1)

outputDirectory='october2016_GIF_plots'
if not os.path.isdir(outputDirectory):
   os.mkdir(outputDirectory)
outputDirectory=outputDirectory+'/'

cSetupName1=ROOT.TCanvas()
ROOT.gStyle.SetOptStat("e")
hSetupName.LabelsDeflate()
hSetupName.SetFillColor(38);
hSetupName.GetXaxis().LabelsOption("a")
hSetupName.GetYaxis().SetTitle("Number of Run using config setup")
hSetupName.Draw()
cSetupName1.SaveAs(outputDirectory+"hSetupName_alpha.png")
hSetupName.GetXaxis().LabelsOption(">")
hSetupName.Draw("TEXT SAME")
cSetupName1.SaveAs(outputDirectory+"hSetupName_order.png")

cSetupName2=ROOT.TCanvas("cSetupName2","",10,10,6*int(maxrun-minrun+1),500)
cSetupName2.SetGrid()
hSetupVsRun.LabelsDeflate("Y")
hSetupVsRunSourceStatusWeight.LabelsDeflate("Y")
hSetupVsRunBeamStatusWeight.LabelsDeflate("Y")
hSetupVsRun.LabelsOption("a","Y")
hSetupVsRunSourceStatusWeight.LabelsOption("a","Y")
hSetupVsRunBeamStatusWeight.LabelsOption("a","Y")
hSetupVsRun.GetXaxis().SetNoExponent()
hSetupVsRunSourceStatusWeight.GetXaxis().SetNoExponent()
hSetupVsRunBeamStatusWeight.GetXaxis().SetNoExponent()
hSetupVsRun.GetXaxis().SetTitle("RunNumber")
hSetupVsRunSourceStatusWeight.GetXaxis().SetTitle("RunNumber")
hSetupVsRunBeamStatusWeight.GetXaxis().SetTitle("RunNumber")
hSetupVsRun.SetMarkerStyle(23)
hSetupVsRunSourceStatusWeight.SetContour(4)
hSetupVsRunSourceStatusWeight.SetContourLevel(1,1.2)
hSetupVsRunSourceStatusWeight.SetContourLevel(2,2.2)
hSetupVsRunSourceStatusWeight.SetContourLevel(3,3.2)
hSetupVsRunBeamStatusWeight.SetContour(4)
hSetupVsRunBeamStatusWeight.SetContourLevel(1,1.2)
hSetupVsRunBeamStatusWeight.SetContourLevel(2,2.2)
hSetupVsRunBeamStatusWeight.SetContourLevel(3,3.2)

#cSetupName2.SetLeftMargin(0.15)
hSetupVsRun.Draw()
print hSetupVsRun.GetXaxis().GetNdivisions()
hSetupVsRun.GetXaxis().SetNdivisions(int((maxrun-minrun+1)/15),4,5)
cSetupName2.SaveAs(outputDirectory+"hSetupVsRun.png")
hSetupVsRun.Draw("col")
hSetupVsRun.Draw("SAME")
cSetupName2.SaveAs(outputDirectory+"hSetupVsRun_col.png")
hSetupVsRunSourceStatusWeight.Draw("colz")
cSetupName2.Update()
colzPaletteAxis=hSetupVsRunSourceStatusWeight.GetListOfFunctions().FindObject("palette")
print colzPaletteAxis
#colzGAxis=colzPaletteAxis.GetAxis()
#ROOT 6 feature
#colzGAxis.ChangeLabel(3,-1,-1,-1,-1,-1,"TUT")
#ROOT 5 compatible
hSetupVsRunSourceStatusWeight.Draw("col") #suppres the Z axis
hcol1=ROOT.TH1F()
hcol1.SetMarkerStyle(23)
hcol1.SetFillColor(colzPaletteAxis.GetValueColor(0.8))
hcol2=ROOT.TH1F()
hcol2.SetMarkerStyle(23)
hcol2.SetFillColor(colzPaletteAxis.GetValueColor(1.8))
hcol3=ROOT.TH1F()
hcol3.SetMarkerStyle(23)
hcol3.SetFillColor(colzPaletteAxis.GetValueColor(2.8))
hcol4=ROOT.TH1F()
hcol4.SetMarkerStyle(23)
hcol4.SetFillColor(colzPaletteAxis.GetValueColor(3.5))
legendSource=ROOT.TLegend(0.15,0.4,0.4,0.7)
legendSource.AddEntry(hcol1,"SOURCE ON","pf")
legendSource.AddEntry(hcol2,"SOURCE OFF","pf")
legendSource.AddEntry(hcol3,"SOURCE status unknown from GIF elog","pf")
legendSource.AddEntry(hcol4,"SOURCE status unknown from M3 elog","pf")
legendSource.Draw()
hSetupVsRun.Draw("SAME")
cSetupName2.SaveAs(outputDirectory+"hSetupVsRun_colsourceweight.png")
hSetupVsRunBeamStatusWeight.Draw("col")
legendBeam=ROOT.TLegend(0.15,0.4,0.4,0.7)
legendBeam.AddEntry(hcol1,"BEAM ON","pf")
legendBeam.AddEntry(hcol2,"BEAM OFF","pf")
legendBeam.AddEntry(hcol3,"BEAM status unknown from GIF elog","pf")
legendBeam.AddEntry(hcol4,"BEAM status unknown from M3 elog","pf")
legendBeam.Draw()
hSetupVsRun.Draw("SAME")
cSetupName2.SaveAs(outputDirectory+"hSetupVsRun_colbeamweight.png")

cSource=ROOT.TCanvas()
hSourceStatus.LabelsDeflate()
hSourceStatus.GetXaxis().LabelsOption("a")
hSourceStatus.GetXaxis().SetTitle("GIF source status")
hSourceStatus.GetYaxis().SetTitle("Number of Run")
hSourceStatus.Draw()
hSourceStatus.Draw("TEXT SAME")
cSource.SaveAs(outputDirectory+"hSourceStatus.png")

cBeam=ROOT.TCanvas()
hBeamStatus.LabelsDeflate()
hBeamStatus.GetXaxis().LabelsOption("a")
hBeamStatus.GetXaxis().SetTitle("GIF beam status")
hBeamStatus.GetYaxis().SetTitle("Number of Run")
hBeamStatus.Draw()
hBeamStatus.Draw("TEXT SAME")
cBeam.SaveAs(outputDirectory+"hBeamStatus.png")

cBeamSource=ROOT.TCanvas("cBeamSource","",10,10,800,800)
cBeamSource.SetGrid()
cBeamSource.SetLeftMargin(0.3)
cBeamSource.SetRightMargin(0.15)
hStatusBeamVsSource.LabelsDeflate("X")
hStatusBeamVsSource.LabelsDeflate("Y")
hStatusBeamVsSource.GetXaxis().LabelsOption("a")
hStatusBeamVsSource.GetYaxis().LabelsOption("a")
hStatusBeamVsSource.GetXaxis().SetTitle("Source status")
hStatusBeamVsSource.GetXaxis().SetTitleOffset(-0.3)
hStatusBeamVsSource.GetYaxis().SetTitleOffset(-0.3)
hStatusBeamVsSource.GetYaxis().SetTitle("Beam  status")
hStatusBeamVsSource.Draw("TEXT")
cBeamSource.SaveAs(outputDirectory+"hStatusBeamVsSource.png")

cGIFatt=ROOT.TCanvas()
hGIFSourceAttenuator.LabelsDeflate()
hGIFSourceAttenuator.GetXaxis().LabelsOption(">")
hGIFSourceAttenuator.GetXaxis().SetTitle("GIF source attenuation")
hGIFSourceAttenuator.GetYaxis().SetTitle("Number of Run")
hGIFSourceAttenuator.Draw()
hGIFSourceAttenuator.Draw("TEXT SAME")
cGIFatt.SaveAs(outputDirectory+"hGIFSourceAttenuator.png")


def writeframe(file,title,plotfile,scale,text):
    file.write("\\begin{frame}{"+title+"}\n")
    file.write('\includegraphics[scale={0}]{{{1}}}\n'.format(scale,plotfile))
    file.write('\n'+text+'\n')
    file.write('\end{frame}\n')

mylatexfilename="report.tex"
mylatex = open(outputDirectory+mylatexfilename,'w')
mylatexheader="""\documentclass[10pt,a4paper,english]{beamer}
\usepackage{xspace}
\usepackage{amsfonts}
\usepackage{amsmath}
\usepackage{amssymb}
\usepackage{graphicx}

%\usetheme{Montpellier}
"""
mylatex.write(mylatexheader)
mylatex.write('\\begin{document}\n')

writeframe(mylatex,'Used configurations',"hSetupName_order.png",0.4,
"{0} runs analysed. {1} for which configuration setup used has not been recorded (no entry in elog).".format(int(hSetupName.GetEntries()),int(hSetupName.GetBinContent(1))))

writeframe(mylatex,'Used configurations versus run number',"hSetupVsRun.png",0.23,
"Distribution of the configuration used as a function of the run number")

writeframe(mylatex,'Source status',"hSourceStatus.png",0.4,
"On the {total} runs with elog info, {mcube} have no info on source since they were written in the M$^3$ elog, {gif} have no source info but are written in the GIF elog. We are left with {ok} runs for which source state is known.".format(total=int(hSourceStatus.GetEntries()),mcube=int(hSourceStatus.GetBinContent(4)),gif=int(hSourceStatus.GetBinContent(3)),ok=int(hSourceStatus.GetBinContent(1)+hSourceStatus.GetBinContent(2))))

writeframe(mylatex,'Beam status',"hBeamStatus.png",0.4,
"On the {total} runs with elog info, {mcube} have no info on beam since they were written in the M$^3$ elog, {gif} have no beam info but are written in the GIF elog.".format(total=int(hBeamStatus.GetEntries()),mcube=int(hBeamStatus.GetBinContent(4)),gif=int(hBeamStatus.GetBinContent(3))))

writeframe(mylatex,'Beam status versus Source status',"hStatusBeamVsSource.png",0.25,
"Correlation on the beam status and the source status in elogs for the {total} runs with elog info. The {mcube} runs recorded in the M$^3$ elog are test runs recorded during the debug phase of the DAQ.".format(total=int(hStatusBeamVsSource.GetEntries()),mcube=int(hStatusBeamVsSource.GetBinContent(4,4))))

writeframe(mylatex,'Used configurations versus run number',"hSetupVsRun_colsourceweight.png",0.23,
"Distribution of the configuration used as a function of the run number. The color corresponds to the GIF source status. Kind of linear variation of setup as a function of run numbers signs threshold scan.")

writeframe(mylatex,'Used configurations versus run number',"hSetupVsRun_colbeamweight.png",0.23,
"Distribution of the configuration used as a function of the run number. The color corresponds to the GIF beam status. Kind of linear variation of setup as a function of run numbers signs threshold scan.")


writeframe(mylatex,'GIF++ attenuation sources',"hGIFSourceAttenuator.png",0.4,
"Number of taken runs as a function of the GIF++ source attenuation. The {total} runs here may have very different time length and no data quality check have been done yet.".format(total=int(hGIFSourceAttenuator.GetEntries())))


mylatex.write('\end{document}')

mylatex.close()

os.system('cd {}; pdflatex {}'.format(outputDirectory,mylatexfilename))
#raw_input("\n\nPress the enter key to exit.")
