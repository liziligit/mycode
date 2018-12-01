StartNum=$1
EndNum=$2

if [ "$1" = "" ]
then
  echo Start number is empty 
  exit	
fi

if [ "$2" = "" ]
then
  echo End number is empty 
  exit	
fi

for ((i=$1; i<=$2; i ++)) 
do
echo begin to turn beam_${i}.pd1
./get_pede.sh $i
sleep 1
done

