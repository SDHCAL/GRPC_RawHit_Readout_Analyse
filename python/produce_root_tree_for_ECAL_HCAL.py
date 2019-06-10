#
#  event  :  BIF, readoutNumber, AbsoluteBCID, DIF BCID, event_time_intervalle
#
#  hit : x, y, z, threshold, timestamp, DIF, ASIC, CHANNEL
#
#

from array import array
import os.path
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
int     clockCountsFromPreviousEvent;\
int     spillNumber;\
int     clockCountInSpill;\
};" );

from ROOT import MyEventTime

def makeTree(lciofileName):

    Unset=0
    Guillaume=1
    Gerald=2
    mode=Unset

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

    t.Branch( 'EventTime', mytime, 'triggerCounter/I:BCID:AbsBCIDup:AbsBCIDlow:EventClockStamp:clockCountsFromPreviousEvent:spillNumber:clockCountInSpill')
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
    
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    first=True
    for ev in lcReader:
        if mode==Unset:
            ke=ROOT.vector('string')()
            ev.parameters().getIntKeys(ke)
            if 'Sub_Readout_Frame_start_time_in_trigger' in ke:

                mode=Gerald
            if 'eventTimeInTrigger' in ke:
                mode=Guillaume
            print " mode is set to value = {0}".format(mode)
            
        evparam=ev.parameters()
        mytime.readoutCounter=evparam.getIntVal('trigger')
        mytime.AbsBCIDup=evparam.getIntVal('bcid1')
        mytime.AbsBCIDlow=evparam.getIntVal('bcid2')
        if mode==Gerald:
            mytime.BCID=evparam.getIntVal('BCID')
            mytime.EventClockStamp=evparam.getIntVal('Sub_Readout_Frame_start_time_in_trigger')
        if mode==Guillaume:
            mytime.BCID=-1
            mytime.EventClockStamp=evparam.getIntVal('eventTimeInTrigger')
            cerenkovFlag[0]=evparam.getIntVal('cerenkovTag')
            
        if first:
            first=False
            #to implement
            mytime.spillNumber=-1
            mytime.clockCountsFromPreviousEvent=-1
            mytime.clockCountInSpill=-1 

        lcCol=ev.getCollection("SDHCAL_HIT")
        if mode==Gerald:
            vecint=ROOT.vector('int')()
            lcCol.parameters().getIntVals('BIF_amplitudes',vecint)
            if vecint.empty():
                cerenkovFlag[0]=0
            else:
                cerenkovFlag[0]=vecint[0]
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
            absoluteBCID=((mytime.AbsBCIDup)<<24)+mytime.AbsBCIDlow
            
            timeSinceRunStart_s[ihit]=(absoluteBCID-timestamp[ihit])*2e-7
            timeSinceResumeAcq_s[ihit]=-1
            if mode==Gerald:
                timeSinceResumeAcq_s[ihit]=(mytime.BCID-timestamp[ihit])*2e-7
            
            position=hit.getPosition()
            x[ihit]=position[0]
            y[ihit]=position[1]
            z[ihit]=position[2]
            ihit=ihit+1
        t.Fill() 
            
    f.Write()
    f.Close()

    
