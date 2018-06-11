alg_primary_obj=0x000B
#key primary : SHA 256
alg_primary_key=0x0023 
#key algorithm: TPM_ALG_ECC
alg_create_obj=0x000B
#key primary : SHA 256
alg_create_key=0x0023
#key algorithm: TPM_ALG_ECC


alg_load=0x0004

file_primary_key_ctx=context.p_"$alg_primary_obj"_"$alg_primary_key"
file_load_key_pub=opu_"$alg_create_obj"_"$alg_create_key"
file_load_key_priv=opr_"$alg_create_obj"_"$alg_create_key"
file_load_key_name=name.load_"$alg_primary_obj"_"$alg_primary_key"-"$alg_create_obj"_"$alg_create_key"
file_load_key_ctx=key1.txt
file_load_output=load_"$file_load_key_ctx"

fail()
{
	    echo "$1 test fail, please check the environment or parameters!"
#			    echo ""$1" fail" >>test_encryptdecrypt_error.log
 exit 1
}
Pass()
{
	    echo ""$1" pass" >>test_getpubak_pass.log
}

rm $file_primary_key_ctx $file_load_key_pub $file_load_key_priv $file_load_key_name $file_load_key_ctx  $file_load_output  -rf


tpm2_takeownership -c

tpm2_createprimary -A e -g $alg_primary_obj -G $alg_primary_key -C $file_primary_key_ctx
if [ $? != 0 ];then
	 fail createprimary 
fi
tpm2_create -g $alg_create_obj -G $alg_create_key -o $file_load_key_pub -O $file_load_key_priv  -c $file_primary_key_ctx
if [ $? != 0 ];then
	fail create 
fi

tpm2_load -c $file_primary_key_ctx  -u $file_load_key_pub  -r $file_load_key_priv -n $file_load_key_name -C $file_load_key_ctx
if [ $? != 0 ];then
	fail load   
fi

