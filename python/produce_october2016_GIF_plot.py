#!/usr/bin/python

#use the tree produced by produce_run_tree_GIF.py

def functest(t):
    t.SetMarkerColor(2)

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
runBIFeffLnAttVsLogThreshXvalue=[]
runBIFeffLnAttVsLogThreshYvalue=[]
for i in range(6):
    runBIFeffLnAttVsLogThreshXvalue.append(array('f'))
    runBIFeffLnAttVsLogThreshYvalue.append([array('f'),array('f'),array('f')])



#for strip chambers plot of scan efficiency versus threshold for source OFF and attenuator=333
class StripGapEfficiencySet:
    def __init__(self):
        self.efficiency=[array('f'),array('f'),array('f')]  # 3 thresholds
        self.fakeEfficiency=[array('f'),array('f'),array('f')]

class StripChamberEfficiencySet:
    def __init__(self):
        self.x=[array('f'),array('f'),array('f')] # 3 thresholds
        self.all=StripGapEfficiencySet()
        self.gap=dict()
        self.gap[1]=StripGapEfficiencySet()
        self.gap[2]=StripGapEfficiencySet()


    def fill(self,config,effData,fakeEffData,DIFnumber,planNumber):
        thresh=[config.get_first_threshold_charge(DIFnumber,1),config.get_second_threshold_charge(DIFnumber,1),config.get_third_threshold_charge(DIFnumber,1)]
        for i in range(3):
            self.x[i].append(thresh[i])
            self.all.efficiency[i].append(effData.flagcount(i+1,planNumber)*1.0/effData.n_event)
            self.all.fakeEfficiency[i].append(fakeEffData.flagcount(i+1,planNumber)*1.0/fakeEffData.n_event)
            for g,v in self.gap.iteritems():
                v.efficiency[i].append(effData.flagcount(i+1,planNumber,g)*1.0/effData.n_event)
                v.fakeEfficiency[i].append(fakeEffData.flagcount(i+1,planNumber,g)*1.0/fakeEffData.n_event)

class thresholdScan:
    def __init__(self):
        self.numberOfBIF=array('I')
        self.runNumbers=array('I')
        self.glass=StripChamberEfficiencySet()
        self.bakelite=StripChamberEfficiencySet()

    def fill(self,runnumber,config,effData,fakeEffData):
        self.numberOfBIF.append(effData.n_event)
        self.runNumbers.append(runnumber)
        self.glass.fill(config,effData,fakeEffData,26,4)
        self.bakelite.fill(config,effData,fakeEffData,2,5)

class effSetOneValue:
    def __init__(self):
        self.eff=array('f')
        self.fakeEff=array('f')

class attenuatorScan:
    def __init__(self,numPlan,planType,runNumbersList,tL,DAC):
        self.runNumbers=runNumbersList
        self.runNumbersFillList=array('I')
        self.numberOfBIF=array('I')
        self.x=array('f')  #1/attenuator value
        self.planeEff=effSetOneValue()
        self.planNumber=numPlan
        self.type=planType
        if self.type=='strip':
            self.gap=dict()
            self.gap[1]=effSetOneValue()
            self.gap[2]=effSetOneValue()
        self.thresholdLevel=tL
        self.thresholdValue=DAC

    def fill(self,runnumber,effData,fakeEffData,cond):
        if not runNumber in self.runNumbers:
            return
        self.runNumbersFillList.append(runnumber)
        self.numberOfBIF.append(effData.n_event)
        if (cond.getSourceStatus()==ROOT.GIF_Conditions.OFF):
            self.x.append(0.0)
        if (cond.getSourceStatus()==ROOT.GIF_Conditions.ON):
            self.x.append(1/cond.getUpAttValue())
        self.planeEff.eff.append(effData.flagcount(self.thresholdLevel,self.planNumber)*1.0/effData.n_event)
        self.planeEff.fakeEff.append(fakeEffData.flagcount(self.thresholdLevel,self.planNumber)*1.0/fakeEffData.n_event)
        if self.type=='strip':
            for g,v in self.gap.iteritems():
                v.eff.append(effData.flagcount(self.thresholdLevel,self.planNumber,g)*1.0/effData.n_event)
                v.fakeEff.append(fakeEffData.flagcount(self.thresholdLevel,self.planNumber,g)*1.0/fakeEffData.n_event)
          
    def tupleIt(self):
        ret=[]
        for i in range(len(self.runNumbersFillList)):
            ret.append((self.runNumbersFillList[i],self.numberOfBIF[i],self.x[i],self.planeEff.eff[i],self.planeEff.fakeEff[i]))
        return ret
    def tupleItsort(self,numcolonne):
        return sorted(self.tupleIt(), key=lambda col: col[numcolonne])
    def printItsort(self,numcolonne):
        for what in self.tupleItsort(numcolonne):
            print "run {:7} : nBIF={:6}, 1/att={:f}, eff={:f}, fakeEff={:f}".format(what[0],what[1],what[2],what[3],what[4])

class ThreshAttSetupList:
    def __init__(self):
        self.attValues=set()
        self.setupNames=set()
        self.runNumbers=set()
        self.attValuesRunList=dict()
    def add(self,attvalue,setup,run):
        self.attValues.add(attvalue)
        self.setupNames.add(setup)
        self.runNumbers.add(run)
        if not self.attValuesRunList.get(attvalue):
            self.attValuesRunList[attvalue]=[]
        self.attValuesRunList[attvalue].append(run)

        
ThreshAttMap=[]
for i in range(6):
    ThreshAttMap.append(dict())
        
thrScansRunList=[]

hAttScanThresholdPossibility=ROOT.TH1F("hAttScanThresholdPossibility","number of attenuator value in runs for (plan, threshold number, threshold value)",2,0,2)
hAttScanThresholdPossibility.SetBit(ROOT.TH1.kCanRebin)
hAttScanThresholdVsSetup=ROOT.TH2F("hAttScanThresholdVsSetup","Setup names vs threshold scans",2,0,2,2,0,2)
hAttScanThresholdVsSetup.SetBit(ROOT.TH1.kCanRebin)
hAttScanThresholdVsAtt=ROOT.TH2F("hAttScanThresholdVsAtt","Attenuator value vs threshold scans",2,0,2,2,0,2)
hAttScanThresholdVsAtt.SetBit(ROOT.TH1.kCanRebin)
hAttScanThresholdVsAttNrunWeight=ROOT.TH2F("hAttScanThresholdVsAttNrunWeight","Attenuator value vs threshold scans",2,0,2,2,0,2)
hAttScanThresholdVsAttNrunWeight.SetBit(ROOT.TH1.kCanRebin)

thrScans=thresholdScan()


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
            sourceOK=False
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
            DIFnumberByPlan=[125,32,28,6,26,2]
            if (sourceindex==1 or sourceindex==2):
                attlog=11.5
                attString='OFF'
                if (sourceindex==1):
                    attlog=ROOT.Math.log(cond.getUpAttValueApprox())
                    attString="{:6.1f}".format(cond.getUpAttValueApprox())
                print attlog
                for numplan in range(6): 
                    DIF=DIFnumberByPlan[numplan]
                    thresh=[config.get_first_threshold_charge(DIF,1),config.get_second_threshold_charge(DIF,1),config.get_third_threshold_charge(DIF,1)]
                    runBIFeffLnAttVsLogThreshXvalue[numplan].append(attlog)
                    for x in range(3):
                        runBIFeffLnAttVsLogThreshYvalue[numplan][x].append(ROOT.Math.log10(thresh[x]))
                        key=(x+1,config.get_threshold(DIF,1,x+1))
                        #print numplan,key,setup
                        if not ThreshAttMap[numplan].get(key):
                            ThreshAttMap[numplan][key]=ThreshAttSetupList()
                        ThreshAttMap[numplan][key].add(attString,setup,runNumber)
                #if setup=='GIFPP_STRIP_22':
                #    for i in range(6):
                #        attScans[i].fill(runNumber,tree.Efficiency,tree.FakeEfficiency,cond)
            if (sourceindex==2 or (sourceindex==1 and cond.getUpAtt()==333) ):
                thrScans.fill(runNumber,config,tree.Efficiency,tree.FakeEfficiency)
                thrScansRunList.append(runNumber)

for i in range(3):
    runBIFeffLnAttVsLogThresh.append(ROOT.TGraph(len(runBIFeffLnAttVsLogThreshXvalue[5]),runBIFeffLnAttVsLogThreshXvalue[5],runBIFeffLnAttVsLogThreshYvalue[5][i]))
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



attScanList=[]
attScanNeedsRuns=set()
for i in range(6):
    planType='pad'
    if i>=4:
        planType='strip'
    for k,v in ThreshAttMap[i].iteritems():
        if (len(v.attValues)>4):
            attScanList.append(attenuatorScan(i,planType,v.runNumbers,k[0],k[1]))
            attScanNeedsRuns=attScanNeedsRuns.union(v.runNumbers)
            #print i,k

for ientry in xrange( entries ):
    nb=tree.GetEntry( ientry )
    if nb <= 0:
        continue
    runNumber=tree.runNumber
    if not runNumber in attScanNeedsRuns:
        continue
    cond=ROOT.GIF_Conditions()
    if (cond.setToRun(runNumber)==True):
        for attscan in attScanList:
            attscan.fill(runNumber,tree.Efficiency,tree.FakeEfficiency,cond)
    else:
        print "Should Never print this line"



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

def makeTText(textlist,x,y,run,nbif,rotate,vtranslate):
    letexte=" {0}".format(run)
    if (nbif<10):
        letexte=" {0} NBIF={1}".format(run,nbif)
    count=0
    if rotate:
        for textes in textlist:
            if (abs(x-textes.GetX())<50 and abs(y-textes.GetY())<0.2):
                count=count+1
    text=ROOT.TText(x,y,letexte)
    text.SetTextAlign(11)
    text.SetTextAngle(15+20*(count))
    text.SetTextSize(0.03)
    vcount=0
    if vtranslate:
        for textes in textlist:
            if (abs(x-textes.GetX())<0.01 and abs(y-textes.GetY())<0.02):
                text.SetTextColor(2+vcount)
                text.SetTextAlign(13)
                if vcount==1:
                    text.SetTextAlign(33)
                vcount=vcount+1
    return text


threshColor=[4,1,2]
strip=thrScans.glass
cEff=ROOT.TCanvas()
cEff.SetLogx()
minv=min([min(v) for v in strip.x])
maxv=max([max(v) for v in strip.x])
dummygr=ROOT.TGraph(2)
dummygr.SetPoint(0,minv*0.8,0)
dummygr.SetPoint(1,maxv*1.2,1.2)
dummygr.SetMarkerColor(0)
dummygr.GetXaxis().SetTitle("Threshold (fC)")
dummygr.GetYaxis().SetTitle("Efficiency")
dummygr.SetTitle("Glass strip chamber")
dummygr.Draw("AP")
effGr=[]
effText=[]
for ithresh in range(3):
    a=ROOT.TGraph(len(thrScans.runNumbers),strip.x[ithresh],strip.all.efficiency[ithresh])
    a.SetMarkerStyle(23)
    a.SetMarkerColor(threshColor[ithresh])
    a.Draw("P")
    effGr.append(a)
    for ival in range(a.GetN()):
        if (strip.all.efficiency[ithresh][ival]<0.6):
            text=makeTText(effText,strip.x[ithresh][ival],strip.all.efficiency[ithresh][ival],thrScans.runNumbers[ival],thrScans.numberOfBIF[ival],True,False)
            text.SetTextColor(a.GetMarkerColor())
            text.Draw()
            effText.append(text)
 
cEff.SaveAs(outputDirectory+"threshScanGIFoct2016_text.png")
del(effText[:])
del(text)
cEff.Update()
cEff.SaveAs(outputDirectory+"threshScanGIFoct2016.png")

for ithresh in range(3):
    a=ROOT.TGraph(len(thrScans.runNumbers),strip.x[ithresh],strip.gap[1].efficiency[ithresh])
    a.SetMarkerStyle(24)
    a.SetMarkerColor(threshColor[ithresh])
    a.Draw("P")
    effGr.append(a)
cEff.Update()
for ithresh in range(3):
    a=ROOT.TGraph(len(thrScans.runNumbers),strip.x[ithresh],strip.gap[2].efficiency[ithresh])
    a.SetMarkerStyle(25)
    a.SetMarkerColor(threshColor[ithresh])
    a.Draw("P")
    effGr.append(a)
cEff.Update()
legendEff=ROOT.TLegend(0.15,0.15,0.45,0.45)
legendEff.AddEntry(effGr[0],"Glass chamber ","p")
legendEff.AddEntry(effGr[3],"Glass Gap 1 ","p")
legendEff.AddEntry(effGr[6],"Glass Gap 2 ","p")
legendEff.AddEntry(effGr[0],"HR2 Threshold 1 ","p")
legendEff.AddEntry(effGr[1],"HR2 Threshold 2 ","p")
legendEff.AddEntry(effGr[2],"HR2 Threshold 3 ","p")
legendEff.Draw()
cEff.Update()
cEff.SaveAs(outputDirectory+"threshScanGIFoct2016_gap.png")
#maintenant, les memes pour bakelite
strip=thrScans.bakelite
minv=min([min(v) for v in strip.x])
maxv=max([max(v) for v in strip.x])
dummygr.SetPoint(0,minv*0.8,0)
dummygr.SetPoint(1,maxv*1.2,1.2)
dummygr.GetXaxis().SetTitle("Threshold (fC)")
dummygr.GetYaxis().SetTitle("Efficiency")
dummygr.SetTitle("Bakelite strip chamber")
dummygr.Draw("AP")
for ithresh in range(3):
    a=ROOT.TGraph(len(thrScans.runNumbers),strip.x[ithresh],strip.all.efficiency[ithresh])
    a.SetMarkerStyle(23)
    a.SetMarkerColor(threshColor[ithresh])
    a.Draw("P")
    effGr.append(a)
cEff.Update()
cEff.SaveAs(outputDirectory+"threshScanGIFoct2016_bakelite.png")
for ithresh in range(3):
    a=ROOT.TGraph(len(thrScans.runNumbers),strip.x[ithresh],strip.gap[1].efficiency[ithresh])
    a.SetMarkerStyle(24)
    a.SetMarkerColor(threshColor[ithresh])
    a.Draw("P")
    effGr.append(a)
cEff.Update()
for ithresh in range(3):
    a=ROOT.TGraph(len(thrScans.runNumbers),strip.x[ithresh],strip.gap[2].efficiency[ithresh])
    a.SetMarkerStyle(25)
    a.SetMarkerColor(threshColor[ithresh])
    a.Draw("P")
    effGr.append(a)
legendEffB=ROOT.TLegend(0.45,0.75,0.9,0.9)
legendEffB.SetNColumns(2)
legendEffB.AddEntry(effGr[0],"Bakelite chamber ","p")
legendEffB.AddEntry(effGr[0],"HR2 Threshold 1 ","p")
legendEffB.AddEntry(effGr[3],"Bakelite Gap 1 ","p")
legendEffB.AddEntry(effGr[1],"HR2 Threshold 2 ","p")
legendEffB.AddEntry(effGr[6],"Bakelite Gap 2 ","p")
legendEffB.AddEntry(effGr[2],"HR2 Threshold 3 ","p")
legendEffB.Draw()
cEff.Update()
cEff.SaveAs(outputDirectory+"threshScanGIFoct2016_bakelite_gap.png")

for i in range(6):
    for key in sorted(ThreshAttMap[i],key=lambda val: len(ThreshAttMap[i][val].attValues)):
        print i,key,len(ThreshAttMap[i][key].attValues),sorted(ThreshAttMap[i][key].setupNames)

planNames=['PAD1','PAD2','PAD3','PAD4','GLASS STRIP','BAK STRIP']
nRunMax=0
for l in ThreshAttMap:
    for v in l.itervalues():
        nrun=len(v.runNumbers)
        if nrun>nRunMax:
            nRunMax=nrun
hAttScanThresholdVsNrun=ROOT.TH2F("hAttScanThresholdVsNrun","Number of runs vs threshold scans",2,0,2,nRunMax+1,0,nRunMax+1)
hAttScanThresholdVsNrun.SetBit(ROOT.TH1.kCanRebin)

f_francois=open(outputDirectory+'pourFrancois.txt','w')

        
for i in range(6):
    for k,v in ThreshAttMap[i].iteritems():
        if len(v.attValues)>1:
                hAttScanThresholdPossibility.Fill(planNames[i]+str(k),len(v.attValues))
                nrun=str(len(v.runNumbers))
                if (len(v.runNumbers)<10):
                    nrun=' '+nrun
                hAttScanThresholdVsNrun.Fill(planNames[i]+str(k),nrun,len(v.attValues))
                for setup in v.setupNames:
                    hAttScanThresholdVsSetup.Fill(planNames[i]+str(k),setup,len(v.attValues))
                for att in v.attValues:
                    hAttScanThresholdVsAtt.Fill(planNames[i]+str(k),att,len(v.attValues))
                    hAttScanThresholdVsAttNrunWeight.Fill(planNames[i]+str(k),att,len(v.attValuesRunList[att]))
                for att in sorted(v.attValues):
                    f_francois.write( "plan={0};threshold={1};attenuator={2}; {3} runs :".format(planNames[i],str(k),att,len(v.attValuesRunList[att])))
                    for run in sorted(v.attValuesRunList[att]):
                        f_francois.write(str(run)+' ')
                    f_francois.write('\n')
                                  
f_francois.close()

cThreshPossibility=ROOT.TCanvas()
cThreshPossibility.SetBottomMargin(0.25)
cThreshPossibility.SetGridx()
hAttScanThresholdPossibility.LabelsDeflate()
hAttScanThresholdPossibility.GetXaxis().LabelsOption(">v")
hAttScanThresholdPossibility.GetYaxis().SetTitle("Number of attenuator values in runs")
hAttScanThresholdPossibility.Draw("TEXT")
hAttScanThresholdPossibility.Draw("SAME")
cThreshPossibility.SaveAs(outputDirectory+"hAttScanThresholdPossibility.png")
hAttScanThresholdPossibility.GetXaxis().LabelsOption("av")
hAttScanThresholdPossibility.Draw("TEXT")
hAttScanThresholdPossibility.Draw("SAME")
cThreshPossibility.Update()
cThreshPossibility.SaveAs(outputDirectory+"hAttScanThresholdPossibility_alpha.png")
cThreshPossibility.SetGridy()
cThreshPossibility.SetLeftMargin(0.15)
hAttScanThresholdVsSetup.LabelsDeflate("X")
hAttScanThresholdVsSetup.LabelsDeflate("Y")
hAttScanThresholdVsSetup.GetXaxis().LabelsOption("av")
hAttScanThresholdVsSetup.GetYaxis().LabelsOption("a")
hAttScanThresholdVsSetup.Draw("COL")
hAttScanThresholdVsSetup.Draw("TEXT SAME")
cThreshPossibility.Update()
cThreshPossibility.SaveAs(outputDirectory+"hAttScanThresholdVsSetup.png")
hAttScanThresholdVsNrun.LabelsDeflate("X")
hAttScanThresholdVsNrun.LabelsDeflate("Y")
hAttScanThresholdVsNrun.GetXaxis().LabelsOption("av")
hAttScanThresholdVsNrun.GetYaxis().LabelsOption("a")
hAttScanThresholdVsNrun.GetYaxis().SetTitle("Number of runs")
hAttScanThresholdVsNrun.Draw("COL")
hAttScanThresholdVsNrun.Draw("TEXT SAME")
cThreshPossibility.Update()
cThreshPossibility.SaveAs(outputDirectory+"hAttScanThresholdVsNrun.png")
hAttScanThresholdVsAtt.LabelsDeflate("X")
hAttScanThresholdVsAtt.LabelsDeflate("Y")
hAttScanThresholdVsAtt.GetXaxis().LabelsOption("av")
hAttScanThresholdVsAtt.GetYaxis().LabelsOption("a")
hAttScanThresholdVsAtt.Draw("COL")
hAttScanThresholdVsAttNrunWeight.LabelsDeflate("X")
hAttScanThresholdVsAttNrunWeight.LabelsDeflate("Y")
hAttScanThresholdVsAttNrunWeight.GetXaxis().LabelsOption("av")
hAttScanThresholdVsAttNrunWeight.GetYaxis().LabelsOption("a")
hAttScanThresholdVsAttNrunWeight.Draw("TEXT SAME")
cThreshPossibility.Update()
cThreshPossibility.SaveAs(outputDirectory+"hAttScanThresholdVsAtt.png")

keepItAroundForROOT=[]

def drawAttScanPlot(att,corr=False):
    att.printItsort(2)
    effgr=ROOT.TGraph( len(att.runNumbers), att.x, att.planeEff.eff)
    fakeeffgr=ROOT.TGraph( len(att.runNumbers), att.x, att.planeEff.fakeEff)
    corrEff=array('f')
    for i in range(len(att.runNumbers)):
        corrEff.append((att.planeEff.eff[i]-att.planeEff.fakeEff[i])/(1-att.planeEff.fakeEff[i]))
    correffgr=ROOT.TGraph( len(att.runNumbers), att.x, corrEff)
    effgr.SetMarkerStyle(23)
    fakeeffgr.SetMarkerStyle(24)
    correffgr.SetMarkerStyle(25)
    effgr.SetMarkerColor(threshColor[att.thresholdLevel-1])
    fakeeffgr.SetMarkerColor(threshColor[att.thresholdLevel-1])
    correffgr.SetMarkerColor(threshColor[att.thresholdLevel-1])
    dummygr=ROOT.TGraph(2)
    dummygr.SetPoint(0,min(att.x),0)
    #dummygr.SetPoint(0,1e-6,0)
    dummygr.SetPoint(1,max(att.x),1)
    dummygr.GetXaxis().SetTitle("1/attenuator")
    dummygr.GetYaxis().SetTitle("efficiency")
    dummygr.SetTitle("{} thresholdLevel={}, thresholdDAC={}".format(planNames[att.planNumber],att.thresholdLevel,att.thresholdValue))
    dummygr.Draw("AP")
    effgr.Draw("P")
    fakeeffgr.Draw("P")
    if (corr):
        correffgr.Draw("P")
    keepItAroundForROOT.append((dummygr,effgr,fakeeffgr,correffgr))

cAttScan=ROOT.TCanvas()
drawAttScanPlot(attScanList[0])
legendAtt=ROOT.TLegend(0.5,0.7,0.8,0.8)
legendAtt.AddEntry(keepItAroundForROOT[0][1],"Raw efficiency","p")
legendAtt.AddEntry(keepItAroundForROOT[0][2],"Fake efficiency","p")
legendAtt.Draw()
effText=[]
a_effgr=keepItAroundForROOT[0][1]
for ival in range(a_effgr.GetN()):
    x=attScanList[0].x[ival]
    y=attScanList[0].planeEff.eff[ival]
    if x>0.21 and x<0.22 and y>0:
        text=makeTText(effText,x,y,attScanList[0].runNumbersFillList[ival],attScanList[0].numberOfBIF[ival],False,False)
        text.SetTextColor(a_effgr.GetMarkerColor())
        text.Draw()
        effText.append(text) 
cAttScan.Update()
cAttScan.SaveAs(outputDirectory+"AttScanPAD1_text.png")
drawAttScanPlot(attScanList[14])
legendAtt=ROOT.TLegend(0.15,0.5,0.45,0.6)
legendAtt.AddEntry(keepItAroundForROOT[1][1],"Raw efficiency","p")
legendAtt.AddEntry(keepItAroundForROOT[1][2],"Fake efficiency","p")
legendAtt.Draw()
effText=[]
a_effgr=keepItAroundForROOT[1][1]
for ival in range(a_effgr.GetN()):
    x=attScanList[14].x[ival]
    y=attScanList[14].planeEff.eff[ival]
    if x>0.09:
        text=makeTText(effText,x,y,attScanList[14].runNumbersFillList[ival],attScanList[14].numberOfBIF[ival],False,True)
        #text.SetTextColor(a_effgr.GetMarkerColor())
        text.Draw()
        effText.append(text) 
cAttScan.Update()
cAttScan.SaveAs(outputDirectory+"AttScanGlassStrip_text.png")
raw_input("\n\nPress the enter key to exit.")

#cAttScan.SetLogy()
#cAttScan.SetLogx()
#for att in attScanList[0:1]:
for att in attScanList:
    drawAttScanPlot(att,True)
    cAttScan.Update()
    attfile=outputDirectory+"AttScan_plan_{}_threshold_{}_DAC_{}.png".format(planNames[att.planNumber],att.thresholdLevel,att.thresholdValue)
    cAttScan.SaveAs(attfile)
    raw_input("\n\nPress the enter key to exit.")

#a.SetMarkerColor(4)
#a.Draw("AP")





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

writeframe(mylatex,"Threshold scans(source OFF or almost) glass chamber","threshScanGIFoct2016_text.png",0.35,
"Efficiency as a function of threshold for strip Glass chamber for source OFF or source attenuation=46000. On the plot are printed the run number for which efficiency is below 60\%. 5 runs have a zero efficiency. In the elog, it is mentionned for 3 of them that HV was at 5~kV, it is likely that it was the same for the 2 other runs.")

writeframe(mylatex,"Threshold scans(source OFF or almost) glass chamber","threshScanGIFoct2016.png",0.4,
"Efficiency as a function of threshold for strip Glass chamber for source OFF or source attenuation=46000 without the text. Each marker corresponds to one run.")

writeframe(mylatex,"Threshold scans(source OFF or almost) glass chamber","threshScanGIFoct2016_gap.png",0.4,
"Efficiency as a function of threshold for strip Glass chamber for source OFF or source attenuation=46000. Each marker of one type corresponds to one run.")

writeframe(mylatex,"Threshold scans(source OFF or almost) bakelite chamber","threshScanGIFoct2016_bakelite.png",0.4,
"Efficiency as a function of threshold for strip Bakelite chamber for source OFF or source attenuation=46000. Each marker corresponds to one run. The third HR2 threshold was used in a non calibrated regime (true also for glass strip RPC).")

writeframe(mylatex,"Threshold scans(source OFF or almost) bakelite chamber","threshScanGIFoct2016_bakelite_gap.png",0.4,
"Efficiency as a function of threshold for strip Bakelite chamber for source OFF or source attenuation=46000. The third HR2 threshold was used in a non calibrated regime (true also for glass strip RPC).")

writeframe(mylatex,"Source scan","hAttScanThresholdPossibility.png",0.4,
"Number of attenuator values versus threshold ( chamber, comparator=1/2/3, comparator DAC value) for thresholds used for at least 2 attenuator values.")

writeframe(mylatex,"Source scan","hAttScanThresholdPossibility_alpha.png",0.4,
"Number of attenuator values versus threshold ( chamber, comparator=1/2/3, comparator DAC value) for thresholds used for at least 2 attenuator values. X axis in alphabetical order")

writeframe(mylatex,"Source scan","hAttScanThresholdVsSetup.png",0.4,
"Configuration setup name as a function of threshold ( chamber, comparator=1/2/3, comparator DAC value) for thresholds used for at least 2 attenuator values. Z axis is the number of attenuator values for the corresonding threshold.")

writeframe(mylatex,"Source scan","hAttScanThresholdVsNrun.png",0.4,
"Number of runs as a function of threshold ( chamber, comparator=1/2/3, comparator DAC value) for thresholds used for at least 2 attenuator values. Z axis is the number of attenuator values for the corresonding threshold.")

writeframe(mylatex,"Source scan","hAttScanThresholdVsAtt.png",0.4,
"Attenuator values as a function of threshold ( chamber, comparator=1/2/3, comparator DAC value) for thresholds used for at least 2 attenuator values. Z axis color (resp number) is number of attenuator values (resp number of runs taken with this attenuator value) for the corresonding threshold.")

writeframe(mylatex,"Source scan","AttScanPAD1_text.png",0.35,
"Efficiency as a function of inverse attenuator value for PAD chamber closest to the source. During runs 733717 and 733721 record, there have been GIF access (source OFF). Run 733726 is a short run with low statistics. Run 733753 is the only long run with source state unchanged during the run.")

writeframe(mylatex,"Source scan","AttScanGlassStrip_text.png",0.4,
"Efficiency as a function of inverse attenuator value for Glass strip chamber. Run numbers written for points with low attenuator value. A lot of run quality check is needed. Lots of runs have less than 10 BIF triggers recorded.")

writeframe(mylatex,"Source scan","AttScanGlassStrip_text.png",0.3,
"For attenuator 4.6, removing too short (low stat) runs and runs where source was OFF sometimes, only run 733753 is potentially good. For attenuator 3.3, no indication in the elog points to problem. For attenuator 10, no problem written in elog for run 733687 and run 733755 is the last run before SPS breaks. Its end is without beam and no source but the period with beam correspond to stable source condition and so should be OK.")


mylatex.write('\end{document}')

mylatex.close()

os.system('cd {}; pdflatex {}'.format(outputDirectory,mylatexfilename))
#raw_input("\n\nPress the enter key to exit.")

runList=""
for r in sorted(thrScansRunList):
    runList=runList+" {}".format(r)
print runList
