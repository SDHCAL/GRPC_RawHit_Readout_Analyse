

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
        lcCol=ev.getCollection("DHCAL_Hits")
        if lcCol.size()>Nmin:
            a.append(ev.getEventNumber())
    lcReader.close()
    return a

def histoNhit(lciofileName):
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    a=ROOT.TH1F("Nhits","Nhits",1500,0,1500)
    for ev in lcReader:
        lcCol=ev.getCollection("DHCAL_Hits")
        a.Fill(lcCol.size())
    lcReader.close()
    return a

def Graph3DNhit(lciofileName,numeroEvent):
    experience=ROOT.CERN_SPS_Sept2018_SDHCAL_ExperimentalSetup()
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    lcReader.skipNEvents(numeroEvent-1)
    ev=lcReader.readNextEvent()
    lcCol=ev.getCollection("DHCAL_Hits")
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
        numeroplan=q(hit)['K'].value()
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
        lcCol=ev.getCollection("DHCAL_Hits")
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
