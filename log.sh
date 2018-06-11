#!/bin/bash
rm key.txt

#rootkey generation

openssl rand 64 >> key.txt

#cp key.txt key1.txt
#rootkey seal
./seal.sh

#filelock
flock -s weatherdatapoint.txt -c ls
if [ $? != 0 ];then
echo log lock fail
exit 1
else
echo log lock SUCCESS
fi

flock -s key.txt -c ls
if [ $? != 0 ];then
echo keyfile lock fail
exit 1
else
echo keyfile lock SUCCESS
fi

#log hmac

./unseal.sh
while read line
do
hmac256 key.txt temp.txt >> hmac.txt
cp temp.txt temp1.txt
rm temp.txt

done < weatherdatapoint.txt
#file unlock
flock -u weatherdatapoint.txt -c ls
if [ $? != 0 ];then 
echo log unlock fail
exit 1
else 
echo log unlock SUCCESS
fi

flock -u key.txt -c ls
if [ $? != 0 ];then
echo keyfile unlock fail
exit 1
else
echo keyfile unlock SUCCESS
fi

#generate newkey
sha1sum key.txt >> keyobject.txt

NEWKEY=`cat keyobject.txt`

rm key.txt
var=$(echo $NEWKEY | awk -F" " '{print $1,$2}') 
set -- $var
echo $1 >> key.txt
echo $2
rm keyobject.txt
./seal.sh

