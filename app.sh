beginappTime=$(date +%s%N)
./app1
endappTime=$(date +%s%N) 
appelapsed=`echo "($endappTime - $beginappTime) / 1000000" | bc` 
appelapsedSec=`echo "scale=6;$appelapsed / 1000" | bc | awk '{printf "%.6f", $1}'` 
echo app execution time: $appelapsedSec sec
