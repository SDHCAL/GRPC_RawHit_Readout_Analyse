# GRPC_RawHit_Readout_Analyse


## Installation :
Database installation code works only on old architecture (ubuntu12,SLC6, ...)
The installation script for the database library is 
Modules/ILC_SDHCAL_ConfDB_installer.sh

For october 2016 GIF, the configuration setup has been included in the soft : you don't need to build against the database. So therecommended build is :

- mkdir build
- cd build 
- cmake ..
- make install

### python script
To run python script under the python directory, you need to add the lib/ directory to the  LD_LIBRARY_PATH environment variable

## scripts

### Related to GIF++ 2016 test beam
- basic_analysis_script_root.py : do a one run analysis from LCIO files with RawCalorimeterHits. Typically produce output that are in analysisResults/GIF_october2016.tgz
- produce_run_tree_GIF.py : produce a ROOT tree from results from extraction of analysisResults/GIF_october2016.tgz. 
- produce_october2016_GIF_plot.py : produce plots and slides from output of  produce_run_tree_GIF.py 


### Related to SDHCAL+SiWECAL september 2018 test beam
- run_trivent.py : perform the trivent operation (readout splitting based on time clustering and outputs LCIO CalorimeterHit collection)
- produce_root_tree_for_ECAL_HCAL.py : produce root tree from run_trivent.py


### interesting tag
- GIF_oct2016_reportslides_v1.0


## Usage example
### Producing ROOT tree from september 2018 test beam

From the GRPC_RawHit_Readout_Analyse directory :
```
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/lib
cd python
python run_trivent.py file1.slcio file2.slcio file3.slcio ...
```
This produces the following files :
- file1_TriventSplit.slcio  (if file1.slcio is not in the current directory, only the file1.slcio basename is used to name the output file)
- noisetrivent_check.root  : contains monitoring plots

Then in the python directory, starts the python interpreter and :
```
import produce_root_tree_for_ECAL_HCAL
produce_root_tree_for_ECAL_HCAL.makeTree("file1_TriventSplit.slcio")
```
This produces the file file1_TriventSplit.root

## License
Code licensed under the CeCILL 2.1 license :
- http://www.cecill.info/licences/Licence_CeCILL_V2.1-fr.txt
- http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

