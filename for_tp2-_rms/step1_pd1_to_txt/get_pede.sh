fileIdStart=$1
#let fileIdEnd=${fileIdStart}+1

if [ "$1" = "" ]
then
  echo Pedestal number is empty 
  exit	
fi


#sampleBytes=1718592
#script_BM=../../script_bm
#cd ${script_BM}
pedeDataChar=beam
pedeDataPathLocal=../runData

#cd ../../daq

#pede=../../data/pedeData/pede
#displayMode=2 ### save pd1 file

#cd ../topmetal1X8/new


echo root -L -l -q -b "get_pede.cpp++(\"${pedeDataPathLocal}\",\"${pedeDataChar}_\",${fileIdStart})"
root -L -l -q -b "get_pede.cpp++(\"${pedeDataPathLocal}\",\"${pedeDataChar}_\",${fileIdStart})"


#cd ${script_BM}
