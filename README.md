# GRPC_RawHit_Readout_Analyse

#Installation :
Database installation code works only on old architecture (ubuntu12,SLC6, ...)
The installation script for the database library is 
Modules/ILC_SDHCAL_ConfDB_installer.sh

For october 2016 GIF, the configuration setup has been included in the soft : you don't need to build against the database. So therecommended build is :

- mkdir build
- cd build 
- cmake -DBUILD_AGAINST_CONFDB=OFF ..
- make install

#python script
To run python script under the python directory, you need to add the lib/ directory to the  LD_LIBRARY_PATH environment variable

scripts
-------
- basic_analysis_script_root.py : do a one run analysis from LCIO files with RawCalorimeterHits. Typically produce output that are in analysisResults/GIF_october2016.tgz
- produce_run_tree_GIF.py : produce a ROOT tree from results from extraction of analysisResults/GIF_october2016.tgz. 
- produce_october2016_GIF_plot.py : produce plots and slides from output of  produce_run_tree_GIF.py 

#interesting tag
- GIF_oct2016_reportslides_v1.0


#License
Code licensed under the CeCILL 2.1 license :
- http://www.cecill.info/licences/Licence_CeCILL_V2.1-fr.txt
- http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

