rmsfile=$1

if [ "$1" = "" ]
then
  echo rmsfile name is empty
  exit
fi

rms_dir=../step2_rms/rms_txt/
g++  rms_of_rms.cpp -o rms_of_rms `root-config --cflags` `root-config --glibs`
./rms_of_rms ${rms_dir} ${rmsfile}
#0rms_noradiaton.txt or 0rms_radiaton.txt
