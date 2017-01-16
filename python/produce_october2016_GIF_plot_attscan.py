#!/usr/bin/python

#use the tree produced by produce_run_tree_GIF.py

import os
from array import array
from operator import attrgetter
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


class runDataPoint:
    def __init__(self):
        self.addEff(0.0,0.0)
        self.runNumber=0
        self.numberOfBIF=0
        self.invattenuator=0
        self.attenuatorText='OFF'
        self.thresholdLevel=0
        self.thresholdDAC=0
        self.thresholdVal=0
        self.planNumber=0

    def addEff(self,effVal,fakeEffVal):
        self.eff=effVal
        self.fakeEff=fakeEffVal
        self.correctedEff=(self.eff-self.fakeEff)/(1-self.fakeEff)

    def __repr__(self):
        return "plan={},thresh=({} at {} -> {}fC),attenuator={},nBIF={},run={},eff=({},{},{})".format(self.planNumber,self.thresholdLevel,self.thresholdDAC,self.thresholdVal,self.attenuatorText,self.numberOfBIF,self.runNumber,self.eff,self.fakeEff,self.correctedEff)

allRunDataPoint=[]
DIFnumberByPlan=[125,32,28,6,26,2]

for ientry in xrange( entries ):
    nb=tree.GetEntry( ientry )
    if nb <= 0:
        continue
    runNumber=tree.runNumber
    setup=all.getConfigName(runNumber)
    if setup == ROOT.all_ConfigInfo.UnknownConfig :  #skip run without known config
        continue
    cond=ROOT.GIF_Conditions()
    if cond.setToRun(runNumber)==False : #skip run without known GIF condition
        continue
    if cond.getSourceStatus()==ROOT.GIF_Conditions.UNKNOWN : #skip run without known GIF source state
        continue
    #noise study would be enough from here
    if cond.getYuris_small_scintillator_in_BIF()==False : #skip run without Yuri's trigger
        continue
    if tree.Efficiency.n_event==0 : #skip runs without trigger 
        continue
    if tree.Efficiency.n_event<50 : #skip runs without enough trigger 
        print "run removed because less than 50 BIF triggers :", runNumber
        continue
    config=all.getConfigInfo(runNumber)
    effData=tree.Efficiency
    fakeEffData=tree.FakeEfficiency
    # pour chaque plan et chaque seuil remplir un runDataPoint avec valeur du seuil depuis config,attenuator depuis cond (remplir si runquality le permet)
    # boucle sur les numeros de plan
    #   check si run quality = good
    #   boucle sur les seuils
    #      remplir les ifos dans runDataPoint
    for numeroPlan in range(6) : #6 plans
        runqual=all.getRunQualityInfo(runNumber)
        if runqual.runIsGoodForPlan(numeroPlan)==False:
            print "run removed for plan number {} because run is bad : {}".format(numeroPlan,runNumber)
            continue
        DIF=DIFnumberByPlan[numeroPlan]
        thresh=[config.get_first_threshold(DIF,1),config.get_second_threshold(DIF,1),config.get_third_threshold(DIF,1)]
        threshval=[config.get_first_threshold_charge(DIF,1),config.get_second_threshold_charge(DIF,1),config.get_third_threshold_charge(DIF,1)]
        for thresholdLevel in [1,2,3] :
            A=runDataPoint()
            A.planNumber=numeroPlan
            A.runNumber=runNumber
            A.numberOfBIF=tree.Efficiency.n_event
            if cond.getSourceStatus()==ROOT.GIF_Conditions.OFF :
                A.invattenuator=0
                A.attenuatorText="{:8}".format("OFF")
            if cond.getSourceStatus()==ROOT.GIF_Conditions.ON :
                A.invattenuator=1/cond.getUpAttValue()
                A.attenuatorText="{:8.1f}".format(cond.getUpAttValueApprox())
            A.thresholdLevel=thresholdLevel
            A.thresholdDAC=thresh[thresholdLevel-1]
            A.thresholdVal=threshval[thresholdLevel-1]
            eff=tree.Efficiency.flagcount(thresholdLevel,numeroPlan)*1.0/tree.Efficiency.n_event
            fakeEff=tree.FakeEfficiency.flagcount(thresholdLevel,numeroPlan)*1.0/tree.FakeEfficiency.n_event
            A.addEff(eff, fakeEff)
            allRunDataPoint.append(A)


print len(allRunDataPoint)
runSummary=dict()
effSummary=dict()
for ar in allRunDataPoint:
    key=(ar.planNumber,ar.thresholdLevel,ar.thresholdDAC)
    if not runSummary.has_key(key):
        runSummary[key]=dict()
        effSummary[key]=[array('f'),array('f'),array('f'),array('f'),[]] #1/att,eff,fakeEff,correctedEff,allData
    if not runSummary[key].has_key(ar.attenuatorText):
        runSummary[key][ar.attenuatorText]=[]
    runSummary[key][ar.attenuatorText].append(ar.runNumber)
    effSummary[key][0].append(ar.invattenuator)
    effSummary[key][1].append(ar.eff)
    effSummary[key][2].append(ar.fakeEff)
    effSummary[key][3].append(ar.correctedEff)
    effSummary[key][4].append(ar)



separator="#########################################################"
separator=separator+'\n'+separator
print separator
print "For attenuator scans :"
for rk,rv in runSummary.iteritems():
    if len(rv)>2 : #at least 3 attenuator values
        print "Numero Plan={}, thresholdLevel={}, thresholdDACvalue={}:".format(rk[0],rk[1],rk[2])
        for rrk in sorted(rv):
            runList=""
            for r in sorted(rv[rrk]):
                runList=runList+" {}".format(r)
            print "   attenuator={} : runList= {}".format(rrk,runList)

print separator
print " summary :"
for rk,rv in runSummary.iteritems():
    if len(rv)>2 : #at least 3 attenuator values
        print "Numero Plan={}, thresholdLevel={}, thresholdDACvalue={} : {}".format(rk[0],rk[1],rk[2],len(rv))

# graphs to produce : (runs with at least 50 BIF triggers)
# 
# Numero Plan=0, thresholdLevel=1, thresholdDACvalue=170 : 7
# Numero Plan=0, thresholdLevel=2, thresholdDACvalue=250 : 8
#
# Numero Plan=4, thresholdLevel=1, thresholdDACvalue=110 : 4
# Numero Plan=4, thresholdLevel=1, thresholdDACvalue=140 : 5
# Numero Plan=4, thresholdLevel=2, thresholdDACvalue=106 : 7
# Numero Plan=4, thresholdLevel=3, thresholdDACvalue=106 : 7
#
# Numero Plan=5, thresholdLevel=1, thresholdDACvalue=100 : 3
# Numero Plan=5, thresholdLevel=1, thresholdDACvalue=110 : 3
# Numero Plan=5, thresholdLevel=1, thresholdDACvalue=130 : 5
# Numero Plan=5, thresholdLevel=2, thresholdDACvalue=106 : 7
# Numero Plan=5, thresholdLevel=3, thresholdDACvalue=106 : 7

listGraphPoint=[(0,1,170),(0,2,250),(4,1,110),(4,1,140),(4,2,106),(4,3,106),(5,1,100),(5,1,110),(5,1,130),(5,2,106),(5,3,106)]

def extractPoints(allDataPoint, numplan, tL, tDAC):
    extracted=[]
    for ar in allDataPoint:
        if numplan==ar.planNumber and tL==ar.thresholdLevel and tDAC==ar.thresholdDAC:
            extracted.append(ar)
    return extracted

threshColor=[4,1,2]

graphs=dict()

def setGraphProperties(agraph,apoint,what,marker):
    agraph.SetMarkerStyle(marker)
    agraph.SetMarkerColor(threshColor[apoint[1]-1])
    if apoint[0]<4:
        chamber="PAD {}".format(apoint[0])
    if apoint[0]==4:
        chamber="GRPC strip"
    if apoint[0]==5:
        chamber="Bakelite strip"
    agraph.SetTitle("{}, threshold Level={}, threshold DAC={}".format(chamber,apoint[1],apoint[2]))
    agraph.GetXaxis().SetTitle("1/source attenuator")
    agraph.GetYaxis().SetTitle(what)

for point in listGraphPoint:
    graphs[point]=[]
    toplot=effSummary[point]
    for i in [1,2,3]:
        graphs[point].append(ROOT.TGraph(len(toplot[0]),toplot[0],toplot[i]))
    setGraphProperties(graphs[point][0],point,"efficiency",23)
    setGraphProperties(graphs[point][1],point,"fake efficiency",24)
    setGraphProperties(graphs[point][2],point,"corrected efficiency",25)

outputDirectory='october2016_GIF_plots'
if not os.path.isdir(outputDirectory):
   os.mkdir(outputDirectory)
outputDirectory=outputDirectory+'/'

cAttScan=ROOT.TCanvas()
graphis=["eff","fakeEff","corrEff"]
for pointval,pointgr in graphs.iteritems():
    i=0
    for gr in pointgr:
        gr.Draw("AP")
        cAttScan.Update()
        cAttScan.SaveAs(outputDirectory+"AttScan_plan{}_threshold{}_atDAC{}_{}.png".format(pointval[0],pointval[1],pointval[2],graphis[i]))
        i=i+1
        #raw_input("\n\nPress the enter key to exit.")


threshRanges=dict()
threshRanges[1]=[10,300]
threshRanges[2]=[70,2000]
threshRanges[3]=[1000,15000]

threshScanSummary=dict()
maxThresh=0
for ar in allRunDataPoint:
    if ar.planNumber<4 :  #remove pad chamber
        continue
    if ar.invattenuator>1.1/46000.0 : #remove attenuator values below 46000
        continue
    #select roughly correct range for threshold
    if ar.thresholdVal<threshRanges[ar.thresholdLevel][0] or ar.thresholdVal>threshRanges[ar.thresholdLevel][1]:
        print "removing {} {}".format(ar.thresholdLevel,ar.thresholdVal)
        continue
    key=(ar.planNumber,ar.thresholdLevel)
    if not threshScanSummary.has_key(key):
        threshScanSummary[key]=[array('f'),array('f'),array('f'),array('f'),[]] #thresh,eff,fakeEff,correctedEff,allData
    if ar.thresholdVal>maxThresh:
        maxThresh=ar.thresholdVal
    threshScanSummary[key][0].append(ar.thresholdVal)
    threshScanSummary[key][1].append(ar.eff)
    threshScanSummary[key][2].append(ar.fakeEff)
    threshScanSummary[key][3].append(ar.correctedEff)
    threshScanSummary[key][4].append(ar)


    

print separator
print maxThresh


for key,val in  threshScanSummary.iteritems():
    val.append(ROOT.TGraph(len(val[0]),val[0],val[1]))
    val[5].SetMarkerStyle(23)
    val[5].SetMarkerColor(threshColor[key[1]-1])

threshCanv=ROOT.TCanvas()
threshCanv.SetLogx()

dummygr=ROOT.TGraph(2)
dummygr.SetPoint(0,0,0)
dummygr.SetPoint(1,maxThresh*1.2,1.2)
dummygr.SetMarkerColor(0)
dummygr.GetXaxis().SetTitle("Threshold (fC)")
dummygr.GetYaxis().SetTitle("Efficiency")

legend=ROOT.TLegend(0.12,0.15,0.52,0.4)


dummygr.SetTitle("Glass strip chamber")
dummygr.Draw("AP")
for i in [1,2,3]:
    threshScanSummary[(4,i)][5].Draw("P")
    legend.AddEntry(threshScanSummary[(4,i)][5],"threshold {} : range {} fC to {} fC".format(i,threshRanges[i][0],threshRanges[i][1]),"p")
legend.Draw()
threshCanv.SaveAs(outputDirectory+"thresh_scan_glass_clean.png")
threshCanv.SaveAs(outputDirectory+"thresh_scan_glass_clean.root")
raw_input("\n\nPress the enter key to exit.")

dummygr.SetTitle("Bakelite strip chamber")
dummygr.Draw("AP")
for i in [1,2,3]:
    threshScanSummary[(5,i)][5].Draw("P")
legend.Draw()
threshCanv.Update()
threshCanv.SaveAs(outputDirectory+"thresh_scan_bakelite_clean.png")
threshCanv.SaveAs(outputDirectory+"thresh_scan_bakelite_clean.root")
raw_input("\n\nPress the enter key to exit.")
