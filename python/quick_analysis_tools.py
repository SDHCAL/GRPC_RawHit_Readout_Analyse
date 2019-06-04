

import pyLCIO
import ROOT
ROOT.gROOT.Reset()
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
#This generates warning about already loaded of some LCIO object (normal)
ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')
dummy=ROOT.intervalle('unsigned int')()

def getEventNumberWithNHitsGreaterThan(Nmin,lciofileName):
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    a=[]
    for ev in lcReader:
        lcCol=ev.getCollection("SDHCAL_HIT")
        if lcCol.size()>Nmin:
            a.append(ev.getEventNumber())
    lcReader.close()
    return a

def histoNhit(lciofileName):
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    a=ROOT.TH1F("Nhits","Nhits",1500,0,1500)
    for ev in lcReader:
        lcCol=ev.getCollection("SDHCAL_HIT")
        a.Fill(lcCol.size())
    lcReader.close()
    return a

def Graph3DNhit(lciofileName,numeroEvent,layerCode="K"):
    experience=ROOT.CERN_SPS_Sept2018_SDHCAL_ExperimentalSetup()
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    lcReader.skipNEvents(numeroEvent-1)
    ev=lcReader.readNextEvent()
    lcCol=ev.getCollection("SDHCAL_HIT")
    q=pyLCIO.UTIL.CellIDDecoder('EVENT::CalorimeterHit')(lcCol)
    a=[]
    a.append(ROOT.TCanvas())
    gr=ROOT.TGraph2D(lcCol.size())
    grTricot=ROOT.TGraph2D()
    indice=0
    indiceTricot=0
    for hit in lcCol:
        pos=hit.getPosition()
        gr.SetPoint(indice,pos[0],pos[2],pos[1])
        indice=indice+1
        numeroplan=q(hit)[layerCode].value()
        if experience.PlanIsTricot(numeroplan):
            grTricot.SetPoint(indiceTricot,pos[0],pos[2],pos[1])
            indiceTricot=indiceTricot+1
    gr.SetMarkerStyle(20)
    gr.SetMarkerSize(1)
    gr.GetXaxis().SetTitle("X")
    gr.GetYaxis().SetTitle("Z")
    gr.GetZaxis().SetTitle("Y")
    gr.Draw("P")
    a.append(gr)
    grTricot.SetMarkerStyle(20)
    grTricot.SetMarkerSize(1)
    grTricot.SetMarkerColor(2)
    grTricot.Draw("P SAME")
    a.append(grTricot)
    lcReader.close()
    return a

#import quick_analysis_tools as qat
#c=qat.Graph3DNhit("DHCAL_744193_I0_0_TriventSplit_all.slcio",86921)

def getEventNumberWithNTricotHits(lciofileName):
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    experience=ROOT.CERN_SPS_Sept2018_SDHCAL_ExperimentalSetup()
    a=dict()
    for ev in lcReader:
        lcCol=ev.getCollection("SDHCAL_HIT")
        q=pyLCIO.UTIL.CellIDDecoder('EVENT::CalorimeterHit')(lcCol)
        nhit=0
        for hit in lcCol:
            numeroplan=q(hit)['K'].value()
            if experience.PlanIsTricot(numeroplan):
                nhit=nhit+1
        if nhit>0:
            a[ev.getEventNumber()]=nhit
    lcReader.close()
    return a

#import quick_analysis_tools as qat
#qat.getEventNumberWithNTricotHits("DHCAL_744193_I0_0_TriventSplit_all.slcio")


def bcidup(lciofileName,index=4,shift=0):
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    a=set()
    for ev in lcReader:
        lcCol=ev.getCollection("DHCALRawHits")
        ke=ROOT.vector('string')()
        lcCol.parameters().getIntKeys(ke)
        for i in ke:
            if i.startswith("DIF") and i.endswith("Triggers"):
                t=ROOT.vector('int')()
                tr=lcCol.parameters().getIntVals(i,t)
                bcidupper=tr[index];
                a.add(bcidupper>>shift)
                break
    return a
#import quick_analysis_tools as qat
#d=qat.bcidup("DHCAL_744193_I0_0_TriventSplit_all.slcio")

def graphTime(lciofileName):
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    r=[]
    r.append(ROOT.TCanvas())
    a=dict()
    nbit=2
    for ev in lcReader:
        lcCol=ev.getCollection("DHCALRawHits")
        ke=ROOT.vector('string')()
        lcCol.parameters().getIntKeys(ke)
        for i in ke:
            if i.startswith("DIF") and i.endswith("Triggers"):
                t=ROOT.vector('int')()
                tr=lcCol.parameters().getIntVals(i,t)
                absbcid=(tr[4]<<24)+tr[3]
                break
        #print "absolute bcid is {0}.".format(absbcid)
        if absbcid % nbit == 0:
            nbit=nbit<<1
            print "{1} : absolute bcid is {0}.".format(absbcid,nbit)
        for hit in lcCol:
            #hitTime=hit.getCellID1()  #the frame bcid = time of the hit since start acq
            hitTime=absbcid-hit.getTimeStamp()
            a[hitTime]=a.get(hitTime,0)+1
    b=a.keys()
    b.sort()
    tmin=b[0]
    tmax=b[-1]
    print "time interval is [{0},{1}] clock tick".format(tmin,tmax)
    clockTick=200e-9
    tmin=tmin*clockTick
    tmax=tmax*clockTick
    print "time interval is [{0},{1}] second".format(tmin,tmax)
    if tmax>300:
        tmax=300
    if (tmin>0):
        tmin=0
    h=ROOT.TH1F("timeDisplay","event time",int((tmax-tmin)/1e-3),tmin,tmax)
    h.GetXaxis().SetTitle("time in second")
    for x,y in a.items():
        h.Fill(x*clockTick,y)
    h.Draw()
    r.append(h)
    r.append(a)
    print "tmin={0} et tmax={1}".format(tmin,tmax)
    return r
#import quick_analysis_tools as qat
#d=qat.graphTime("DHCAL_744193_I0_0_TriventSplit_all.slcio")
