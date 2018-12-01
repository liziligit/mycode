txt_type=$1

if [ "$1" = "" ]
then
  echo txt_type is empty
  exit
fi

radiationFile=./combine_100/radiation_${txt_type}
#radiation_rd.txt or radiation_nord.txt
g++  rms_pixels_rd.cpp -o rms_pixels_rd `root-config --cflags` `root-config --glibs`
./rms_pixels_rd ${radiationFile} ${txt_type}
