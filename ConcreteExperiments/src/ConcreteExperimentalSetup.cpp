#include "ConcreteExperimentalSetup.h"
#include "ConfigInfo.h"


GIF_oct2016_ExperimentalSetup::GIF_oct2016_ExperimentalSetup()
{
  //Counting from source 
  addBIF(3);
  addOneDIFPadDevice(125);
  addOneDIFPadDevice(32);
  addOneDIFPadDevice(28);
  addOneDIFPadDevice(6);
  addCMSstrip(142,26); //verre
  addCMSstrip(23,2); //bakelite

  setConfig();
  setRunList();
  setGIFconditions();
  setRunQuality();
}

void  GIF_oct2016_ExperimentalSetup::setConfig()
{
  all_ConfigInfo &all=all_ConfigInfo::instance();
  Setup_ConfigInfo s;
  s.setConfig(3,ASIC_HR_ConfigInfo()); //BIF
  ASIC_HR_ConfigInfo padConfig(170,250,250,128);
  s.setConfig(6,padConfig);
  s.setConfig(28,padConfig);
  s.setConfig(32,padConfig);
  s.setConfig(125,padConfig);
  s.setConfig( 26,ASIC_HR_ConfigInfo(120,250,250));
  s.setConfig(142,ASIC_HR_ConfigInfo(120,250,250));
  s.setConfig( 23,ASIC_HR_ConfigInfo(110,250,250));
  s.setConfig(  2,ASIC_HR_ConfigInfo(110,250,250));
  all.addConfigInfo("GIFPP_STRIP_8",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140,250,250));
  s.setConfig(142,ASIC_HR_ConfigInfo(140,250,250));
  s.setConfig( 23,ASIC_HR_ConfigInfo(130,250,250));
  s.setConfig(  2,ASIC_HR_ConfigInfo(130,250,250));
  all.addConfigInfo("GIFPP_STRIP_9",s);
  all.addConfigInfo("GIFPP_STRIP_11",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140,106,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(140,106,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(130,106,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(130,106,106));
  all.addConfigInfo("GIFPP_STRIP_12",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140,218,180));
  s.setConfig(142,ASIC_HR_ConfigInfo(140,218,180));
  s.setConfig( 23,ASIC_HR_ConfigInfo(130,104,104));
  s.setConfig(  2,ASIC_HR_ConfigInfo(130,104,104));
  all.addConfigInfo("GIFPP_STRIP_13",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140,162,147));
  s.setConfig(142,ASIC_HR_ConfigInfo(140,162,147));
  s.setConfig( 23,ASIC_HR_ConfigInfo(130,110,103));
  s.setConfig(  2,ASIC_HR_ConfigInfo(130,110,103));
  all.addConfigInfo("GIFPP_STRIP_14",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140,114,114));
  s.setConfig(142,ASIC_HR_ConfigInfo(140,114,114));
  s.setConfig( 23,ASIC_HR_ConfigInfo(130,114,101));
  s.setConfig(  2,ASIC_HR_ConfigInfo(130,114,101));
  all.addConfigInfo("GIFPP_STRIP_15",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140,114,101));
  s.setConfig(142,ASIC_HR_ConfigInfo(140,114,101));
  all.addConfigInfo("GIFPP_STRIP_16",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140,181,115));
  s.setConfig(142,ASIC_HR_ConfigInfo(140,181,115));
  s.setConfig( 23,ASIC_HR_ConfigInfo(130,181,115));
  s.setConfig(  2,ASIC_HR_ConfigInfo(130,181,115));
  all.addConfigInfo("GIFPP_STRIP_17",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140, 98,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(140, 98,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(130, 98,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(130, 98,106));
  all.addConfigInfo("GIFPP_STRIP_18",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(140,138,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(140,138,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(130,138,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(130,138,106));
  all.addConfigInfo("GIFPP_STRIP_19",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(130,106,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(130,106,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(120,106,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(120,106,106));
  all.addConfigInfo("GIFPP_STRIP_20",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(120,106,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(120,106,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(110,106,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(110,106,106));
  all.addConfigInfo("GIFPP_STRIP_21",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(110,106,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(110,106,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(100,106,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(100,106,106));
  all.addConfigInfo("GIFPP_STRIP_22",s);
  s.setConfig( 26,ASIC_HR_ConfigInfo(100,106,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(100,106,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo( 90,106,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo( 90,106,106));
  all.addConfigInfo("GIFPP_STRIP_23",s);
  padConfig=ASIC_HR_ConfigInfo(150,250,250,128);
  s.setConfig(6,padConfig);
  s.setConfig(28,padConfig);
  s.setConfig(32,padConfig);
  s.setConfig(125,padConfig);
  s.setConfig( 26,ASIC_HR_ConfigInfo(110,106,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(110,106,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(110,106,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(110,106,106));
  all.addConfigInfo("GIFPP_STRIP_26",s);
  padConfig=ASIC_HR_ConfigInfo(160,250,250,128);
  s.setConfig(6,padConfig);
  s.setConfig(28,padConfig);
  s.setConfig(32,padConfig);
  s.setConfig(125,padConfig);
  s.setConfig( 26,ASIC_HR_ConfigInfo(100,106,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(100,106,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(100,106,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(100,106,106));
  all.addConfigInfo("GIFPP_STRIP_27",s);
  padConfig=ASIC_HR_ConfigInfo(170,250,250,128);
  s.setConfig(6,padConfig);
  s.setConfig(28,padConfig);
  s.setConfig(32,padConfig);
  s.setConfig(125,padConfig);
  s.setConfig( 26,ASIC_HR_ConfigInfo(200,106,106));
  s.setConfig(142,ASIC_HR_ConfigInfo(200,106,106));
  s.setConfig( 23,ASIC_HR_ConfigInfo(200,106,106));
  s.setConfig(  2,ASIC_HR_ConfigInfo(200,106,106));
  all.addConfigInfo("GIFPP_STRIP_28",s);
 }

// From SourceDir
// cd analysisResults/; tar xzvf GIF_october2016.tgz
// here `ls GIF_october2016` allows to get the list of run numbers
// runs without config info in elog
// for run in `ls GIF_october2016`; do a=`cat ../ConcreteExperiments/ElogExtract/GIF_October2016/*.csv | grep $run -c`; echo $run,$a | awk -F, -v RUN=$run '{if ($2==0) print "  all.addRun("RUN");"}'; done
// other runs from elog exports
//for run in  `ls GIF_october2016`;do grep $run ../ConcreteExperiments/ElogExtract/GIF_October2016/GIF_elog_export.csv | awk -F\; -v RUN=$run '{print "  all.addRun("RUN","$6");"}'; done 
//for run in  `ls GIF_october2016`;do grep $run ../ConcreteExperiments/ElogExtract/GIF_October2016/M3_elog_export.csv | awk -F\; -v RUN=$run '{print "  all.addRun("RUN","$4");"}'; done
// assuming all above commands have been redirecter in file tut then :
// sort -n -t, -k2 tut 
void  GIF_oct2016_ExperimentalSetup::setRunList()
{
  all_ConfigInfo &all=all_ConfigInfo::instance();
  all.addRun(733517);
  all.addRun(733518,"GIFPP_OYONAX_8");
  all.addRun(733519,"GIFPP_OYONAX_8");
  all.addRun(733520,"GIFPP_OYONAX_8");
  all.addRun(733521,"GIFPP_OYONAX_8");
  all.addRun(733522,"GIFPP_OYONAX_8");
  all.addRun(733523,"GIFPP_OYONAX_8");
  all.addRun(733524,"GIFPP_OYONAX_8");
  all.addRun(733525,"GIFPP_OYONAX_8");
  all.addRun(733526,"GIFPP_OYONAX_8");
  all.addRun(733527,"GIFPP_OYONAX_8");
  all.addRun(733528,"GIFPP_OYONAX_8");
  all.addRun(733529,"GIFPP_OYONAX_8");
  all.addRun(733530,"GIFPP_OYONAX_8");
  all.addRun(733531,"GIFPP_OYONAX_8");
  all.addRun(733532,"GIFPP_OYONAX_8");
  all.addRun(733535,"GIFPP_OYONAX_8");
  all.addRun(733536,"GIFPP_OYONAX_8");
  all.addRun(733537,"GIFPP_OYONAX_8");
  all.addRun(733538,"GIFPP_OYONAX_8");
  all.addRun(733539);
  all.addRun(733541,"GIFPP_OYONAX_8");
  all.addRun(733542,"GIFPP_OYONAX_8");
  all.addRun(733543,"GIFPP_OYONAX_8");
  all.addRun(733545,"GIFPP_OYONAX_8");
  all.addRun(733546,"GIFPP_OYONAX_8");
  all.addRun(733547,"GIFPP_OYONAX_8");
  all.addRun(733548,"GIFPP_OYONAX_8");
  all.addRun(733549);
  all.addRun(733551,"GIFPP_OYONAX_8");
  all.addRun(733552,"GIFPP_OYONAX_8");
  all.addRun(733553,"GIFPP_OYONAX_8");
  all.addRun(733554,"GIFPP_OYONAX_8");
  all.addRun(733555);
  all.addRun(733556);
  all.addRun(733557);
  all.addRun(733558);
  all.addRun(733559);
  all.addRun(733560);
  all.addRun(733561);
  all.addRun(733562);
  all.addRun(733563);
  all.addRun(733564);
  all.addRun(733565);
  all.addRun(733567);
  all.addRun(733568);
  all.addRun(733569);
  all.addRun(733570);
  all.addRun(733571);
  all.addRun(733572);
  all.addRun(733573);
  all.addRun(733574);
  all.addRun(733575);
  all.addRun(733576);
  all.addRun(733577);
  all.addRun(733578);
  all.addRun(733579);
  all.addRun(733585,"GIFPP_OYONAX_12");
  all.addRun(733586,"GIFPP_OYONAX_12");
  all.addRun(733587,"GIFPP_OYONAX_12");
  all.addRun(733588,"GIFPP_OYONAX_12");
  all.addRun(733589,"GIFPP_OYONAX_12");
  all.addRun(733590,"GIFPP_OYONAX_12");
  all.addRun(733591,"GIFPP_OYONAX_12");
  all.addRun(733592,"GIFPP_OYONAX_12");
  all.addRun(733593);
  all.addRun(733594,"GIFPP_OYONAX_12");
  all.addRun(733595,"GIFPP_OYONAX_12");
  all.addRun(733596,"GIFPP_OYONAX_12");
  all.addRun(733597,"GIFPP_OYONAX_12");
  all.addRun(733599,"GIFPP_OYONAX_12");
  all.addRun(733600,"GIFPP_OYONAX_12");
  all.addRun(733601);
  all.addRun(733602);
  all.addRun(733603);
  all.addRun(733604);
  all.addRun(733605);
  all.addRun(733606);
  all.addRun(733607);
  all.addRun(733608);
  all.addRun(733609);
  all.addRun(733610);
  all.addRun(733611);
  all.addRun(733612);
  all.addRun(733613);
  all.addRun(733614,"GIFPP_STRIP_8");
  all.addRun(733616,"GIFPP_STRIP_8");
  all.addRun(733617,"GIFPP_STRIP_9");
  all.addRun(733618,"GIFPP_STRIP_9");
  all.addRun(733619);
  all.addRun(733620);
  all.addRun(733621);
  all.addRun(733622);
  all.addRun(733623);
  all.addRun(733624);
  all.addRun(733625);
  all.addRun(733627);
  all.addRun(733629);
  all.addRun(733630);
  all.addRun(733634);
  all.addRun(733635,"GIFPP_STRIP_9");
  all.addRun(733638,"GIFPP_STRIP_11");
  all.addRun(733639,"GIFPP_STRIP_11");
  all.addRun(733640,"GIFPP_STRIP_11");
  all.addRun(733648,"GIFPP_STRIP_11");
  all.addRun(733649,"GIFPP_STRIP_11");
  all.addRun(733651,"GIFPP_STRIP_11");
  all.addRun(733652,"GIFPP_STRIP_11");
  all.addRun(733657,"GIFPP_STRIP_11");
  all.addRun(733661,"GIFPP_STRIP_12");
  all.addRun(733662,"GIFPP_STRIP_13");
  all.addRun(733663,"GIFPP_STRIP_13");
  all.addRun(733664,"GIFPP_STRIP_13");
  all.addRun(733666,"GIFPP_STRIP_14");
  all.addRun(733667,"GIFPP_STRIP_15");
  all.addRun(733668,"GIFPP_STRIP_16");
  all.addRun(733669,"GIFPP_STRIP_17");
  all.addRun(733670,"GIFPP_STRIP_17");
  all.addRun(733671,"GIFPP_STRIP_18");
  all.addRun(733672,"GIFPP_STRIP_19");
  all.addRun(733673,"GIFPP_STRIP_20");
  all.addRun(733674,"GIFPP_STRIP_21");
  all.addRun(733676,"GIFPP_STRIP_12");
  all.addRun(733677,"GIFPP_STRIP_12");
  all.addRun(733681,"GIFPP_STRIP_12");
  all.addRun(733682,"GIFPP_STRIP_12");
  all.addRun(733684,"GIFPP_STRIP_12");
  all.addRun(733685,"GIFPP_STRIP_21");
  all.addRun(733687,"GIFPP_STRIP_26");
  all.addRun(733690,"GIFPP_STRIP_26");
  all.addRun(733691,"GIFPP_STRIP_22");
  all.addRun(733694,"GIFPP_STRIP_22");
  all.addRun(733695,"GIFPP_STRIP_22");
  all.addRun(733697,"GIFPP_STRIP_23");
  all.addRun(733703,"GIFPP_STRIP_23");
  all.addRun(733704,"GIFPP_STRIP_12");
  all.addRun(733706,"GIFPP_STRIP_26");
  all.addRun(733709,"GIFPP_STRIP_26");
  all.addRun(733712,"GIFPP_STRIP_27");
  all.addRun(733713,"GIFPP_STRIP_22");
  all.addRun(733714,"GIFPP_STRIP_22");
  all.addRun(733715,"GIFPP_STRIP_12");
  all.addRun(733716,"GIFPP_STRIP_28");
  all.addRun(733717,"GIFPP_STRIP_22");
  all.addRun(733721,"GIFPP_STRIP_22");
  all.addRun(733726,"GIFPP_STRIP_22");
  all.addRun(733727,"GIFPP_STRIP_22");
  all.addRun(733729,"GIFPP_STRIP_22");
  all.addRun(733730,"GIFPP_STRIP_22");
  all.addRun(733731,"GIFPP_STRIP_22");
  all.addRun(733732,"GIFPP_STRIP_22");
  all.addRun(733733,"GIFPP_STRIP_22");
  all.addRun(733734,"GIFPP_STRIP_22");
  all.addRun(733735,"GIFPP_STRIP_22");
  all.addRun(733736,"GIFPP_STRIP_22");
  all.addRun(733737,"GIFPP_STRIP_22");
  all.addRun(733739,"GIFPP_STRIP_22");
  all.addRun(733744,"GIFPP_STRIP_22");
  all.addRun(733745,"GIFPP_STRIP_22");
  all.addRun(733746,"GIFPP_STRIP_22");
  all.addRun(733747,"GIFPP_STRIP_22");
  all.addRun(733749,"GIFPP_STRIP_22");
  all.addRun(733751,"GIFPP_STRIP_22");
  all.addRun(733752,"GIFPP_STRIP_22");
  all.addRun(733753,"GIFPP_STRIP_22");
  all.addRun(733755,"GIFPP_STRIP_22");
  all.addRun(733762);
  all.addRun(733771,"GIFPP_STRIP_22");
  all.addRun(733772,"GIFPP_STRIP_22");
  all.addRun(733773,"GIFPP_STRIP_22");
  all.addRun(733774,"GIFPP_STRIP_22");
  all.addRun(733775,"GIFPP_STRIP_22");
  all.addRun(733776,"GIFPP_STRIP_22");
  all.addRun(733777,"GIFPP_STRIP_22");
  all.addRun(733778,"GIFPP_STRIP_22");
  all.addRun(733779,"GIFPP_STRIP_22");
  all.addRun(733780,"GIFPP_STRIP_22");
  all.addRun(733781,"GIFPP_STRIP_22");
  all.addRun(733782,"GIFPP_STRIP_22");
  all.addRun(733783);
  all.addRun(733784);
  all.addRun(733785);
  all.addRun(733786);
}

// From SourceDir
// cd analysisResults/; tar xzvf GIF_october2016.tgz
// here `ls GIF_october2016` allows to get the list of run numbers
// runs without config info in elog
// for run in `ls GIF_october2016`; do a=`cat ../ConcreteExperiments/ElogExtract/GIF_October2016/*.csv | grep $run -c`; echo $run,$a | awk -F, -v RUN=$run '{if ($2==0) print "  all.addRun("RUN",nPlans());"}'; done
//
// for run in M3 elog. 
// Warning using the fact that the OK field has never been set for these runs to avoid processing between "", "good data" and "bad data"
// for run in  `ls GIF_october2016`;do grep $run ../ConcreteExperiments/ElogExtract/GIF_October2016/M3_elog_export.csv | awk -F\; -v RUN=$run '{print "  all.addRun("RUN",nPlans(),RunQualityInfo::NOTSET);"}'; done
//
// for run in GIF elog
//
// for run in  `ls GIF_october2016`;do grep $run ../ConcreteExperiments/ElogExtract/GIF_October2016/GIF_elog_export.csv | awk -F\; -v RUN=$run '{s="RunQualityInfo::NOTSET"; if ($7=="\"Yes\"") s="RunQualityInfo::GOOD"; if ($7=="\"No\"") s="RunQualityInfo::BAD"; if (length($8)>0) s=s "," $8; print "  all.addRun("RUN",nPlans(),"s");"}'; done
//
// assuming all above commands have been redirecter in file tut then :
// sort -n -t, -k2 tut 


void GIF_oct2016_ExperimentalSetup::setRunQuality()
{
  all_ConfigInfo &all=all_ConfigInfo::instance();
  all.addRun(733517,nPlans());
  all.addRun(733518,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733519,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733520,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733521,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733522,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733523,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733524,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733525,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733526,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733527,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733528,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733529,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733530,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733531,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733532,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733535,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733536,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733537,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733538,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733539,nPlans());
  all.addRun(733541,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733542,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733543,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733545,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733546,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733547,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733548,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733549,nPlans());
  all.addRun(733551,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733552,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733553,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733554,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733555,nPlans());
  all.addRun(733556,nPlans());
  all.addRun(733557,nPlans());
  all.addRun(733558,nPlans());
  all.addRun(733559,nPlans());
  all.addRun(733560,nPlans());
  all.addRun(733561,nPlans());
  all.addRun(733562,nPlans());
  all.addRun(733563,nPlans());
  all.addRun(733564,nPlans());
  all.addRun(733565,nPlans());
  all.addRun(733567,nPlans());
  all.addRun(733568,nPlans());
  all.addRun(733569,nPlans());
  all.addRun(733570,nPlans());
  all.addRun(733571,nPlans());
  all.addRun(733572,nPlans());
  all.addRun(733573,nPlans());
  all.addRun(733574,nPlans());
  all.addRun(733575,nPlans());
  all.addRun(733576,nPlans());
  all.addRun(733577,nPlans());
  all.addRun(733578,nPlans());
  all.addRun(733579,nPlans());
  all.addRun(733585,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733586,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733587,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733588,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733589,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733590,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733591,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733592,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733593,nPlans());
  all.addRun(733594,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733595,nPlans(),RunQualityInfo::GOOD);
  all.addRun(733596,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733597,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733599,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733600,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733601,nPlans());
  all.addRun(733602,nPlans());
  all.addRun(733603,nPlans());
  all.addRun(733604,nPlans());
  all.addRun(733605,nPlans());
  all.addRun(733606,nPlans());
  all.addRun(733607,nPlans());
  all.addRun(733608,nPlans());
  all.addRun(733609,nPlans());
  all.addRun(733610,nPlans());
  all.addRun(733611,nPlans());
  all.addRun(733612,nPlans());
  all.addRun(733613,nPlans());
  all.addRun(733614,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733616,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733617,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733618,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733619,nPlans());
  all.addRun(733620,nPlans());
  all.addRun(733621,nPlans());
  all.addRun(733622,nPlans());
  all.addRun(733623,nPlans());
  all.addRun(733624,nPlans());
  all.addRun(733625,nPlans());
  all.addRun(733627,nPlans());
  all.addRun(733629,nPlans());
  all.addRun(733630,nPlans());
  all.addRun(733634,nPlans());
  all.addRun(733635,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733638,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733639,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733640,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733648,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733649,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733651,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733652,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733657,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733661,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733662,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733663,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733664,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733666,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733667,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733668,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733669,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733670,nPlans(),RunQualityInfo::BAD,"Test");
  all.addRun(733671,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733672,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733673,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733674,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733676,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733677,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733681,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733682,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733684,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733685,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733687,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733690,nPlans(),RunQualityInfo::BAD,"Strip chamber behavior changed");
  all.addRun(733691,nPlans(),RunQualityInfo::BAD,"Strip chamber behavior changed");
  all.addRun(733694,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733695,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733697,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733703,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733704,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733706,nPlans(),RunQualityInfo::BAD,"HV");
  all.addRun(733709,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733712,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733713,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733714,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733715,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733716,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733717,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733721,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733726,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733727,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733729,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733730,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733731,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733732,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733733,nPlans(),RunQualityInfo::BAD,"DAQ");
  all.addRun(733734,nPlans(),RunQualityInfo::BAD,"DAQ");
  all.addRun(733735,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733736,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733737,nPlans(),RunQualityInfo::BAD,"DAQ");
  all.addRun(733739,nPlans(),RunQualityInfo::BAD,"BIF trigger unplugged");
  all.addRun(733744,nPlans(),RunQualityInfo::BAD,"BIF trigger unplugged");
  all.addRun(733745,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733746,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733747,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733749,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733751,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733752,nPlans(),RunQualityInfo::BAD,"HV");
  all.addRun(733753,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733755,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733762,nPlans());
  all.addRun(733771,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733772,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733773,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733774,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733775,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733776,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733777,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733778,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733779,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733780,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733781,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733782,nPlans(),RunQualityInfo::NOTSET);
  all.addRun(733783,nPlans());
  all.addRun(733784,nPlans());
  all.addRun(733785,nPlans());
  all.addRun(733786,nPlans());

  //set correction for goodness
  try 
    { 
      //don't consider BIF unplugged as bad, just there should be no BIF trigger
      for (unsigned int numplan=0;numplan<nPlans();++numplan)
	{
	  all.changeRunQualityInfo(733739).setPlanGoodness(numplan,true);
	  all.changeRunQualityInfo(733744).setPlanGoodness(numplan,true);
	}
      //HV going to 5kV is a problem for strip chamber only
      for (unsigned int numplan=0;numplan<4;++numplan)
	{
	  all.changeRunQualityInfo(733690).setPlanGoodness(numplan,true);
	  all.changeRunQualityInfo(733691).setPlanGoodness(numplan,true);
	  all.changeRunQualityInfo(733706).setPlanGoodness(numplan,true);
	  all.changeRunQualityInfo(733752).setPlanGoodness(numplan,true);
	}
      //bakelite strip only going to 5kV during one run (manual intervantion by Salvadore)
      all.changeRunQualityInfo(733667).setPlanGoodness(5,false);
      //HV going to 5 kV deduced from analysis
      for (unsigned int numplan=4; numplan<6; ++numplan)
	{
	  all.changeRunQualityInfo(733703).setPlanGoodness(numplan,false);
	  all.changeRunQualityInfo(733704).setPlanGoodness(numplan,false);
	}

      // from text mention in elog
      all.changeRunQualityInfo(733729).set_Status(RunQualityInfo::BAD,"Test");

      //source status change during data taking (ON/OFF)
      all.changeRunQualityInfo(733755).set_Status(RunQualityInfo::BAD,"Source"); //from text in the elog
      all.changeRunQualityInfo(733717).set_Status(RunQualityInfo::BAD,"Source"); //from text in the elog and check in GIF source status elog
      all.changeRunQualityInfo(733721).set_Status(RunQualityInfo::BAD,"Source"); //from check in GIF source status elog
    }
  catch ( const RunNotFound_ConfigException& e) {}

}


// From SourceDir
// cd analysisResults/; tar xzvf GIF_october2016.tgz
// code generated with 
//  for run in  `ls GIF_october2016`;do grep $run ../ConcreteExperiments/ElogExtract/GIF_October2016/GIF_elog_export.csv | awk -F\; '{print "  gif=GIF_Conditions();"; if ($9=="\"ON\"") print "  gif.setBeamON();";  if ($9=="\"OFF\"") print "  gif.setBeamOFF();"; if ($14=="\"OFF\"") print "  gif.setSourceOFF();"; if ($14=="\"ON\"") print "  gif.setSourceON("substr($13,2,3)","substr($12,2,3)");"; if ($16!="\"Not Set\"") print "  gif.setScintillator("$16");"; print "  all.addRun("substr($5,2,length($5)-2)",gif);"; }'; done
//
void GIF_oct2016_ExperimentalSetup::setGIFconditions()
{
  all_ConfigInfo &all=all_ConfigInfo::instance();
  GIF_Conditions gif;
  gif=GIF_Conditions();
  all.addRun(733585,gif);
  gif=GIF_Conditions();
  all.addRun(733586,gif);
  gif=GIF_Conditions();
  all.addRun(733587,gif);
  gif=GIF_Conditions();
  all.addRun(733588,gif);
  gif=GIF_Conditions();
  gif.setSourceOFF();
  all.addRun(733589,gif);
  gif=GIF_Conditions();
  gif.setSourceOFF();
  all.addRun(733590,gif);
  gif=GIF_Conditions();
  all.addRun(733591,gif);
  gif=GIF_Conditions();
  all.addRun(733592,gif);
  gif=GIF_Conditions();
  all.addRun(733594,gif);
  gif=GIF_Conditions();
  gif.setSourceON(333,113);
  gif.setScintillator("None");
  all.addRun(733595,gif);
  gif=GIF_Conditions();
  gif.setSourceON(333,113);
  gif.setScintillator("None");
  all.addRun(733596,gif);
  gif=GIF_Conditions();
  gif.setSourceOFF();
  all.addRun(733597,gif);
  gif=GIF_Conditions();
  all.addRun(733599,gif);
  gif=GIF_Conditions();
  all.addRun(733600,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,112);
  gif.setScintillator("Both");
  all.addRun(733614,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,213);
  gif.setScintillator("Ours");
  all.addRun(733616,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733617,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(212,211);
  gif.setScintillator("Ours");
  all.addRun(733618,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733635,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733638,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733639,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733640,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733648,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733649,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733651,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(312,121);
  gif.setScintillator("Ours");
  all.addRun(733652,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(221,333);
  gif.setScintillator("Ours");
  all.addRun(733657,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(221,333);
  gif.setScintillator("Ours");
  all.addRun(733661,gif);
  gif=GIF_Conditions();
  gif.setSourceON(221,333);
  gif.setScintillator("Ours");
  all.addRun(733662,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(221,333);
  gif.setScintillator("Ours");
  all.addRun(733663,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733664,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733666,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733667,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733668,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733669,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733670,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733671,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733672,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733673,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733674,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733676,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733677,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,213);
  gif.setScintillator("Ours");
  all.addRun(733681,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(221,213);
  gif.setScintillator("Ours");
  all.addRun(733682,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(221,212);
  gif.setScintillator("Ours");
  all.addRun(733684,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(211,212);
  gif.setScintillator("Ours");
  all.addRun(733685,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(333,211);
  gif.setScintillator("Ours");
  all.addRun(733687,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733690,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733691,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733694,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733695,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733697,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733703,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733704,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733706,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733709,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733712,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733713,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,113);
  gif.setScintillator("Ours");
  all.addRun(733714,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,113);
  gif.setScintillator("Ours");
  all.addRun(733715,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,113);
  gif.setScintillator("Ours");
  all.addRun(733716,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,113);
  gif.setScintillator("Ours");
  all.addRun(733717,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(122,113);
  gif.setScintillator("Ours");
  all.addRun(733721,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,113);
  gif.setScintillator("Ours");
  all.addRun(733726,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,113);
  gif.setScintillator("Ours");
  all.addRun(733727,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733729,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733730,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733731,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733732,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733733,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733734,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733735,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733736,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733737,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733739,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,122);
  gif.setScintillator("Ours");
  all.addRun(733744,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733745,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(311,122);
  gif.setScintillator("Ours");
  all.addRun(733746,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,122);
  gif.setScintillator("Ours");
  all.addRun(733747,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(222,122);
  gif.setScintillator("Ours");
  all.addRun(733749,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceOFF();
  gif.setScintillator("Ours");
  all.addRun(733751,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,113);
  gif.setScintillator("Ours");
  all.addRun(733752,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(213,113);
  gif.setScintillator("Ours");
  all.addRun(733753,gif);
  gif=GIF_Conditions();
  gif.setBeamON();
  gif.setSourceON(222,211);
  gif.setScintillator("Ours");
  all.addRun(733755,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceON(211,122);
  gif.setScintillator("Ours");
  all.addRun(733771,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceON(211,113);
  gif.setScintillator("Ours");
  all.addRun(733772,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceON(211,211);
  gif.setScintillator("Ours");
  all.addRun(733773,gif);
  gif=GIF_Conditions();
  gif.setBeamOFF();
  gif.setSourceON(211,212);
  gif.setScintillator("Ours");
  all.addRun(733774,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733775,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733776,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733777,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733778,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733779,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733780,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733781,gif);
  gif=GIF_Conditions();
  gif.setScintillator("Ours");
  all.addRun(733782,gif);

  //config info added by hand
  for (unsigned int run=733651; run<=733782; ++run)
    {
      if (run==733737 || run==733739 || run==733744) continue;
      try { all.changeGIFconditions(run).setYuris_small_scintillator_in_BIF(); }
      catch ( const RunNotFound_ConfigException& e) {}
    }
}



CERN_SPS_Sept2018_SDHCAL_ExperimentalSetup::CERN_SPS_Sept2018_SDHCAL_ExperimentalSetup()
{
  //counting from beam upstream
  addBIF(3);
  float padSize=10.408f;
  float zShift=226.5f;
  Plane_position SDHCALPos({-padSize/2,96*padSize+padSize/2,zShift},{0,-1,0},{1,0,0});
  float SDHCAL_layer_width=28.0f;//26.131f;
  /*                                      */ addSDHCALPlan( 30, 94,181, SDHCALPos); //slot  0
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(100, 97, 92, SDHCALPos); //slot  1
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 40, 93, 98, SDHCALPos); //slot  2
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(119,118,129, SDHCALPos); //slot  3
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(151,152,164, SDHCALPos); //slot  4
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 75, 61, 74, SDHCALPos); //slot  5
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(110,111,156, SDHCALPos); //slot  6
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(103,177,102, SDHCALPos); //slot  7
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(176,175,174, SDHCALPos); //slot  8
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(121,120,128, SDHCALPos); //slot  9
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 58, 64, 65, SDHCALPos); //slot 10
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 73, 72,148, SDHCALPos); //slot 11
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 60, 79, 78, SDHCALPos); //slot 12
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(113, 43, 44, SDHCALPos); //slot 13
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(243,242,241, SDHCALPos); //slot 14
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(186,127,154, SDHCALPos); //slot 15
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 71, 70,147, SDHCALPos); //slot 16
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(139,140, 47, SDHCALPos); //slot 17
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 76, 77,143, SDHCALPos); //slot 18
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 36, 91,159, SDHCALPos); //slot 19
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(183,178,179, SDHCALPos); //slot 20
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 67, 42, 41, SDHCALPos); //slot 21
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(138, 46,137, SDHCALPos); //slot 22
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(144,173,131, SDHCALPos); //slot 23
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(160,184,189, SDHCALPos); //slot 24
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(171,167,172, SDHCALPos); //slot 25
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(145,135,146, SDHCALPos); //slot 26
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(180,170,185, SDHCALPos); //slot 27
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(190,188,187, SDHCALPos); //slot 28
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(166,165,169, SDHCALPos); //slot 29
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 50, 57,155, SDHCALPos); //slot 30
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(153,108, 25, SDHCALPos); //slot 31
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(109, 56, 51, SDHCALPos); //slot 32
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(107,150,116, SDHCALPos); //slot 33
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 49,124,126, SDHCALPos); //slot 34
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(115,149,117, SDHCALPos); //slot 35
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(114, 45, 48, SDHCALPos); //slot 36

  //5 tricots
  //FIXME, don't know exactly the positions, and what should be the direction ?
  Plane_position tricotPos;
  tricotPos.setPosition({0,0,zShift+50*SDHCAL_layer_width+100});
  float tricot_layer_width=25.0f;
  /*                                       */ addTricot(80,tricotPos);
  tricotPos.translate(2, tricot_layer_width); addTricot(182,tricotPos);
  tricotPos.translate(2, tricot_layer_width); addTricot(105,tricotPos);
  tricotPos.translate(2, tricot_layer_width); addTricot(63,tricotPos);
  tricotPos.translate(2, tricot_layer_width); addTricot(87,tricotPos);

  //AEGIS single PAD
  //FIXME don't know exactly the positions.
  Plane_position aegisPos=tricotPos;
  aegisPos.translate(2, 150.0f);
  float aegis_layer_width=25.0f;
  /*                                     */ addOneDIFPadDevice(18,aegisPos);
  aegisPos.translate(2, aegis_layer_width); addOneDIFPadDevice(5,aegisPos);
  aegisPos.translate(2, aegis_layer_width); addOneDIFPadDevice(8,aegisPos);

  setRunList();
  setRunQuality();
}


CERN_SPS_H2_Sept2022_part1_SDHCAL_ExperimentalSetup::CERN_SPS_H2_Sept2022_part1_SDHCAL_ExperimentalSetup()
{
  //counting from beam upstream
  addBIF(252); //There is a second BIF unpluggged number 251
  float padSize=10.408f;
  float zShift=226.5f;
  Plane_position SDHCALPos({-padSize/2,96*padSize+padSize/2,zShift},{0,-1,0},{1,0,0});
  float SDHCAL_layer_width=28.0f;//26.131f;
  /*                                      */ addSDHCALPlan( 30, 94,181, SDHCALPos); //slot  0
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(100, 97, 92, SDHCALPos); //slot  1
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 40, 93, 98, SDHCALPos); //slot  2
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(119,118,129, SDHCALPos); //slot  3
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(151,152,164, SDHCALPos); //slot  4
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 75, 61, 74, SDHCALPos); //slot  5
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(110,111,156, SDHCALPos); //slot  6
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(103,177,102, SDHCALPos); //slot  7
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(176,175,174, SDHCALPos); //slot  8
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(121,120,128, SDHCALPos); //slot  9
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 58, 64, 65, SDHCALPos); //slot 10
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 73, 72,148, SDHCALPos); //slot 11
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 60, 79, 78, SDHCALPos); //slot 12
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(113, 43, 44, SDHCALPos); //slot 13
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(243,242,241, SDHCALPos); //slot 14
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(186,127,154, SDHCALPos); //slot 15
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 71, 70,147, SDHCALPos); //slot 16
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(139,140, 47, SDHCALPos); //slot 17
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 76, 77,143, SDHCALPos); //slot 18
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 36, 91,159, SDHCALPos); //slot 19
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(183,178,179, SDHCALPos); //slot 20
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 67, 42, 41, SDHCALPos); //slot 21
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(138, 46,137, SDHCALPos); //slot 22
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(144,173,131, SDHCALPos); //slot 23
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(160,184,189, SDHCALPos); //slot 24
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(171,167,172, SDHCALPos); //slot 25
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(145,135,146, SDHCALPos); //slot 26
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(180,170,185, SDHCALPos); //slot 27
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(190,188,187, SDHCALPos); //slot 28
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(166,165,169, SDHCALPos); //slot 29
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 50, 57,155, SDHCALPos); //slot 30
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(153,108, 25, SDHCALPos); //slot 31
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(109, 56, 51, SDHCALPos); //slot 32
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(107,150,116, SDHCALPos); //slot 33
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan( 49,124,126, SDHCALPos); //slot 34
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(115,149,117, SDHCALPos); //slot 35
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(114, 45, 48, SDHCALPos); //slot 36
  SDHCALPos.translate(2,SDHCAL_layer_width); addSDHCALPlan(133, 68, 90, SDHCALPos); //slot 37
}
