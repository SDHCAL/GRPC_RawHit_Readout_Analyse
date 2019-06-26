#
#  event  :  BIF, readoutNumber, AbsoluteBCID, DIF BCID, event_time_intervalle
#
#  hit : x, y, z, threshold, timestamp, DIF, ASIC, CHANNEL
#
#

from array import array
import os.path
import itertools
import pyLCIO
import ROOT
ROOT.gROOT.Reset()

ROOT.gROOT.ProcessLine(
"struct MyEventTime {\
UInt_t     readoutCounter;\
Int_t      BCID;\
ULong64_t  AbsBCID;\
Int_t     EventClockStamp;\
UInt_t     spillNumber;\
UInt_t     clockCountInSpill;\
};" );

from ROOT import MyEventTime


#
#  NB : Guillaume Trivent's cut
#
# min number of consecutive layers with hits = 6
# no plan with more than 400 hits
# absence of ASIC with 64 hits in the event
# no DIF with more than 70 hits in channels 29 and 31 (RamFull Events)
# 
ROOT.gROOT.ProcessLine(
"struct GuillaumeVariables {\
int     RamFullCount;\
int     NFullAsic;\
int     MaxHitInPlan;\
int     MaxNConsecutiveLayers;\
int     MaxNConsecutiveLayersWithOneHole;\
};" );

from ROOT import GuillaumeVariables


def computeMaxNumberOfConsecutivePlan(orderedListOfFiredPlans):
    if len(orderedListOfFiredPlans)==1:
        return 1
    pp=[orderedListOfFiredPlans[i]-orderedListOfFiredPlans[i-1]  for i in range(1,len(orderedListOfFiredPlans))]  #pp is difference list of orderedListOfFiredPlans, consecutive plans correspond to 1
    consecutiveSize=[len(list(group))+1 for key,group in itertools.groupby(pp) if key==1] #group pp by consecutive value, keep ajacent group for wich pp[i]==1, list the group size (NB : +1 because list of difference (pp) has one element less than original list of plan number (orderedListOfFiredPlans) )
    if len(consecutiveSize):
        return max(consecutiveSize)
    else:
        return 1


def computeMaxNumberOfConsecutivePlanWithOneHole(orderedListOfFiredPlans):
    HolePlans=[orderedListOfFiredPlans[i] for i in range(1,len(orderedListOfFiredPlans)) if orderedListOfFiredPlans[i]-orderedListOfFiredPlans[i-1]==2]
    if len(HolePlans)==0:
        return computeMaxNumberOfConsecutivePlan(orderedListOfFiredPlans)
    else:
        listOfMax=[]
        for holes in HolePlans:
            copylistplan=[i for i in orderedListOfFiredPlans]
            copylistplan.append(holes-1)
            copylistplan.sort()
            listOfMax.append(computeMaxNumberOfConsecutivePlan(copylistplan))
        return max(listOfMax)

def makeTree(lciofileName,guillaumeCutVar=True):

    Unset=0
    Guillaume=1
    Gerald=2
    Antoine=3
    mode=Unset

    #determine mode
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    ev=lcReader.readNextEvent()
    ke=ROOT.vector('string')()
    ev.parameters().getIntKeys(ke)
    kf=ROOT.vector('string')()
    ev.parameters().getFloatKeys(kf)
    if 'beamEnergy' in kf:
        mode=Antoine
    if 'Sub_Readout_Frame_start_time_in_trigger' in ke:
        mode=Gerald
    if 'eventTimeInTrigger' in ke:
        mode=Guillaume
    print " mode is set to value = {0}".format(mode)
    lcReader.close()

    if mode==Guillaume or mode==Antoine:
        #get the min,max of hit time for each readout
        triggerdict=dict()
        lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
        lcReader.open( lciofileName )
        triggerNumber=-1
        for ev in lcReader:
            evparam=ev.parameters()
            triggerCounter=evparam.getIntVal('trigger')
            if triggerCounter != triggerNumber:
                #new readout
                triggerNumber=triggerCounter
                AbsBCIDup=evparam.getIntVal('bcid1')
                AbsBCIDlow=evparam.getIntVal('bcid2')
                absBCIDtrigger=(AbsBCIDup<<24)+AbsBCIDlow
                triggerdict[triggerCounter]=[absBCIDtrigger,0]
            lcCol=ev.getCollection("SDHCAL_HIT")
            for hit in lcCol:
                hit_ts=int(hit.getTime())
                if triggerdict[triggerCounter][1] < hit_ts:
                    triggerdict[triggerCounter][1]=hit_ts
        lcReader.close()
        firstReadout=min(triggerdict)
        lastReadout=max(triggerdict)
        spillNumber=0
        startspill=triggerdict[firstReadout][0]-triggerdict[firstReadout][1]
        spills=[]
        spills.append([startspill,[firstReadout] ])
        ireadout = firstReadout+1
        while ireadout <= lastReadout:
            if triggerdict.has_key(ireadout):
                if (triggerdict[ireadout][0]-triggerdict[spills[spillNumber][1][0]][0])*2e-7>10:
                    #more than XX seconds between 2 readouts, consider it is from different spills
                    spillNumber=spillNumber+1
                    startspill=triggerdict[ireadout][0]-triggerdict[ireadout][1]
                    spills.append([startspill,[ireadout] ])
                else:
                    spills[spillNumber][1].append(ireadout)
            ireadout=ireadout+1
        # spills is a list with elements containing spillstart and a list of readout that are part of this spill
        # repackage stuff to have spill number and startspill per trigger readout
        triggerSpillDict=dict()
        for i in range(len(spills)):
            for triggers in spills[i][1]:
                triggerSpillDict[triggers]=[i+1,spills[i][0]]
        print "Guillaume/Antoine mode spill analysis finished"
    
    rootfileName=os.path.basename(lciofileName).split('.')[0]+'.root'
    f=ROOT.TFile(rootfileName,"recreate")
    t=ROOT.TTree('SDHCAL','SDHCAL events')
    treeDoc=ROOT.TPaveText(.05,.1,.95,.8)
    treeDoc.SetName("README")
    
    mytime = MyEventTime()
    
    maxnhits = 8000

    cerenkovFlag  = array( 'i', [0] )
    nHits = array( 'i', [0] )
    plan = array( 'i', maxnhits*[0] )
    CellI = array( 'i', maxnhits*[0] )
    CellJ = array( 'i', maxnhits*[0] )
    DIFnum = array( 'i', maxnhits*[0] )
    ASICnum = array( 'i', maxnhits*[0] )
    Channelnum = array( 'i', maxnhits*[0] )
    threshold = array( 'i', maxnhits*[0] )
    timestamp = array( 'i', maxnhits*[0] )
    BCID_Frame = array( 'i', maxnhits*[0] )
    timeSinceResumeAcq_s = array( 'f', maxnhits*[0.] )
    timeSinceRunStart_s = array( 'f', maxnhits*[0.] )
    x = array( 'f', maxnhits*[0.] )
    y = array( 'f', maxnhits*[0.] )
    z = array( 'f', maxnhits*[0.] )

    t.Branch( 'EventTime', mytime, 'triggerCounter/i:BCID_DIF/I:AbsoluteBCID/l:EventClockStamp/I:BeamSpillNumber/i:clockCountInBeamSpill')
    t.Branch( 'cerenkovFlag', cerenkovFlag, 'cerenkov/I' )
    t.Branch( 'nHits', nHits, 'nHits/I' )
    t.Branch( 'layer', plan, 'layer[nHits]/I' )
    t.Branch( 'I', CellI, 'I[nHits]/I' )
    t.Branch( 'J', CellJ, 'J[nHits]/I' )
    t.Branch( 'DIF', DIFnum, 'DIF[nHits]/I' )
    t.Branch( 'ASIC', ASICnum, 'ASIC[nHits]/I' )
    t.Branch( 'CHANNEL', Channelnum, 'CHANNEL[nHits]/I' )
    t.Branch( 'threshold', threshold, 'threshold[nHits]/I' )
    t.Branch( 'timestamp', timestamp, 'timestamp[nHits]/I' )
    t.Branch( 'BCID_Frame', BCID_Frame, 'BCID_Frame[nHits]/I' )
    t.Branch( 'timeSinceResumeAcq_s', timeSinceResumeAcq_s, 'timeSinceResumeAcq_seconds[nHits]/F' )
    t.Branch( 'timeSinceRunStart_s', timeSinceRunStart_s, 'timeSinceRunStart_seconds[nHits]/F' )
    t.Branch( 'x', x, 'x[nHits]/F' )
    t.Branch( 'y', y, 'y[nHits]/F' )
    t.Branch( 'z', z, 'z[nHits]/F' )

    treeDoc.AddText("clock tick is 200 ns")
    treeDoc.AddText("triggerCounter=data readout number in the run, should correspond to ECAL spill number")
    treeDoc.AddText("BCID_DIF=data readout time in clock tick since acquisition has resumed")
    treeDoc.AddText("AbsoluteBCID=long counter (48 bits) in clock tick that is not reset during one run")
    treeDoc.AddText("AbsoluteBCID is reset at zero typically at the first start of acquisition after a power cycle or a configure")
    treeDoc.AddText("EventClockStamp=number of clock ticks separating the start of this event subset of hits and the BCID_DIF of the readout it comes from")
    treeDoc.AddText("BeamSpillNumber=SPS beam particle spill number")
    treeDoc.AddText("clockCountInBeamSpill=estimated number of clock ticks separating this event data and the start time of the SPS beam spill")
    treeDoc.AddText("cerenkovFlag=The BIF amplitude (Cernkov signal) for this event")
    treeDoc.AddText("nHits=total number of hits in this event")
    treeDoc.AddText("For the following tables, data for a hit are at the same array index")
    treeDoc.AddText("layer=plan or layer for each hit, although known as K")
    treeDoc.AddText("I and J=pad coordinate in the layer in pad size unit")
    treeDoc.AddText("DIF, ASIC and CHANNEL=electronic coordinates for the hit")
    treeDoc.AddText("threshold=the threshold crossed (1, 2 or 3)")
    treeDoc.AddText("timestamp=the time (in clock tick) as stored in the RawCalorimeterHit collection")
    treeDoc.AddText("BCID_Frame=the hit time (in clock tick) since acquisition has resumed. It equals BCID_DIF-timestamp")
    treeDoc.AddText("BCID_Frame is the time that should be used to match HCAL data and ECAL data")
    treeDoc.AddText("timeSinceResumeAcq_s=BCID_Frame*200e-9")
    treeDoc.AddText("timeSinceRunStart_s=(AbsoluteBCID-timestamp)*200e-9")
    treeDoc.AddText("x, y, z= hit coordinates")
    
    if guillaumeCutVar:
        myGCutVar = GuillaumeVariables()
        t.Branch( 'GuillaumeCutVariables', myGCutVar, 'MaxRamFullCount/I:NFullASIC:MaxNHitsInLayer:MaxNConsecutiveLayers:MaxNConsecutiveLayersWithOneHole')
    
    #lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    for ev in lcReader:            
        evparam=ev.parameters()
        mytime.readoutCounter=evparam.getIntVal('trigger')
        AbsBCIDup=evparam.getIntVal('bcid1')
        AbsBCIDlow=evparam.getIntVal('bcid2')
        absoluteBCID=((AbsBCIDup)<<24)+AbsBCIDlow
        mytime.AbsBCID=absoluteBCID
        if mode==Gerald:
            mytime.BCID=evparam.getIntVal('BCID')
            mytime.EventClockStamp=evparam.getIntVal('Sub_Readout_Frame_start_time_in_trigger')
        if mode==Guillaume or mode==Antoine:
            mytime.BCID=-1
            mytime.EventClockStamp=-1
            if mode==Guillaume:
                mytime.EventClockStamp=evparam.getIntVal('eventTimeInTrigger')
            cerenkovFlag[0]=evparam.getIntVal('cerenkovTag')

        if  mode==Guillaume or mode==Antoine:
            mytime.spillNumber=triggerSpillDict[mytime.readoutCounter][0]
            mytime.clockCountInSpill=(absoluteBCID-mytime.EventClockStamp)-triggerSpillDict[mytime.readoutCounter][1]
        if mode==Gerald:
            mytime.spillNumber=evparam.getIntVal('spillNumber')
            spilltime=(evparam.getIntVal('startSpill_BCID_up')<<24)+evparam.getIntVal('startSpill_BCID_low')
            mytime.clockCountInSpill=(absoluteBCID-mytime.EventClockStamp)-spilltime
            
        lcCol=ev.getCollection("SDHCAL_HIT")
        if lcCol.getNumberOfElements()>=maxnhits:
            print "WARNING, skipping event {1}, collection has {0} hits".format(lcCol.getNumberOfElements(),ev.getEventNumber())
            continue
        if mode==Gerald:
            vecint=ROOT.vector('int')()
            lcCol.parameters().getIntVals('BIF_amplitudes',vecint)
            if vecint.empty():
                cerenkovFlag[0]=0
            else:
                cerenkovFlag[0]=vecint[0]

        if mode==Gerald:
            difID='DIF'
            asicID='ASIC'
            channelID='Channel'
        if mode==Guillaume or mode==Antoine:
            difID='Dif_id'
            asicID='Asic_id'
            channelID='Chan_id'
            
        if guillaumeCutVar:
            RamFullCount=dict()
            AsicFillCount=dict()
            LayerFillCount=dict()
        nHits[0]=lcCol.size()
        q=pyLCIO.UTIL.CellIDDecoder('EVENT::CalorimeterHit')(lcCol)
        ihit=0
        for hit in lcCol:
            if mode==Gerald:
                plan[ihit]=q(hit)['K'].value()
            else:
                plan[ihit]=q(hit)['K-1'].value()
            threshold[ihit]=int(hit.getEnergy())
            timestamp[ihit]=int(hit.getTime())
            
            timeSinceRunStart_s[ihit]=(absoluteBCID-timestamp[ihit])*2e-7
            BCID_Frame[ihit] = -1
            timeSinceResumeAcq_s[ihit]=-1
            if mode==Gerald:
                BCID_Frame[ihit]=mytime.BCID-timestamp[ihit]
                timeSinceResumeAcq_s[ihit]=(mytime.BCID-timestamp[ihit])*2e-7
            
            position=hit.getPosition()
            x[ihit]=position[0]
            y[ihit]=position[1]
            z[ihit]=position[2]

            DIF=q(hit)[difID].value()
            ASIC=q(hit)[asicID].value()
            CHANNEL=q(hit)[channelID].value()
            DIFnum[ihit]=DIF
            ASICnum[ihit]=ASIC
            Channelnum[ihit]=CHANNEL
            CellI[ihit]=q(hit)['I'].value()
            CellJ[ihit]=q(hit)['J'].value()
            
            if guillaumeCutVar:
                if CHANNEL in [29,31]:
                    RamFullCount[DIF]=RamFullCount.get(DIF,0)+1
                AsicFillCount[DIF*1000+ASIC]=AsicFillCount.get(DIF*1000+ASIC,0)+1
                LayerFillCount[plan[ihit]]=LayerFillCount.get(plan[ihit],0)+1
            
            ihit=ihit+1

        #now fill Guillaume's cut variables
        if guillaumeCutVar:
            myGCutVar.RamFullCount=0
            if len(RamFullCount):
                myGCutVar.RamFullCount=max(RamFullCount.itervalues())
            myGCutVar.MaxHitInPlan=0
            if len(LayerFillCount):
                myGCutVar.MaxHitInPlan=max(LayerFillCount.itervalues())
            AsicOccupancyList=[val for val in AsicFillCount.itervalues() if val>63]
            myGCutVar.NFullAsic=len(AsicOccupancyList)
            PlancOccupancyList=[val for val in LayerFillCount.iterkeys()]
            myGCutVar.MaxNConsecutiveLayers=1
            myGCutVar.MaxNConsecutiveLayersWithOneHole=1
            if len(PlancOccupancyList)>1:
                PlancOccupancyList.sort() #list plan number for plan with hits in order
                myGCutVar.MaxNConsecutiveLayers=computeMaxNumberOfConsecutivePlan(PlancOccupancyList)
                myGCutVar.MaxNConsecutiveLayersWithOneHole=computeMaxNumberOfConsecutivePlanWithOneHole(PlancOccupancyList)
        t.Fill() 

    treeDoc.Write()
    f.Write()
    f.Close()
    lcReader.close()
    
    
#import produce_root_tree_for_ECAL_HCAL
#produce_root_tree_for_ECAL_HCAL.makeTree("DHCAL_744193_I0_0_TriventSplit.slcio")
#produce_root_tree_for_ECAL_HCAL.makeTree("/data/software/binaries/SDHCAL/ggarillot/Trivent/script/TDHCAL.slcio")


def drawComparison(rootFileName1,rootFileName2,variable,condition="",first=1,labels=["GRPC_RawHit_Readout_Analyse","Trivent"]):
    if first==1:
        file1=ROOT.TFile(rootFileName1)
        file2=ROOT.TFile(rootFileName2)
        file2.SDHCAL.SetLineColor(2)
        file2.SDHCAL.SetMarkerColor(2)
        label1=labels[0]
        label2=labels[1]
    else:
        file2=ROOT.TFile(rootFileName1)
        file1=ROOT.TFile(rootFileName2)
        file1.SDHCAL.SetLineColor(2)
        file1.SDHCAL.SetMarkerColor(2)
        label1=labels[1]
        label2=labels[0]

    file1.SDHCAL.SetMarkerStyle(20)
    file2.SDHCAL.SetMarkerStyle(20)
    c=ROOT.TCanvas()
    file1.SDHCAL.Draw(variable,condition)
    file2.SDHCAL.Draw(variable,condition,"SAME")
    leg=c.BuildLegend();
    leg.GetListOfPrimitives()[0].SetLabel(label1)
    leg.GetListOfPrimitives()[1].SetLabel(label2)
    return c
