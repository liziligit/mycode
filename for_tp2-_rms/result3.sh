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

pedeFile=./beam_txt/beam

#for((i=$1;i<=$2; i++)) can work too
for((i=${pedeStart};i<=${pedeEnd}; i++))
do
g++  result3.cpp -o result3 `root-config --cflags` `root-config --glibs`
./result3 ${pedeFile} ${i}
sleep 3
done

