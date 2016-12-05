#!/usr/bin/python

#use the tree produced by produce_run_tree_GIF.py
       

import os
from array import array
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
hSetupVsRunBeamBIFWeight=ROOT.TH2F("hSetupVsRunBeamBIFWeight","Config vs run number",int(maxrun-minrun+1),minrun,maxrun+1,2,0,2)
hSetupVsRunBeamBIFWeight.SetBit(ROOT.TH1.kCanRebin)

hStatusBeamVsSource=ROOT.TH2F("hStatusBeamVsSource","GIF Beam status Vs Source",2,0,2,2,0,2)
hStatusBeamVsSource.SetBit(ROOT.TH1.kCanRebin)

hGIFSourceAttenuator=ROOT.TH1F("hGIFSourceAttenuator","GIF++ source attenuation",2,0,2)
hGIFSourceAttenuator.SetBit(ROOT.TH1.kCanRebin)

hNBIFeventsBeamON=ROOT.TH1F("hNBIFeventsBeamON","Number of Yuri's trigger event in BIF when GIF beam status is ON", 100, 0,100)
hNBIFeventsBeamON.SetBit(ROOT.TH1.kCanRebin)
hNBIFeventsBeamOFF=ROOT.TH1F("hNBIFeventsBeamOFF","Number of Yuri's trigger event in BIF when GIF beam status is OFF", 100, 0,100)
hNBIFeventsBeamOFF.SetBit(ROOT.TH1.kCanRebin)
hNBIFeventsBeamUNKNOWN=ROOT.TH1F("hNBIFeventsBeamUNKNOWN","Number of Yuri's trigger event in BIF when GIF beam status is Unknown", 100, 0,100)
hNBIFeventsBeamUNKNOWN.SetBit(ROOT.TH1.kCanRebin)
hNBIF=[hNBIFeventsBeamON,hNBIFeventsBeamOFF,hNBIFeventsBeamUNKNOWN]
hBeamStatusNoBIFYuri=ROOT.TH1F("hBeamStatusNoBIFYuri","Beam Status with Yuri's trigger in BIF and without recorded trigger",2,0,2)
hBeamStatusNoBIFYuri.SetBit(ROOT.TH1.kCanRebin)
hBeamStatusBIFYuri=ROOT.TH1F("hBeamStatusBIFYuri","Beam Status with Yuri's trigger in BIF and with recorded trigger",2,0,2)
hBeamStatusBIFYuri.SetBit(ROOT.TH1.kCanRebin)
hBeamStatusBIF=[hBeamStatusNoBIFYuri,hBeamStatusBIFYuri]

statusLists = ['ON', 'OFF', 'Unknown from GIF elog', 'Unknown from M3 elog']
runLists=[dict(),dict()]
for i in range(3):
    runLists[0][statusLists[i]]=[]
    runLists[1][statusLists[i]]=[]

runBIFeffLnAttVsLogThresh=[]
runBIFeffLnAttVsLogThresh.append(ROOT.TGraph(2)) # to set axis range
runBIFeffLnAttVsLogThresh[0].SetPoint(0,-0.1,0.5)
runBIFeffLnAttVsLogThresh[0].SetPoint(1,12,4.5)
runBIFeffLnAttVsLogThresh[0].SetTitle("log10(threshold (fC)) vs ln(source attenuation)")
runBIFeffLnAttVsLogThresh[0].GetXaxis().SetTitle("ln(source attenuation), 11.5=source OFF")
runBIFeffLnAttVsLogThresh[0].GetYaxis().SetTitle("Bakelite strip log10(threshold (fC))")
runBIFeffLnAttVsLogThresh[0].SetMarkerColor(0)
runBIFeffLnAttVsLogThreshXvalue=array('f')
runBIFeffLnAttVsLogThreshYvalue=[array('f'),array('f'),array('f')]


BIFefficiencyData=dict()


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
        BIFstatus=0
        if (cond.setToRun(runNumber)==True):
            if (cond.getSourceStatus()==ROOT.GIF_Conditions.ON):
                sourceindex=1
                sourceOK=True
                hGIFSourceAttenuator.Fill(str(cond.getUpAttValueApprox()),1)
                print runNumber," ",cond.getUpAttValueApprox()," ",cond.getUpAtt()
            if (cond.getSourceStatus()==ROOT.GIF_Conditions.OFF):
                sourceindex=2
                sourceOK=True
                hGIFSourceAttenuator.Fill('infinite',1)
            if (cond.getSourceStatus()==ROOT.GIF_Conditions.UNKNOWN):
                sourceindex=3
            if (cond.getBeamStatus()==ROOT.GIF_Conditions.ON):
                beamindex=1
            if (cond.getBeamStatus()==ROOT.GIF_Conditions.OFF):
                beamindex=2
            if (cond.getBeamStatus()==ROOT.GIF_Conditions.UNKNOWN):
                beamindex=3
            if (sourceOK and cond.getYuris_small_scintillator_in_BIF()==True):
                BIFstatus=1
                if (tree.Efficiency.n_event > 0):
                    BIFstatus=2+int(tree.Efficiency.n_event > 10)+int(tree.Efficiency.n_event > 100)
                hNBIF[beamindex-1].Fill(tree.Efficiency.n_event)
        else:
            sourceindex=4
            beamindex=4
        hSourceStatus.Fill(statusLists[sourceindex-1],1)
        hSetupVsRunSourceStatusWeight.Fill(runNumber,setup,sourceindex)
        hBeamStatus.Fill(statusLists[beamindex-1],1)
        hSetupVsRunBeamStatusWeight.Fill(runNumber,setup,beamindex)
        hStatusBeamVsSource.Fill(statusLists[sourceindex-1],statusLists[beamindex-1],1)
        hSetupVsRunBeamBIFWeight.Fill(runNumber,setup,BIFstatus)
        if (BIFstatus==1):
            hBeamStatusNoBIFYuri.Fill(statusLists[beamindex-1],1)
            runLists[0][statusLists[beamindex-1]].append(runNumber)
        if (BIFstatus>1):
            hBeamStatusBIFYuri.Fill(statusLists[beamindex-1],1)
            runLists[1][statusLists[beamindex-1]].append(runNumber)
            config=all.getConfigInfo(runNumber)
            thresh=[config.get_first_threshold_charge(2,1),config.get_second_threshold_charge(2,1),config.get_third_threshold_charge(2,1)]
            if (sourceindex==1 or sourceindex==2):
                attlog=11.5
                attloginv=0
                if (sourceindex==1):
                    attlog=ROOT.Math.log(cond.getUpAttValueApprox())
                    attloginv=1/cond.getUpAttValue()
                print attlog
                runBIFeffLnAttVsLogThreshXvalue.append(attlog)
                for x in range(3):
                    runBIFeffLnAttVsLogThreshYvalue[x].append(ROOT.Math.log10(thresh[x]))
                for thresh in [1,2,3]:
                    BIFefficiencyData[('PAD1',runNumber,attloginv,thresh,config.get_threshold_charge(thresh,125,1))]=(tree.Efficiency.n_event,tree.Efficiency.flagcount(thresh,0),tree.FakeEfficiency.flagcount(thresh,0))
                    BIFefficiencyData[('PAD2',runNumber,attloginv,thresh,config.get_threshold_charge(thresh,32,1))]=(tree.Efficiency.n_event,tree.Efficiency.flagcount(thresh,1),tree.FakeEfficiency.flagcount(thresh,1))
                    BIFefficiencyData[('PAD3',runNumber,attloginv,thresh,config.get_threshold_charge(thresh,28,1))]=(tree.Efficiency.n_event,tree.Efficiency.flagcount(thresh,2),tree.FakeEfficiency.flagcount(thresh,2))
                    BIFefficiencyData[('PAD4',runNumber,attloginv,thresh,config.get_threshold_charge(thresh,6,1))]=(tree.Efficiency.n_event,tree.Efficiency.flagcount(thresh,3),tree.FakeEfficiency.flagcount(thresh,3))
                    BIFefficiencyData[('STRIP_GLASS',runNumber,attloginv,thresh,config.get_threshold_charge(thresh,26,1))]=(tree.Efficiency.n_event,tree.Efficiency.flagcount(thresh,4),tree.FakeEfficiency.flagcount(thresh,4),tree.Efficiency.flagcount(thresh,4,1),tree.FakeEfficiency.flagcount(thresh,4,1),tree.Efficiency.flagcount(thresh,4,2),tree.FakeEfficiency.flagcount(thresh,4,2))
                    BIFefficiencyData[('STRIP_BAKELITE',runNumber,attloginv,thresh,config.get_threshold_charge(thresh,2,1))]=(tree.Efficiency.n_event,tree.Efficiency.flagcount(thresh,5),tree.FakeEfficiency.flagcount(thresh,5),tree.Efficiency.flagcount(thresh,5,1),tree.FakeEfficiency.flagcount(thresh,5,1),tree.Efficiency.flagcount(thresh,5,2),tree.FakeEfficiency.flagcount(thresh,5,2))

print "OOOOOPPPPPSSSS ",len(BIFefficiencyData)


for i in range(3):
    runBIFeffLnAttVsLogThresh.append(ROOT.TGraph(len(runBIFeffLnAttVsLogThreshXvalue),runBIFeffLnAttVsLogThreshXvalue,runBIFeffLnAttVsLogThreshYvalue[i]))
grcolor=[4,1,2]
for thresh in [1,2,3]:
    runBIFeffLnAttVsLogThresh[thresh].SetMarkerStyle(23)
    runBIFeffLnAttVsLogThresh[thresh].SetMarkerColor(grcolor[thresh-1])

for n in runLists:
    for i in range(3):
        n[statusLists[i]].sort()
        n[statusLists[i]].sort()
print runLists

usedRunLists=[]
for i in range(3):
    usedRunLists.extend(runLists[1][statusLists[i]])
usedRunLists.sort()
print "YEEEEEEEEEEEEEEEEE",len(usedRunLists),usedRunLists




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
hSetupVsRunBeamBIFWeight.LabelsDeflate("Y")
hSetupVsRun.LabelsOption("a","Y")
hSetupVsRunSourceStatusWeight.LabelsOption("a","Y")
hSetupVsRunBeamStatusWeight.LabelsOption("a","Y")
hSetupVsRunBeamBIFWeight.LabelsOption("a","Y")
hSetupVsRun.GetXaxis().SetNoExponent()
hSetupVsRunSourceStatusWeight.GetXaxis().SetNoExponent()
hSetupVsRunBeamStatusWeight.GetXaxis().SetNoExponent()
hSetupVsRunBeamBIFWeight.GetXaxis().SetNoExponent()
hSetupVsRun.GetXaxis().SetTitle("RunNumber")
hSetupVsRunSourceStatusWeight.GetXaxis().SetTitle("RunNumber")
hSetupVsRunBeamStatusWeight.GetXaxis().SetTitle("RunNumber")
hSetupVsRunBeamBIFWeight.GetXaxis().SetTitle("RunNumber")
hSetupVsRun.SetMarkerStyle(23)
hSetupVsRunSourceStatusWeight.SetContour(4)
hSetupVsRunSourceStatusWeight.SetContourLevel(1,1.2)
hSetupVsRunSourceStatusWeight.SetContourLevel(2,2.2)
hSetupVsRunSourceStatusWeight.SetContourLevel(3,3.2)
hSetupVsRunBeamStatusWeight.SetContour(4)
hSetupVsRunBeamStatusWeight.SetContourLevel(1,1.2)
hSetupVsRunBeamStatusWeight.SetContourLevel(2,2.2)
hSetupVsRunBeamStatusWeight.SetContourLevel(3,3.2)
hSetupVsRunBeamBIFWeight.SetContour(4)
hSetupVsRunBeamBIFWeight.SetContourLevel(1,1.2)
hSetupVsRunBeamBIFWeight.SetContourLevel(2,2.2)
hSetupVsRunBeamBIFWeight.SetContourLevel(3,3.2)

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
hSetupVsRunBeamBIFWeight.Draw("col")
legendBIF=ROOT.TLegend(0.15,0.4,0.4,0.7)
legendBIF.AddEntry(hcol1,"Yuri's trigger in BIF","pf")
legendBIF.AddEntry(hcol2,"Yuri's trigger in BIF+BIF events found","pf")
legendBIF.AddEntry(hcol3,"Yuri's trigger in BIF+at least 10 BIF events found","pf")
legendBIF.AddEntry(hcol4,"Yuri's trigger in BIF+at least 100 BIF events found","pf")
legendBIF.Draw()
hSetupVsRun.Draw("SAME")
cSetupName2.SaveAs(outputDirectory+"hSetupVsRun_colBIFweight.png")

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

cNBIF=ROOT.TCanvas()
cNBIF.Divide(1,3)
for i in range(3):
    cNBIF.cd(i+1)
    hNBIF[i].GetXaxis().SetTitle("Number of recorded Yuri's trigger in BIF")
    hNBIF[i].GetYaxis().SetTitle("Number of runs")
    hNBIF[i].Draw()
cNBIF.SaveAs(outputDirectory+"hNBIFeventsBeamStatus.png")
cNBIF.Clear()
cNBIF.Divide(2,1)
for i in range(2):
    cNBIF.cd(i+1)
    hBeamStatusBIF[i].LabelsDeflate()
    hBeamStatusBIF[i].GetXaxis().LabelsOption("a")
    hBeamStatusBIF[i].GetXaxis().SetTitle("GIF beam status")
    hBeamStatusBIF[i].GetYaxis().SetTitle("Number of Run")
    hBeamStatusBIF[i].Draw()
    hBeamStatusBIF[i].Draw("TEXT SAME")
cNBIF.SaveAs(outputDirectory+"hBeamStatusBIF.png")

cWhat=ROOT.TCanvas()
runBIFeffLnAttVsLogThresh[0].Draw("AP")
legendWhat=ROOT.TLegend(0.4,0.7,0.55,0.85)
for thresh in [1,2,3]:
    runBIFeffLnAttVsLogThresh[thresh].Draw("P")
    legendWhat.AddEntry(runBIFeffLnAttVsLogThresh[thresh],"threshold "+str(thresh),"p")
legendWhat.Draw()
cWhat.SaveAs(outputDirectory+"runBIFeffLnAttVsLogThresh.png")



def graphBIFData(BIFdata,chambre,invatt,MarkerStyle):
    subBIFdata=dict()
    for k in BIFdata.keys():
        if (k[0]==chambre and k[2]==invatt):
            subBIFdata[k]=BIFdata[k]
    #print len(subBIFdata.itervalues().next())
    thresholds=[array('f'),array('f'),array('f')]
    Yvalues=[]
    for ngr in range(len(subBIFdata.itervalues().next())):
        Yvalues.append([array('f'),array('f'),array('f')])
    for k,v in subBIFdata.iteritems():
        thresholds[k[3]-1].append(k[4])
        Yvalues[0][k[3]-1].append(v[0])
        for iy in xrange(1, len(subBIFdata.itervalues().next())):
            Yvalues[iy][k[3]-1].append(v[iy]*1.0/v[0])
        #print k,"########",v
    outputGraphs=[]
    for ngr in range(len(subBIFdata.itervalues().next())):
        outputGraphs.append([ROOT.TGraph(len(thresholds[ithresh]),thresholds[ithresh],Yvalues[ngr][ithresh]) for ithresh in range(3)])
        outputGraphs[ngr][0].SetMarkerColor(4)
        outputGraphs[ngr][1].SetMarkerColor(1)
        outputGraphs[ngr][2].SetMarkerColor(2)
        for gr in outputGraphs[ngr]:
            gr.SetMarkerStyle(MarkerStyle)
        for thresh in range(3):
            outputGraphs[ngr][thresh].SetMaximum(max(Yvalues[ngr][thresh]))
            outputGraphs[ngr][thresh].SetMinimum(min(Yvalues[ngr][thresh]))
    print [(min(i),max(i)) for i in thresholds]
    XaxisRange=(min([min(i) for i in thresholds]),max([max(i) for i in thresholds]))
    print XaxisRange
    return (XaxisRange,outputGraphs)
    

def graphBIFDataAtt(BIFdata,chambre):
    invattvalues=set()
    for k in BIFdata.keys():
        if (k[0]==chambre):
            invattvalues.add(k[2])
    print invattvalues
    nattvalue=len(invattvalues)
    retgr=dict()
    for i in range(nattvalue):
        a=invattvalues.pop()
        retgr[a]=graphBIFData(BIFdata,chambre,a,20+i)
    return retgr

def drawBIFDataAtt(BIFdata,chambre):
    graphs=graphBIFDataAtt(BIFdata,chambre)
    namePlots=['Nevents','efficicieny_plan','fake_efficiency_plan','efficicieny_plan_gap1','fake_efficiency_plan_gap1','efficicieny_plan_gap2','fake_efficiency_plan_gap2']
    canvasName='c'+chambre+'att'
    nplot=len(graphs.values()[0][1])
    xmin=min([g[0][0] for g in graphs.values()])
    xmax=max([g[0][1] for g in graphs.values()])
    print [g[0] for g in graphs.values()], xmin, xmax 
    for plot in range(nplot):
        extrema=[]
        for g in graphs.values():
            for thresh in range(3):
                extrema.append((g[1][plot][thresh].GetMinimum(),g[1][plot][thresh].GetMaximum()))
        ymin=min([v[0] for v in extrema])
        ymax=max([v[1] for v in extrema]) 
        print ymin,ymax
        if (ymax>0.10 and ymax <1): ymax=1 #efficiency plot max=1
        if (ymax<2*ymin): ymax=2*ymin # make sure we have a non zero length yaxis
        fakeGraph=ROOT.TGraph(2)
        fakeGraph.SetPoint(0,xmin,ymin)
        fakeGraph.SetPoint(1,xmax,ymax)
        fakeGraph.SetMarkerColor(0)
        fakeGraph.GetXaxis().SetTitle("threshold (fc)")
        fakeGraph.GetYaxis().SetTitle(namePlots[plot])
        for k,g in graphs.items():
            mycanvas=ROOT.TCanvas()
            if (xmax>100*xmin): mycanvas.SetLogx()
            att='sourceOFF'
            if (k!=0): att=1/k
            fakeGraph.SetTitle(chambre+' attenuation='+str(att))
            fakeGraph.Draw("AP")
            for thresh in range(3):
                g[1][plot][thresh].Draw("P")
            mycanvas.SaveAs(outputDirectory+canvasName+'_'+chambre+'_att_'+str(att)+'_'+namePlots[plot]+'.png')


drawBIFDataAtt(BIFefficiencyData,'PAD1')
drawBIFDataAtt(BIFefficiencyData,'PAD2')
drawBIFDataAtt(BIFefficiencyData,'PAD3')
drawBIFDataAtt(BIFefficiencyData,'PAD4')
drawBIFDataAtt(BIFefficiencyData,'STRIP_GLASS')
drawBIFDataAtt(BIFefficiencyData,'STRIP_BAKELITE')


def graphBIFDataThreshAddPoint(index,xvalue,yvalue,k,v):
    xvalue[index].append(k[2])
    if len(yvalue[index])==0:
        yvalue[index]=[array('f') for i in v]
    yvalue[index][0].append(v[0])
    for i in xrange(1,len(v)):
        yvalue[index][i].append(v[i]*1.0/v[0])
    

def graphBIFDataThresh(BIFdata,chambre,thresholds):
    #refsetup=all.getConfigInfo(AsSetup)
    xvalue=[array('f') for i in thresholds] #1/att
    yvalue=[ [] for i in thresholds]
    for k,v in BIFdata.items():
        if k[0]!=chambre: continue
        if k[3]==1 and k[4]==thresholds[0]:
            graphBIFDataThreshAddPoint(0,xvalue,yvalue,k,v)
        if k[3]==2 and k[4]==thresholds[1]:
            graphBIFDataThreshAddPoint(1,xvalue,yvalue,k,v)
    return (xvalue,yvalue)


def drawgraphBIFDataThresh(BIFdata,chambre,thresholds):
    namePlots=['Nevents','efficicieny_plan','fake_efficiency_plan','efficicieny_plan_gap1','fake_efficiency_plan_gap1','efficicieny_plan_gap2','fake_efficiency_plan_gap2']
    graphArray=graphBIFDataThresh(BIFefficiencyData,chambre,thresholds)
    xvalue=graphArray[0]
    yvalue=graphArray[1]
    cScanAtt=ROOT.TCanvas()
    for ithresh in range(len(thresholds)):
        grxvalue=xvalue[ithresh]
        gryvalues=yvalue[ithresh]
        for iplot in range(len(gryvalues)):
            myGraph=ROOT.TGraph(len(grxvalue),grxvalue,gryvalues[iplot])
            myGraph.GetXaxis().SetTitle("1/att")
            myGraph.GetYaxis().SetTitle(namePlots[iplot])
            myGraph.SetMarkerStyle(23)
            canName=chambre+'_thresh_is_'+str(thresholds[ithresh])+'_fC'
            if iplot>0:
                fakeGraph=ROOT.TGraph(2)
                fakeGraph.SetPoint(0,min(grxvalue),0)
                fakeGraph.SetPoint(1,max(grxvalue),1)
                fakeGraph.SetMarkerColor(0)
                fakeGraph.SetTitle(canName)
                fakeGraph.GetXaxis().SetTitle("1/att")
                fakeGraph.GetYaxis().SetTitle(namePlots[iplot])
                fakeGraph.Draw("AP")
                myGraph.Draw("P")
            else:
                myGraph.SetTitle(canName)
                myGraph.Draw("AP")
            cScanAtt.SaveAs(outputDirectory+'attScan_'+canName+'_'+namePlots[iplot]+'.png')

refsetup=all.getConfigInfo('GIFPP_STRIP_22')
drawgraphBIFDataThresh(BIFefficiencyData,'PAD1',[refsetup.get_first_threshold_charge(125,1),refsetup.get_second_threshold_charge(125,1)])
drawgraphBIFDataThresh(BIFefficiencyData,'PAD2',[refsetup.get_first_threshold_charge(32,1),refsetup.get_second_threshold_charge(32,1)])
drawgraphBIFDataThresh(BIFefficiencyData,'PAD3',[refsetup.get_first_threshold_charge(28,1),refsetup.get_second_threshold_charge(28,1)])
drawgraphBIFDataThresh(BIFefficiencyData,'PAD4',[refsetup.get_first_threshold_charge(6,1),refsetup.get_second_threshold_charge(6,1)])
drawgraphBIFDataThresh(BIFefficiencyData,'STRIP_GLASS',[refsetup.get_first_threshold_charge(26,1),refsetup.get_second_threshold_charge(26,1)])
drawgraphBIFDataThresh(BIFefficiencyData,'STRIP_BAKELITE',[refsetup.get_first_threshold_charge(2,1),refsetup.get_second_threshold_charge(2,1)])


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

writeframe(mylatex,'Used configurations versus run number',"hSetupVsRun_colBIFweight.png",0.23,
"Distribution of the configuration used as a function of the run number. The color corresponds to the BIF trigger status. Kind of linear variation of setup as a function of run numbers signs threshold scan.")


writeframe(mylatex,'GIF++ attenuation sources',"hGIFSourceAttenuator.png",0.4,
"Number of taken runs as a function of the GIF++ source attenuation. The {total} runs here may have very different time length and no data quality check have been done yet.".format(total=int(hGIFSourceAttenuator.GetEntries())))

writeframe(mylatex,"Beam status for runs with Yuri's trigger","hBeamStatusBIF.png",0.3,
"{total} runs have source info and were recorded once Yuri's small scintillator trigger was in place. Histograms display the beam status with (right)/without(left) triggers found in the BIF. Most of the runs without GIF beam info have no trigger event and likely correspond to beam OFF runs. However, {unknownisON} run with unknown status has events. With these remarks, the majority of runs without Yuri's beam trigger events are beam OFF runs and the majority with are beam ON. There are few exceptions, though.".format(total=int(hBeamStatusBIF[0].GetEntries()+hBeamStatusBIF[1].GetEntries()),unknownisON=int(hBeamStatusBIF[1].GetBinContent(3))))

writeframe(mylatex,"number of triggered event","hNBIFeventsBeamStatus.png",0.35,
"Number of recorded Yuri's trigger event when GIF beam status is ON(top), OFF(middle) and Unknown(bottom). The unique run with unknown beam status has a relatively high number of events and may be a beam ON run. Same remark for at least one of the runs with beam OFF status and trigger events in.")

mylatex.write("\\begin{frame}{Exceptions in beam status runs}\n")
mylatex.write("\n The list of runs with beam ON and no events is {runs}.\n These runs correspond to 2 failed attempts to run in triggered DAQ\n".format(runs=runLists[0]['ON']))
mylatex.write("\n The list of runs with Unknown beam and no events is {runs}.\n These runs have been taken on the last test beam day when SPS was broken.They are beam OFF runs\n".format(runs=runLists[0][statusLists[2]]))
mylatex.write("\n The list of runs with Unknown beam with BIF events is {runs}.\n The elog says that the runs starts with beam but the beam has been lost after 15 minutes of data taking\n".format(runs=runLists[1][statusLists[2]]))
mylatex.write("\n The list of runs with beam OFF and events is {runs}.\n The elog says the RD51 magnet was on. This means beam was on but believed not reaching the small setup.\n".format(runs=runLists[1]['OFF']))
mylatex.write('\end{frame}\n')


mylatex.write("\\begin{frame}{Run list for BIF-based efficiency study}\n")
mylatex.write("\n There are {num} runs with events to estimate efficiencies from BIF :  {runs}.\n ".format(num=len(usedRunLists),runs=usedRunLists))
mylatex.write('\end{frame}\n')


writeframe(mylatex,"Threshold and attenuator scans","runBIFeffLnAttVsLogThresh.png",0.4,
"The plot shows the thresholds and source attenuation for the runs with Yuri's trigger and recorded corresponding BIF events")


mylatex.write('\end{document}')

mylatex.close()

os.system('cd {}; pdflatex {}'.format(outputDirectory,mylatexfilename))
#raw_input("\n\nPress the enter key to exit.")
