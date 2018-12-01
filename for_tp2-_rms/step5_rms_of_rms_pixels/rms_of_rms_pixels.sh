RadiationType=$1

if [ "$1" = "" ]
then
  echo RadiationType is empty
  exit
fi


RadFile=../step4_rms_pixels/combine_100/${RadiationType}_new100rms
#nord_new100rms.txt or rd_new100rms.txt
g++  rms_of_rms_pixels.cpp -o rms_of_rms_pixels `root-config --cflags` `root-config --glibs`

./rms_of_rms_pixels ${RadFile} ${RadiationType}


