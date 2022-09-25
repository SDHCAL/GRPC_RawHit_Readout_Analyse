import pyLCIO

#all studies takes a collection of LCIO calorimeter hits as argument

def doNothing(col):
    pass

def asic_study(col):
    q=pyLCIO.UTIL.CellIDDecoder('EVENT::CalorimeterHit')(col)
    asicStat={}
    for hit in col:
        key=(q(hit)["DIF"].value(),q(hit)["ASIC"].value())
        if key in asicStat:
            asicStat[key]+=1
        else:
            asicStat[key]=1
    highest_ASIC=sorted(asicStat.values())[-1:][0]
    two_highest_ASIC=sum(sorted(asicStat.values())[-2:])
    three_highest_ASIC=sum(sorted(asicStat.values())[-3:])
    sum_ASIC=sum(asicStat.values())
    print (highest_ASIC,sum_ASIC,highest_ASIC/sum_ASIC)
    print (two_highest_ASIC,sum_ASIC,two_highest_ASIC/sum_ASIC)
    print (three_highest_ASIC,sum_ASIC,three_highest_ASIC/sum_ASIC)
    


    
