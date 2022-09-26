import pyLCIO

#all studies takes a collection of LCIO calorimeter hits as argument

def doNothing(col):
    pass

def asic_study(col):
    q=pyLCIO.UTIL.CellIDDecoder('EVENT::CalorimeterHit')(col)
    asicStat={}
    asicFiredChannels={}
    for hit in col:
        key=(q(hit)["DIF"].value(),q(hit)["ASIC"].value())
        channel=q(hit)["Channel"].value()
        if key in asicStat:
            asicStat[key]+=1
        else:
            asicStat[key]=1
            asicFiredChannels[key]=set()
        asicFiredChannels[key].add(channel)
    highest_ASIC=sorted(asicStat.values())[-1:][0]
    two_highest_ASIC=sum(sorted(asicStat.values())[-2:])
    three_highest_ASIC=sum(sorted(asicStat.values())[-3:])
    sum_ASIC=sum(asicStat.values())
    print (highest_ASIC,sum_ASIC,highest_ASIC/sum_ASIC)
    print (two_highest_ASIC,sum_ASIC,two_highest_ASIC/sum_ASIC)
    print (three_highest_ASIC,sum_ASIC,three_highest_ASIC/sum_ASIC)
    occupancyList=sorted([len(v) for v in asicFiredChannels.values()])
    print (" distribution of number of fired channels for asics ", occupancyList)
    BigAsic=[n for n in asicStat.values() if n>80] 
    print ( "mean number of hits per fired ASIC and number of big ASIC", sum_ASIC/len(asicStat), len(BigAsic))
    if len(BigAsic) > 0 :
        print ( "hit/number big asic and hit in big asic/number big asic", sum_ASIC/len(BigAsic), sum(BigAsic)/len(BigAsic)  )
    
    

def plane_stat(col):
    q=pyLCIO.UTIL.CellIDDecoder('EVENT::CalorimeterHit')(col)
    planeStat={}
    for hit in col:
        key=q(hit)["K"].value()
        if key in planeStat:
            planeStat[key]+=1
        else:
            planeStat[key]=1
    #sort dict by keys
    sorted_plane_by_layer = {k: v for k, v in sorted(planeStat.items(), key=lambda item: item[0])}
    print ("plane content sorted by layer",sorted_plane_by_layer)
    #sort dict by values
    sorted_plane_by_hit_content = {k: v for k, v in sorted(planeStat.items(), key=lambda item: item[1])}
    print ("plane content sorted by increasing number of hits",sorted_plane_by_hit_content)
    
    
