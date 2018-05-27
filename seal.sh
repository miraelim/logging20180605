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

rm $file_primary_key_ctx $file_unseal_key_pub $file_unseal_key_priv $file_unseal_key_ctx $file_unseal_key_name $file_unseal_output_data -rf

begintakeownershipTime=$(date +%s%N)
tpm2_takeownership -c
endtakeownershipTime=$(date +%s%N)
takeownershipelapsed=`echo "($endtakeownershipTime - $begintakeownershipTime) / 1000000" | bc`
takeownershipelapsedSec=`echo "scale=6;$takeownershipelapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
echo takeownership time: $takeownershipelapsedSec sec

begincreateprimaryTime=$(date +%s%N)
tpm2_createprimary -A p -g $alg_primary_obj -G $alg_primary_key -C $file_primary_key_ctx
if [ $? != 0 ];then
echo "createprimary fail, please check the environment or parameters!"
exit 1
fi
endcreateprimaryTime=$(date +%s%N) 
createprimaryelapsed=`echo "($endcreateprimaryTime - $begincreateprimaryTime) / 1000000" | bc` 
createprimaryelapsedSec=`echo "scale=6;$createprimaryelapsed / 1000" | bc | awk '{printf "%.6f", $1}'` 
echo createprimary time: $createprimaryelapsedSec sec

begincreateTime=$(date +%s%N)
tpm2_create -g $alg_create_obj -G $alg_create_key -o $file_unseal_key_pub -O $file_unseal_key_priv  -I $file_input_data -c $file_primary_key_ctx
if [ $? != 0 ];then
echo "create fail, please check the environment or parameters!"
exit 1
fi
endcreateTime=$(date +%s%N) 
createelapsed=`echo "($endcreateTime - $begincreateTime) / 1000000" | bc` 
createelapsedSec=`echo "scale=6;$createelapsed / 1000" | bc | awk '{printf "%.6f", $1}'` 
echo create time: $createelapsedSec sec

beginloadTime=$(date +%s%N)
tpm2_load -c $file_primary_key_ctx  -u $file_unseal_key_pub  -r $file_unseal_key_priv -n $file_unseal_key_name -C $file_unseal_key_ctx
if [ $? != 0 ];then
echo "load fail, please check the environment or parameters!"
exit 1
fi
endloadTime=$(date +%s%N) 
loadelapsed=`echo "($endloadTime - $beginloadTime) / 1000000" | bc` 
loadelapsedSec=`echo "scale=6;$loadelapsed / 1000" | bc | awk '{printf "%.6f", $1}'` 
echo load time: $loadelapsedSec sec
