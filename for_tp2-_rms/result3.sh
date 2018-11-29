pedeId=$1

if [ "$1" = "" ]
then
  echo Pede number is empty
  exit
fi

pedeFile=./beam_txt/beam
g++  result3.cpp -o result3 `root-config --cflags` `root-config --glibs`
./result3 ${pedeFile} ${pedeId}
