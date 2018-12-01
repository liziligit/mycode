pedeStart=$1
pedeEnd=$2

if [ "$1" = "" ]
then
  echo Pede Start No. is empty
  exit
fi

if [ "$2" = "" ]
then
  echo Pede End No. is empty
  exit
fi

pedeFile=../beam_txt/beam
g++  rms_for_8chip.cpp -o rms_for_8chip `root-config --cflags` `root-config --glibs`

#for((i=$1;i<=$2; i++)) can work too
for((i=${pedeStart};i<=${pedeEnd}; i++))
do
./rms_for_8chip ${pedeFile} ${i}
sleep 1
done

