LOG=`cat hmac.txt`
INDEX=1
var=$(echo $LOG | awk -F" " '{print $1,$2, $3, $4}')
set -- $var
echo start
echo $1
echo $2
echo $3
echo $4
TEMP1=`echo $3`

while read line
do
echo line
echo $line
echo $line >> temp.txt
hmac256 key.txt temp.txt >> hmac1.txt

HMAC=`cat hmac1.txt`
echo $HMAC
var1=$(echo $HMAC | awk -F" " '{print $1,$2}')
set -- $var1

echo $1
echo $2
TEMP2=`echo $1`
echo TEMP1
echo $TEMP1
echo TEMP2
echo $TEMP2
if [ "$TEMP1" == "$TEMP2" ];
then
    echo same
    else 
    echo different
fi
rm temp.txt
rm hmac1.txt
done < weatherdatapoint.txt

