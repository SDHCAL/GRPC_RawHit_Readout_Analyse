import pyLCIO
import ROOT
ROOT.gROOT.Reset()
import sys
#ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
#This generates warning about already loaded of some LCIO object (normal)
#ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')
#dummy=ROOT.intervalle('unsigned int')()

import Event_studies


def display_event(lciofileName,minNhits,maxNhits,theStudy):
    func=getattr(Event_studies,theStudy)
    #experience=ROOT.CERN_SPS_H2_Sept2022_part1_SDHCAL_ExperimentalSetup()
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    canvas=ROOT.TCanvas()
    h=ROOT.TH1F("Nhits","Nhits",2000,0,2000)
    for event in lcReader:
        lcCol=event.getCollection("SDHCAL_HIT")
        #q=pyLCIO.UTIL.CellIDDecoder('EVENT::CalorimeterHit')(lcCol)
        Nhits=lcCol.size()
        h.Fill(Nhits)  
        if Nhits<minNhits:
            continue
        if Nhits>maxNhits:
            continue
        gr=ROOT.TGraph2D(Nhits)
        indice=0
        grThresh={1:ROOT.TGraph2D(),2:ROOT.TGraph2D(),3:ROOT.TGraph2D()}
        for i,g in grThresh.items():
            g.SetMarkerStyle(21)
            g.SetMarkerSize(0.5)
            g.SetMarkerColor(5-i) # 1=blue, 2=green, 3=red
        for hit in lcCol:
            pos=hit.getPosition()
            gr.SetPoint(indice,pos[0],pos[2],pos[1])
            indice=indice+1
            threshkey=int(hit.getEnergy())&3
            grThresh[threshkey].AddPoint(pos[0],pos[2],pos[1])
        gr.SetMarkerStyle(21)
        gr.SetMarkerSize(0.5)
        gr.GetXaxis().SetTitle("X")
        gr.GetYaxis().SetTitle("Z")
        gr.GetZaxis().SetTitle("Y")
        gr.Draw("P")
        for g in grThresh.values():
            g.Draw("P SAME")
        canvas.Update()
        func(lcCol)
        #raw_input is python2
        #a=raw_input("press s to stop, any other key to continue")
        a=input("Number of hits = "+str(Nhits)+" press s to stop, any other key to continue")
        if a=='s':
            break
        del gr
        for g in grThresh.values():
            del g
    lcReader.close()
    h.Draw()
    canvas.Update()
    a=input("The hit number distribution seen so far, press any other key to quit")
    
if __name__ == '__main__':
    print("Usage : python3 event_diplay.py lcio_file_name min_number_of_hits max_number_of_hits <study>")
    print("study is an optionnal keyword corresponding to functions in Event_studies.py file")
    theStudy="doNothing"
    if len(sys.argv)==5:
        theStudy=(sys.argv[4])
    display_event(sys.argv[1],int(sys.argv[2]),int(sys.argv[3]),theStudy)

