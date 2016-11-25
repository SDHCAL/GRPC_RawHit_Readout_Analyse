#!/bin/bash

targetDirectory=$1
#tmpfile=`mktemp stringXXX`
#echo ${targetDirectory} | sed 's/\//\\\//g' > $tmpfile 
#targetDirectory_sed=`cat $tmpfile` 
#echo ${targetDirectory_sed}
#rm $tmpfile 

# DB installation
   cd $targetDirectory
   mkdir ILCConfDB
   cd ILCConfDB
   mkdir lib
   mkdir include
   cd ..
   svn --non-interactive --trust-server-cert co https://lyosvn.in2p3.fr/repository/ilc/src/online/database/ILCConfDB build
   cd build
   source setEnv.sh 
   cd src
#Change python to 2.7  and -Wl,--nos-as-needed
   mv Makefile Makefile_org
   sed 's/python2.6/python2.7/g' Makefile_org | sed 's/-Wl/-Wl,--no-as-needed/g' > Makefile
   make -j 8

   cd -
   cp lib64/libclntsh.so.11.1 ${targetDirectory}/ILCConfDB/lib/
   cp lib64/libnnz11.so ${targetDirectory}/ILCConfDB/lib/
   cp lib64/libocci.so.11.1 ${targetDirectory}/ILCConfDB/lib/
   cp lib64/libociei.so ${targetDirectory}/ILCConfDB/lib/
   cp lib64/libociicus.so ${targetDirectory}/ILCConfDB/lib/libociicus.so  #ABSENTE ???
   cp lib64/libocijdbc11.so ${targetDirectory}/ILCConfDB/lib/libocijdbc11.so
   cd ${targetDirectory}/ILCConfDB/lib/
   ln -s libocci.so.11.1 libocci.so
   ln -s libclntsh.so.11.1 libclntsh.so
   cd -
#   Installation des headers oracle
   cp include/*.h ${targetDirectory}/ILCConfDB/include/
#   #Installation de la librairie ilcconfdb
   cp lib64/libilcconfdb.so.0.1 ${targetDirectory}/ILCConfDB/lib/libilcconfdb.so
#   #Installation des headers ilcconfdb
   sed 's/ILCConfDB\///' src/ILCConfDB.h > ${targetDirectory}/ILCConfDB/include/ILCConfDB.h

   mkdir ${targetDirectory}/ILCConfDB/include/Exceptions
   mkdir ${targetDirectory}/ILCConfDB/include/configObjects
   mkdir ${targetDirectory}/ILCConfDB/include/daq
   mkdir ${targetDirectory}/ILCConfDB/include/db
   cp src/Exceptions/*.h ${targetDirectory}/ILCConfDB/include/Exceptions/
   cp src/daq/*.h ${targetDirectory}/ILCConfDB/include/daq/
   cp src/configObjects/*.h ${targetDirectory}/ILCConfDB/include/configObjects/
   cp src/db/*.h ${targetDirectory}/ILCConfDB/include/db/
# python interface installation
   mkdir ${targetDirectory}/ILCConfDB/pythonlib
   cd include 
   ln -s ../src/ ILCConfDB
   cd ../src
   cd swig
   ln -s ../  ILCConfDB
   cd ..
   mv swig/ilcconfdb.i swig/ilcconfdb.i_prev
   #sed 's/ILCConfDB\///' swig/ilcconfdb.i_prev > swig/ilcconfdb.i_temp
   #sed "s/\/usr/${targetDirectory_sed}\/ILCConfDB/g" swig/ilcconfdb.i_temp > swig/ilcconfdb.i_temp2
   #sed "s/ILCConfDB.h/${targetDirectory_sed}\/ILCConfDB\/include\/ILCConfDB.h/" swig/ilcconfdb.i_temp2 >  swig/ilcconfdb.i
   sed 's/\/usr\/include\///' swig/ilcconfdb.i_prev > swig/ilcconfdb.i

   make ../lib64/_ilcconfdb.so
   cp swig/ilcconfdb.py ${targetDirectory}/ILCConfDB/pythonlib/
   cp ../lib64/_ilcconfdb.so ${targetDirectory}/ILCConfDB/pythonlib/

echo "INSTALLATION DONE"
echo "C++ shared library are in ${targetDirectory}/ILCConfDB/lib"
echo "python interface library are in ${targetDirectory}/ILCConfDB/pythonlib"
