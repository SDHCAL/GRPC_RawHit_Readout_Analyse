// test program

#include <assert.h>
#include <fstream>




#include "GG_counter.h"
#include "GG_extended_counters.h"


void testArrayCounterLevelThree(ArrayCounter<DIFCounters>& level3counter)
{
  assert( level3counter.size()==2 );
  assert( level3counter.getCounter(0).sumcount()==3);
  assert( level3counter.getCounter(0).flagcount()==1);
  assert( level3counter.getCounter(0).size()==1 );
  assert( level3counter.getCounter(0).begin()->first==3 );
  assert( level3counter.getCounter(0).begin()->second.size()==1 );
  assert( level3counter.getCounter(0).begin()->second.sumcount()==3 );
  assert( level3counter.getCounter(0).begin()->second.flagcount()==1 );
  assert( level3counter.getCounter(0).begin()->second.begin()->first==4 );
  assert( level3counter.getCounter(0).begin()->second.begin()->second.size()==1 );
  assert( level3counter.getCounter(0).begin()->second.begin()->second.sumcount()==3 );
  assert( level3counter.getCounter(0).begin()->second.begin()->second.flagcount()==1 );
  assert( level3counter.getCounter(0).begin()->second.begin()->second.begin()->first==5);
  assert( level3counter.getCounter(0).begin()->second.begin()->second.begin()->second.sumcount() ==3);
  assert( level3counter.getCounter(0).begin()->second.begin()->second.begin()->second.flagcount() ==1);
  assert( level3counter.getCounter(1).sumcount()==0);
  assert( level3counter.getCounter(1).flagcount()==0);
  assert( level3counter.getCounter(1).size()==0 );
  
}


void testRunPlaneGap(RunThresholdCounter_PlaneGap &bob)
{
  assert ( bob.n_event==7866 );
  assert ( bob.getLabel(0)=="Threshold" );
  assert ( bob.getLabel(1)=="all_plans" );
  assert ( bob.getLabel(2)=="plan" );
  assert ( bob.getLabel(3)=="gap" );
  assert ( bob.m_thresholdCounters.size() == 3 );
  assert ( bob.m_thresholdCounters.getCounter(2).size()==1 );
  assert ( bob.m_thresholdCounters.getCounter(2).sumcount()==8 );
  assert ( bob.m_thresholdCounters.getCounter(2).flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(2).begin()->first==88 );
  assert ( bob.m_thresholdCounters.getCounter(2)[88].size() == 1 );
  assert ( bob.m_thresholdCounters.getCounter(2)[88].sumcount()==8 );
  assert ( bob.m_thresholdCounters.getCounter(2)[88].flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(2)[88].begin()->first==5 );
  assert ( bob.m_thresholdCounters.getCounter(2)[88][5].sumcount()==8 );
  assert ( bob.m_thresholdCounters.getCounter(2)[88][5].flagcount()==1 );

  assert ( bob.m_thresholdCounters.getCounter(1).size()==2 );
  assert ( bob.m_thresholdCounters.getCounter(1).sumcount()== 11 );
  assert ( bob.m_thresholdCounters.getCounter(1).flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(1)[4].sumcount()==3 );
  assert ( bob.m_thresholdCounters.getCounter(1)[4].flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(1)[4][5].sumcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(1)[4][5].flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(1)[4][6].sumcount()==2 );
  assert ( bob.m_thresholdCounters.getCounter(1)[4][6].flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(1)[88].sumcount()==8 );
  assert ( bob.m_thresholdCounters.getCounter(1)[88].flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(1)[88].begin()->first==5 );
  assert ( bob.m_thresholdCounters.getCounter(1)[88][5].sumcount()==8 );
  assert ( bob.m_thresholdCounters.getCounter(1)[88][5].flagcount()==1 );
  
  assert ( bob.m_thresholdCounters.getCounter(0).size()==2 );
  assert ( bob.m_thresholdCounters.getCounter(0).sumcount()== 14 );
  assert ( bob.m_thresholdCounters.getCounter(0).flagcount()==2 );
  assert ( bob.m_thresholdCounters.getCounter(0)[4].sumcount()==6 );
  assert ( bob.m_thresholdCounters.getCounter(0)[4].flagcount()==2 );
  assert ( bob.m_thresholdCounters.getCounter(0)[4][5].sumcount()==4 );
  assert ( bob.m_thresholdCounters.getCounter(0)[4][5].flagcount()==2 );
  assert ( bob.m_thresholdCounters.getCounter(0)[4][6].sumcount()==2 );
  assert ( bob.m_thresholdCounters.getCounter(0)[4][6].flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(0)[88].sumcount()==8 );
  assert ( bob.m_thresholdCounters.getCounter(0)[88].flagcount()==1 );
  assert ( bob.m_thresholdCounters.getCounter(0)[88].begin()->first==5 );
  assert ( bob.m_thresholdCounters.getCounter(0)[88][5].sumcount()==8 );
  assert ( bob.m_thresholdCounters.getCounter(0)[88][5].flagcount()==1 );
}

int main()
{


  SingleCounter countA;
  countA.add(1);   assert(countA.sumcount() == 1); assert(countA.flagcount()==1);
  countA.add(3);   assert(countA.sumcount() == 4); assert(countA.flagcount()==1);
  countA.newSet(); assert(countA.sumcount() == 4); assert(countA.flagcount()==1);
  countA.add(2);   assert(countA.sumcount() == 6); assert(countA.flagcount()==2);
  std::cout << "countA"; countA.write();
 
  std::ofstream outfile;
  outfile.open("test.txt");
  countA.ASCIIwrite(outfile);
  outfile.close();
  std::ifstream infile;
  infile.open("test.txt");
  SingleCounter countA_readback;
  countA_readback.ASCIIread(infile);
  infile.close();
  assert(countA.sumcount() == countA_readback.sumcount());
  assert(countA.flagcount() == countA_readback.flagcount());
  assert(countA == countA_readback);

  unsigned int valeur=1;
  unsigned int param[3];
  MappedCounters<SingleCounter> channelCount;
  valeur=1; param[2]=4; channelCount.add(valeur, param+2); channelCount.newSet();
  valeur=1; channelCount.add(valeur, param+2); param[2]=6; channelCount.add(valeur, param+2);
  assert(channelCount.sumcount()==3);
  assert(channelCount.flagcount()==2);
  assert(channelCount.size()==2);
  assert(channelCount.counterAtLevel(1,param+2).sumcount()==3);
  assert(channelCount.counterAtLevel(2,param+2).sumcount()==3);
  assert(channelCount.counterAtLevel(0,param+2).sumcount()==1);
  std::map<unsigned int,SingleCounter>::iterator ita=channelCount.begin();
  assert(ita->first==4);
  assert(ita->second.sumcount()==2);
  assert(ita->second.flagcount()==2);
  ++ita;
  assert(ita->first==6);
  assert(ita->second.sumcount()==1);
  assert(ita->second.flagcount()==1);
  std::string labels[2]={"all Channels", "channel"};
  channelCount.write(labels);

  outfile.open("test.txt");
  channelCount.ASCIIwrite(outfile);
  outfile.close();
  infile.open("test.txt");
  MappedCounters<SingleCounter>  channelCount_readback;
  channelCount_readback.ASCIIread(infile);
  infile.close();
  channelCount_readback.write(labels);
  assert(channelCount_readback.sumcount()==3);
  assert(channelCount_readback.flagcount()==2);
  assert(channelCount.sumcount() == channelCount_readback.sumcount());
  assert(channelCount.flagcount() == channelCount_readback.flagcount());
  assert(channelCount.size() == channelCount_readback.size());
  assert(channelCount == channelCount_readback);
  channelCount_readback[4].add(3);
  //channelCount_readback.write(labels);
  assert(channelCount != channelCount_readback);
  infile.open("test.txt"); channelCount_readback.ASCIIread(infile); infile.close();
  //channelCount_readback.write(labels);
  assert(channelCount == channelCount_readback);
  param[2]=7; channelCount_readback.add(valeur, param+2);
  channelCount_readback.write(labels);
  assert(channelCount != channelCount_readback);
  infile.open("test.txt"); channelCount_readback.ASCIIread(infile); infile.close();
  channelCount_readback.write(labels);
  assert(channelCount == channelCount_readback);
 
  MappedCounters<MappedCounters<SingleCounter> > asicchannelcount;
  valeur=1; param[2]=4; param[1]=1;  asicchannelcount.add(valeur, param+1); //[1][4] = (1/1)
  assert(asicchannelcount.begin()->first==1);
  assert(asicchannelcount[1].begin()->first==4);
  param[1]=2;  asicchannelcount.add(valeur, param+1);   //[1][4] = (1/1) [2][4] = (1/1)
  assert(asicchannelcount.begin()->first==1);
  assert(asicchannelcount[1].begin()->first==4);
  assert(asicchannelcount.size()==2);
  assert(asicchannelcount[2].begin()->first==4);
  asicchannelcount.newSet();
  valeur=3; param[2]=5; param[1]=1;  asicchannelcount.add(valeur, param+1); //[1][4] = (1/1) [1][5] = (3/1) [2][4] = (1/1)
  assert(asicchannelcount.size()==2);
  std::map<unsigned int,MappedCounters<SingleCounter> >::iterator itb=asicchannelcount.begin();
  assert(itb->first==1);
  assert(itb->second.size()==2);
  assert(itb->second[4].sumcount()==1);
  assert(itb->second[4].flagcount()==1);
  assert(itb->second[5].sumcount()==3);
  assert(itb->second[5].flagcount()==1);
  assert(itb->second.sumcount()==4);
  assert(itb->second.flagcount()==2);
  ++itb;
  assert(itb->first==2);
  assert(itb->second.size()==1);
  assert(itb->second[4].sumcount()==1);
  assert(itb->second[4].flagcount()==1);
  assert(itb->second.sumcount()==1);
  assert(itb->second.flagcount()==1);
  assert(asicchannelcount.sumcount()==5);
  assert(asicchannelcount.flagcount()==2);
  assert(asicchannelcount.counterAtLevel(2,param+1).sumcount()==5); //param[1]=1 et param[2]=5
  assert(asicchannelcount.counterAtLevel(3,param+1).sumcount()==5); 
  assert(asicchannelcount.counterAtLevel(1,param+1).sumcount()==4); 
  assert(asicchannelcount.counterAtLevel(0,param+1).sumcount()==3); 
  std::string asiclabels[3]={"all ASICs", "ASIC", "channel"};
  asicchannelcount.write(asiclabels);

  outfile.open("test.txt");
  asicchannelcount.ASCIIwrite(outfile);
  outfile.close();
  infile.open("test.txt");
  MappedCounters<MappedCounters<SingleCounter> > asicchannelcount_readback;
  asicchannelcount_readback.ASCIIread(infile);
  infile.close();
  asicchannelcount_readback.write(asiclabels);
  assert(asicchannelcount==asicchannelcount_readback);


  MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > difasichannelcount;
  valeur=1; param[2]=4; param[1]=1; param[0]=1; difasichannelcount.add(valeur, param);
  valeur=3; param[2]=4; param[1]=1; param[0]=2; difasichannelcount.add(valeur, param);
  difasichannelcount.newSet();
  valeur=1; param[2]=4; param[1]=1; param[0]=1; difasichannelcount.add(valeur, param);
  valeur=1; param[2]=4; param[1]=2; param[0]=1; difasichannelcount.add(valeur, param);
  difasichannelcount.newSet();
  valeur=1; param[2]=4; param[1]=1; param[0]=1; difasichannelcount.add(valeur, param);
  valeur=1; param[2]=5; param[1]=2; param[0]=1; difasichannelcount.add(valeur, param);
  std::string difasiclabels[4]={"all DIFs", "DIF", "ASIC", "channel"};
  difasichannelcount.write(difasiclabels);
  assert(difasichannelcount.counterAtLevel(3,param).sumcount()==8); //param[0]=1 param[1]=2 et param[2]=5
  assert(difasichannelcount.counterAtLevel(4,param).sumcount()==8); 
  assert(difasichannelcount.counterAtLevel(2,param).sumcount()==5); 
  assert(difasichannelcount.counterAtLevel(1,param).sumcount()==2); 
  assert(difasichannelcount.counterAtLevel(0,param).sumcount()==1); 
  

  outfile.open("test.txt");
  difasichannelcount.ASCIIwrite(outfile);
  outfile.close();
  infile.open("test.txt");
  MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > difasichannelcount_readback;
  difasichannelcount_readback.ASCIIread(infile);
  infile.close();
  difasichannelcount_readback.write(difasiclabels);
  assert(difasichannelcount==difasichannelcount_readback);
  param[2]=8; difasichannelcount_readback.add(valeur, param);
  assert(difasichannelcount!=difasichannelcount_readback);
  infile.open("test.txt"); difasichannelcount_readback.ASCIIread(infile); infile.close();
  assert(difasichannelcount==difasichannelcount_readback);


  ArrayCounter<SingleCounter> counterArray(5);
  ArrayUpToCounter<SingleCounter> counterArrayUpTo(5);
  assert(counterArray.size()==5);
  assert(counterArrayUpTo.size()==5);
  param[1]=3; param[2]=88; 
  counterArray.add(2,param+1);  //+2 pour [3]
  counterArray.newSet();
  counterArray.add(4,1,param+2); //+4 pour [1]
  counterArrayUpTo.add(2,param+1);  // +2 pour [0], [1],[2],[3] 
  counterArrayUpTo.newSet();
  counterArrayUpTo.add(4,1,param+2); // +4 pour [0] et [1] 
  std::string singleArrayLabel[2]={"indice", "array_element"};
  //counterArray.write(singleArrayLabel);
  //counterArrayUpTo.write(singleArrayLabel);
  unsigned int expectedFlagCount[5]={0,1,0,1,0};
  unsigned int expectedSumCount[5]={0,4,0,2,0};
  unsigned int expectedFlagCountUpTo[5]={2,2,1,1,0};
  unsigned int expectedSumCountUpTo[5]={6,6,2,2,0};
  for (unsigned int i=0; i<5; ++i)
    {
      assert(counterArray.getCounter(i).sumcount()==expectedSumCount[i]);
      assert(counterArray.getCounter(i).flagcount()==expectedFlagCount[i]);
      assert(counterArrayUpTo.getCounter(i).sumcount()==expectedSumCountUpTo[i]);
      assert(counterArrayUpTo.getCounter(i).flagcount()==expectedFlagCountUpTo[i]);
      assert(counterArray.getCounterAtLevel(i,0,NULL).sumcount()==expectedSumCount[i]);
      assert(counterArrayUpTo.getCounterAtLevel(i,0,NULL).sumcount()==expectedSumCountUpTo[i]);
   }

  ArrayCounter<DIFCounters> level3counter(2);
  assert (level3counter.size()==2);
  param[0]=3; param[1]=4; param[2]=5; //dif=3, asic=4, channel=5
  level3counter.add(3,0,param); //add data to first counter
  std::string difasiclabelsarray[5]={"setup", "all DIFs", "DIF", "ASIC", "channel"};
  level3counter.write(difasiclabelsarray);
  testArrayCounterLevelThree(level3counter);

  outfile.open("test.txt");
  level3counter.ASCIIwrite(outfile);
  outfile.close();
  infile.open("test.txt");
  ArrayCounter<DIFCounters> level3counter_readback(8);
  level3counter_readback.ASCIIread(infile);
  infile.close();
  testArrayCounterLevelThree(level3counter_readback);

  RunThresholdCounter_PlaneGap bob;
  bob.n_event=7866;
  assert (bob.m_thresholdCounters.size() == 3 );
  assert (bob.nLevels() == 4);
  bob.setLabel(0,"Threshold");
  bob.setLabel(1,"all plans"); //should be  converted to "all_plans"
  bob.setLabel(2,"plan");
  bob.setLabel(3,"gap");
  param[1]=4; param[2]=5; // plan=4, asic=5
  bob.m_thresholdCounters.add(3,0,param+1); // [0] set (3,1) to [4][5]
  bob.m_thresholdCounters.newSet();
  bob.m_thresholdCounters.add(1,1,param+1); // [0][4][5] = (4,2), [1][4][5]  =(1,1)
  param[1]=4; param[2]=6;
  bob.m_thresholdCounters.add(2,1,param+1); // [0][4][6] = (2,1), [1][4][6] = (2,1)
  param[1]=88; param[2]=5;
  bob.m_thresholdCounters.add(8,2,param+1); // [0][88][5] = (8,1), [1][88][5] = (8,1), [2][88][5] = (8,1)
  bob.write();

  testRunPlaneGap(bob);
  assert( bob.getCounterAtLevel(1,2,param+1).sumcount()==14 ); // param[1]=88 param[2]=5 access to [0]
  assert( bob.getCounterAtLevel(2,2,param+1).sumcount()==11 ); // access to [1]
  assert( bob.getCounterAtLevel(1,1,param+1).sumcount()==8 ); // access to [0][88]
  assert( bob.getCounterAtLevel(3,0,param+1).sumcount()==8 ); // access to [2][88][5]
  assert( bob.getCounterAtLevel(1,4).sumcount()==6 ); //access to [0][4]
  assert( bob.getCounterAtLevel(2,4,6).sumcount()==2 ); //access to [1][4][6]

  outfile.open("test.txt");
  bob.ASCIIwrite(outfile);
  outfile.close();
  infile.open("test.txt");
  RunThresholdCounter_DifAsicChannel failread;
  assert (failread.nLevels() == 5);
  assert (failread.ASCIIread(infile)==false);
  RunThresholdCounter_PlaneGap bob_readback;
  bob_readback.ASCIIread(infile);
  infile.close();
  bob_readback.write();
  testRunPlaneGap(bob_readback);

  return 0;
}
