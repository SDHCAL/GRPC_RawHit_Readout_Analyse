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
int     readoutCounter;\
int     BCID;\
int     AbsBCIDup;\
int     AbsBCIDlow;\
int     EventClockStamp;\
int     spillNumber;\
int     clockCountInSpill;\
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
    consecutiveSize=[len(list(group))+1 for key,group in itertools.groupby(pp) if key==1] #group pp by consecutive value, keep ajacent group for wich pp[i]==1, list the group size (NB : +1 because list of difference (pp) has element less than original list of plan number (PlancOccupancyList) )
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
    mode=Unset

    #determine mode
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    ev=lcReader.readNextEvent()
    ke=ROOT.vector('string')()
    ev.parameters().getIntKeys(ke)
    if 'Sub_Readout_Frame_start_time_in_trigger' in ke:
        mode=Gerald
    if 'eventTimeInTrigger' in ke:
        mode=Guillaume
    print " mode is set to value = {0}".format(mode)
    lcReader.close()

    if mode==Guillaume:
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
        print "Guillaume mode spill analysis finished"
    
    rootfileName=os.path.basename(lciofileName).split('.')[0]+'.root'
    f=ROOT.TFile(rootfileName,"recreate")
    t=ROOT.TTree('SDHCAL','SDHCAL events')

    mytime = MyEventTime()
    
    maxnhits = 8000

    cerenkovFlag  = array( 'i', [0] )
    nHits = array( 'i', [0] )
    plan = array( 'i', maxnhits*[0] )
    threshold = array( 'i', maxnhits*[0] )
    timestamp = array( 'i', maxnhits*[0] )
    timeSinceResumeAcq_s = array( 'f', maxnhits*[0.] )
    timeSinceRunStart_s = array( 'f', maxnhits*[0.] )
    x = array( 'f', maxnhits*[0.] )
    y = array( 'f', maxnhits*[0.] )
    z = array( 'f', maxnhits*[0.] )

    t.Branch( 'EventTime', mytime, 'triggerCounter/I:BCID:AbsBCIDup:AbsBCIDlow:EventClockStamp:spillNumber:clockCountInSpill')
    t.Branch( 'cerenkovFlag', cerenkovFlag, 'cerenkov/I' )
    t.Branch( 'nHits', nHits, 'nHits/I' )
    t.Branch( 'plan', plan, 'plan[nHits]/I' )
    t.Branch( 'threshold', threshold, 'threshold[nHits]/I' )
    t.Branch( 'timestamp', timestamp, 'timestamp[nHits]/I' )
    t.Branch( 'timeSinceResumeAcq_s', timeSinceResumeAcq_s, 'timeSinceResumeAcq_seconds[nHits]/F' )
    t.Branch( 'timeSinceRunStart_s', timeSinceRunStart_s, 'timeSinceRunStart_seconds[nHits]/F' )
    t.Branch( 'x', x, 'x[nHits]/F' )
    t.Branch( 'y', y, 'y[nHits]/F' )
    t.Branch( 'z', z, 'z[nHits]/F' )

    if guillaumeCutVar:
        myGCutVar = GuillaumeVariables()
        t.Branch( 'GuillaumeCutVariables', myGCutVar, 'MaxRamFullCount/I:NFullASIC:MaxNHitsInLayer:MaxNConsecutiveLayers:MaxNConsecutiveLayersWithOneHole')
    
    #lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    for ev in lcReader:            
        evparam=ev.parameters()
        mytime.readoutCounter=evparam.getIntVal('trigger')
        mytime.AbsBCIDup=evparam.getIntVal('bcid1')
        mytime.AbsBCIDlow=evparam.getIntVal('bcid2')
        absoluteBCID=((mytime.AbsBCIDup)<<24)+mytime.AbsBCIDlow
        if mode==Gerald:
            mytime.BCID=evparam.getIntVal('BCID')
            mytime.EventClockStamp=evparam.getIntVal('Sub_Readout_Frame_start_time_in_trigger')
        if mode==Guillaume:
            mytime.BCID=-1
            mytime.EventClockStamp=evparam.getIntVal('eventTimeInTrigger')
            cerenkovFlag[0]=evparam.getIntVal('cerenkovTag')

        if  mode==Guillaume:
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

        if guillaumeCutVar:
            if mode==Gerald:
                difID='DIF'
                asicID='ASIC'
                channelID='Channel'
            if mode==Guillaume:
                difID='Dif_id'
                asicID='Asic_id'
                channelID='Chan_id'
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
            timeSinceResumeAcq_s[ihit]=-1
            if mode==Gerald:
                timeSinceResumeAcq_s[ihit]=(mytime.BCID-timestamp[ihit])*2e-7
            
            position=hit.getPosition()
            x[ihit]=position[0]
            y[ihit]=position[1]
            z[ihit]=position[2]

            if guillaumeCutVar:
                DIF=q(hit)[difID].value()
                ASIC=q(hit)[asicID].value()
                CHANNEL=q(hit)[channelID].value()
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
            
    f.Write()
    f.Close()
    lcReader.close()
    
    
#import produce_root_tree_for_ECAL_HCAL
#produce_root_tree_for_ECAL_HCAL.makeTree("DHCAL_744193_I0_0_TriventSplit.slcio")
#produce_root_tree_for_ECAL_HCAL.makeTree("/data/software/binaries/SDHCAL/ggarillot/Trivent/script/TDHCAL.slcio")
