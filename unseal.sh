alg_primary_obj=0x000B
alg_primary_key=0x0023
alg_create_obj=0x000B
alg_create_key=0x0008

file_input_data=key.txt
file_primary_key_ctx=primary.txt
file_unseal_key_pub=public.txt
file_unseal_key_priv=private.txt
file_unseal_key_ctx=unsealctx.txt
file_unseal_key_name=loadname.txt
file_unseal_output_data=unsealoutput.txt 

rm unsealoutput.txt

beginTime=$(date +%s%N)
tpm2_unseal -c $file_unseal_key_ctx -o $file_unseal_output_data
if [ $? != 0 ];then
echo "unseal fail, please check the environment or parameters!"
exit 1
fi
endTime=$(date +%s%N)
elapsed=`echo "($endTime - $beginTime) / 1000000" | bc` 
elapsedSec=`echo "scale=6;$elapsed / 1000" | bc | awk '{printf "%.6f", $1}'` 
echo Unseal : $elapsedSec sec
