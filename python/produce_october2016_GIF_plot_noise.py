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
if ROOT.gROOT.GetVersion()[0]=='6':
    #ROOT 6.08.02 don't understand non template dictionnary without it (don't know why)
    dummy=ROOT.intervalle('unsigned int')()


experience=ROOT.GIF_oct2016_ExperimentalSetup()
all=ROOT.all_ConfigInfo.instance()

f = ROOT.TFile( 'GIF_oct2016.root')
tree = ROOT.gDirectory.Get( 'results' )
entries = tree.GetEntriesFast()

minrun=tree.GetMinimum('runNumber')
maxrun=tree.GetMaximum('runNumber')

print minrun,maxrun

#have computed noise as mean number of hits in a 10 microsecond window 
#so mean number of hits in a one second window is times 1/10e-6=1/1e-5=1e5=100000
scaleToHz=1e5
chamberSurfacecm=30.0*30.0

class runDataPoint:
    def __init__(self):
        self.noise=0
        self.stat=0
        self.runNumber=0
        self.invattenuator=0
        self.attenuatorText='OFF'
        self.thresholdLevel=0
        self.thresholdDAC=0
        self.thresholdVal=0
        self.planNumber=0

    def __repr__(self):
        return "plan={},thresh=({} at {} -> {}fC),attenuator={},run={},stat={},noise={} (unit?)".format(self.planNumber,self.thresholdLevel,self.thresholdDAC,self.thresholdVal,self.attenuatorText,self.runNumber,self.stat,self.noise)

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
    if setup=='GIFPP_OYONAX_8' or setup=='GIFPP_OYONAX_12' : #Wrong config
        continue
    cond=ROOT.GIF_Conditions()
    if cond.setToRun(runNumber)==False : #skip run without known GIF condition
        continue
    if cond.getSourceStatus()==ROOT.GIF_Conditions.UNKNOWN : #skip run without known GIF source state
        continue
    #noise study would be enough from here
    config=all.getConfigInfo(runNumber)
    noiseData=tree.Noise
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
            A.stat=noiseData.n_event
            if cond.getSourceStatus()==ROOT.GIF_Conditions.OFF :
                A.invattenuator=0
                A.attenuatorText="{:8}".format("OFF")
            if cond.getSourceStatus()==ROOT.GIF_Conditions.ON :
                A.invattenuator=1/cond.getUpAttValue()
                A.attenuatorText="{:8.1f}".format(cond.getUpAttValueApprox())
            A.thresholdLevel=thresholdLevel
            A.thresholdDAC=thresh[thresholdLevel-1]
            A.thresholdVal=threshval[thresholdLevel-1]
            A.noise=noiseData.sumcount(thresholdLevel,numeroPlan)*scaleToHz/noiseData.n_event/chamberSurfacecm
            allRunDataPoint.append(A)


print len(allRunDataPoint)
runSummary=dict()
noiseSummary=dict()
for ar in allRunDataPoint:
    key=(ar.planNumber,ar.thresholdLevel,ar.thresholdDAC)
    if not runSummary.has_key(key):
        runSummary[key]=dict()
        noiseSummary[key]=[array('f'),array('f'),[]] #1/att,noise,allData
    if not runSummary[key].has_key(ar.attenuatorText):
        runSummary[key][ar.attenuatorText]=[]
    runSummary[key][ar.attenuatorText].append(ar.runNumber)
    noiseSummary[key][0].append(ar.invattenuator)
    noiseSummary[key][1].append(ar.noise)
    noiseSummary[key][2].append(ar)

separator="#########################################################"
separator=separator+'\n'+separator
print separator
print "For noise attenuator scans :"
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
listGraphPoint=[]
for rk,rv in runSummary.iteritems():
    if len(rv)>2 : #at least 3 attenuator values
        print "Numero Plan={}, thresholdLevel={}, thresholdDACvalue={} : {}".format(rk[0],rk[1],rk[2],len(rv))
        listGraphPoint.append(rk)

# graphs to produce : 
# listGraphPoint=[(0,1,170),(0,2,250),(4,1,110),(4,1,140),(4,2,106),(4,3,106),(5,1,100),(5,1,110),(5,1,130),(5,2,106),(5,3,106)]


threshColor=[4,1,2]

graphs=dict()

def threshConverter(threshLevel,threshDAC): #threshlevel=1, 2 or 3
    pedestal=[90,98,98]
    factor=[0.7,0.08,0.0163]
    t=threshLevel-1
    return (threshDAC-pedestal[t])/factor[t]

def setGraphProperties(agraph,apoint,what,marker):
    agraph.SetMarkerStyle(marker)
    agraph.SetMarkerColor(threshColor[apoint[1]-1])
    if apoint[0]<4:
        chamber="PAD {}".format(apoint[0])
    if apoint[0]==4:
        chamber="GRPC strip"
    if apoint[0]==5:
        chamber="Bakelite strip"
    agraph.SetTitle("{}, threshold Level={}, threshold DAC={} ({} fC)".format(chamber,apoint[1],apoint[2],threshConverter(apoint[1],apoint[2])))
    agraph.SetName("{}_{}_{}".format(chamber,apoint[1],apoint[2]))
    agraph.GetXaxis().SetTitle("1/source attenuator")
    agraph.GetYaxis().SetTitle(what)

for point in listGraphPoint:
    toplot=noiseSummary[point]
    graphs[point]=[ROOT.TGraph(len(toplot[0]),toplot[0],toplot[1]),toplot[2]] # [graph,datapoint]
    setGraphProperties(graphs[point][0],point,"Noise (Hz/cm2)",23)



threshRanges=dict()
threshRanges[1]=[10,300]
threshRanges[2]=[70,2000]
threshRanges[3]=[1000,15000]

threshScanSummary=dict()
maxThresh=0
maxNoiseGlass=0
maxNoiseBakelite=0
textPoints=[]
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
        threshScanSummary[key]=[array('f'),array('f'),[]] #thresh,noise,allData
    if ar.thresholdVal>maxThresh:
        maxThresh=ar.thresholdVal
    if ar.noise>maxNoiseGlass and ar.planNumber==4:
        maxNoiseGlass=ar.noise
    if ar.noise>maxNoiseBakelite and ar.planNumber==5:
        maxNoiseBakelite=ar.noise
    threshScanSummary[key][0].append(ar.thresholdVal)
    threshScanSummary[key][1].append(ar.noise)
    threshScanSummary[key][2].append(ar)
    textPoints.append("plan {}, threshLevel {} ({:7.2f} fC), noise={:7.4f} Hz/cm2, run={}".format(ar.planNumber,ar.thresholdLevel,ar.thresholdVal,ar.noise,ar.runNumber))

for key,val in  threshScanSummary.iteritems():
    #val.append(ROOT.TGraph(len(val[0]),val[0],val[1]))
    val.append(ROOT.TGraph(len(val[0]),val[0],val[1]))
    val[3].SetMarkerStyle(23)
    val[3].SetMarkerColor(threshColor[key[1]-1])
    val[3].SetLineColor(threshColor[key[1]-1])


outputDirectory='october2016_GIF_plots'
if not os.path.isdir(outputDirectory):
   os.mkdir(outputDirectory)
outputDirectory=outputDirectory+'/'

noiseFile=ROOT.TFile(outputDirectory+"noiseWork.root", "RECREATE")

cAttScan=ROOT.TCanvas()
for pointval,pointgr in graphs.iteritems():
    pointgr[0].Draw("AP")
    TTextList=[]
    for ar in pointgr[1]:
        print ar.attenuatorText,ar.invattenuator,ar.noise,ar.stat,ar.runNumber
        text=ROOT.TText(ar.invattenuator,ar.noise," {}".format(ar.runNumber))
        text.SetTextAlign(11)
        text.SetTextSize(0.03)
        text.Draw()
        TTextList.append(text)
    cAttScan.SetName("c_{}".format(pointgr[0].GetName()))
    cAttScan.Update()
    cAttScan.Write()
    #cAttScan.SaveAs(outputDirectory+"AttScan_plan{}_threshold{}_atDAC{}_noise.png".format(pointval[0],pointval[1],pointval[2]))
    raw_input("\n\nPress the enter key to exit.")



threshCanv=ROOT.TCanvas()
threshCanv.SetLogx()
threshCanv.SetLogy()

dummygr=ROOT.TGraph(2)
dummygr.SetPoint(0,1,1e-3)
dummygr.SetPoint(1,maxThresh*1.2,maxNoiseGlass*1.2)
dummygr.SetMarkerColor(0)
dummygr.GetXaxis().SetTitle("Threshold (fC)")
dummygr.GetYaxis().SetTitle("Noise (Hz/cm2)")


legend=ROOT.TLegend(0.12,0.15,0.52,0.4)


dummygr.SetTitle("Glass strip chamber")
threshCanv.SetName("Strip_threshold_scan_glass")
dummygr.Draw("AP")
for i in [1,2,3]:
    threshScanSummary[(4,i)][3].Draw("P")
    legend.AddEntry(threshScanSummary[(4,i)][3],"threshold {} : range {} fC to {} fC".format(i,threshRanges[i][0],threshRanges[i][1]),"p")
#legend.Draw()
threshCanv.Update()
threshCanv.Write()
raw_input("\n\nPress the enter key to exit.")


dummygr.SetTitle("Bakelite strip chamber")
threshCanv.SetName("Strip_threshold_scan_bakelite")
#dummygr.SetPoint(1,maxThresh*1.2,maxNoiseBakelite*1.2)
dummygr.Draw("AP")
for i in [1,2,3]:
    threshScanSummary[(5,i)][3].Draw("P")
#legend.Draw()
threshCanv.Update()
threshCanv.Write()
raw_input("\n\nPress the enter key to exit.")


noiseFile.Close()

for text in sorted(textPoints):
    print text
