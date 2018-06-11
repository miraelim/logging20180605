#filelock
flock -s weatherdatapoint.txt -c cat; weatherdatapoint.txt

flock -u weatherdatapoint.txt
