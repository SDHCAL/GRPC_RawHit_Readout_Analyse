import pyLCIO
import ROOT
ROOT.gROOT.Reset()
import sys
#ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser')
#This generates warning about already loaded of some LCIO object (normal)
#ROOT.gSystem.Load('libGRPC_RawHit_Readout_Analyser_dict')
#dummy=ROOT.intervalle('unsigned int')()

def display_event(lciofileName):
    #experience=ROOT.CERN_SPS_H2_Sept2022_part1_SDHCAL_ExperimentalSetup()
    lcReader=pyLCIO.IOIMPL.LCFactory.getInstance().createLCReader(pyLCIO.IO.LCReader.directAccess)
    lcReader.open( lciofileName )
    canvas=ROOT.TCanvas()
    for event in lcReader:
        lcCol=event.getCollection("SDHCAL_HIT")
        #q=pyLCIO.UTIL.CellIDDecoder('EVENT::CalorimeterHit')(lcCol)
        Nhits=lcCol.size()
        gr=ROOT.TGraph2D(Nhits)
        indice=0
        for hit in lcCol:
            pos=hit.getPosition()
            gr.SetPoint(indice,pos[0],pos[2],pos[1])
            indice=indice+1
        gr.SetMarkerStyle(21)
        gr.SetMarkerSize(1)
        gr.GetXaxis().SetTitle("X")
        gr.GetYaxis().SetTitle("Z")
        gr.GetZaxis().SetTitle("Y")
        gr.Draw("P")
        canvas.Update()
        #raw_input is python2
        a=raw_input("press s to stop, any other key to continue")
        if a=='s':
            break
        del gr
    lcReader.close()
    
if __name__ == '__main__':
    display_event(sys.argv[1])

