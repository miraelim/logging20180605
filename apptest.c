#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <sys/file.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/x509.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
//#include "tss2/TPM_Types.h"
#include <time.h>

#define BUFF_SIZE 2048

unsigned char digest[SHA256_DIGEST_LENGTH];
//const char* id = "MIRAE LIM";
//char rootkey[SHA256_DIGEST_LENGTH*2+1];
char rootkey[BUFF_SIZE];
char keyobject[BUFF_SIZE];
FILE  *fd, *kfd, *hmac, *temp1, *temporary, *keyobj, *temp2, *temporary1;
int fd1, fd2;
char    temp[BUFF_SIZE];
char	hmac_log[BUFF_SIZE];
struct log{
    char* log;
    char* hmac_log;
};
float gap;
time_t starttime=0, endtime=0;


void makeRootkey(){
    system("openssl rand 64 >> key.txt");
}

void start(){
    kfd = fopen("key.txt", "r");
    char key[1024];
    if(kfd<0)
	printf("open kfd first fail\n");

    else{
	printf("open fd1 success\n");
	fscanf(kfd,"%s", key);
	printf("key\n");
    }
    system("./seal.sh");
    fclose(kfd);

}

void get_filelock(){

    fd1 = open("key.txt", O_RDWR);
    fd2 = open("weatherdatapoint.txt", O_RDWR);
    if(fd1<0)
	printf("open fd1 fail\n");

    else{
	printf("open fd1 success\n");
	read(fd1, keyobject, 2048);
    }

    //    printf("test1\n");
    if(fd2<0)
	printf("open fd2 fail\n");

    else{
	printf("open fd2 success\n");
	read(fd2, temp, 2048);
	printf("logs: %s\n",temp);
    }

    if (flock(fd1, LOCK_SH) == -1) {
	printf("key lock failed\n");
	exit(1);
    }
    else
	printf("key.txt lock success\n");

    if (flock(fd2, LOCK_SH) == -1) {
	printf("log lock failed\n");
	exit(1);
    }
    else
	printf("log.txt lock success\n");
    printf("fd in filelock:%d\n",fd2);
}

void get_fileunlock(){
    if (flock(fd1, LOCK_UN) == -1) {
	printf("keyobject unlock failed\n");
	exit(1);
    }
    else
	printf("keyobject.txt unlock success\n");

    if (flock(fd2, LOCK_UN) == -1) {
	printf("log unlock failed\n");
	exit(1);
    }
    else
	printf("log.txt unlock success\n");
    close(fd1);
    close(fd2);
}

void log_hmac(char *string){
    system("hmac256 \"key.txt\" \"temp1.txt\" >> temporary.txt");
    temporary = fopen("temporary.txt", "r");
    if(temporary<0)
	printf("temporary.txt fail\n");
    else
	printf("temporary.txt success\n");

    fscanf(temporary, "%s", hmac_log);
    printf("log hmac : %s\n",hmac_log);
    fprintf(hmac,"%s\n", hmac_log);
    fclose(temporary);
//    system("rm temporary.txt");

}

void log_hmac1(char *string){
         system("hmac256 \"key.txt\" \"temp2.txt\" >> temporary1.txt");
         temporary1 = fopen("temporary1.txt", "r");
        if(temporary1<0)
	         printf("temporary1.txt fail\n");
        else
	         printf("temporary1.txt success\n");
    
	     fscanf(temporary1, "%s", hmac_log);
         printf("log hmac : %s\n",hmac_log);
         fprintf(hmac,"%s\n", hmac_log);
         fclose(temporary1);
//    system("rm temporary.txt");
    
	 }

void generate_newkey(){
    char newkey[BUFF_SIZE], temp[BUFF_SIZE];
    system("sha1sum \"key.txt\" >>\"keyobject.txt\"");

    keyobj = fopen("keyobject.txt", "r");
    if(keyobj<0)
	printf("keyobject.txt fail\n");
    else
	printf("keyobj.txt success\n");

    fscanf(keyobj, "%s %s", newkey,temp);
    printf("new key : %s	temp:%s\n",newkey, temp);


    system("rm key.txt");
    kfd = fopen("key.txt", "w");
    if(keyobj<0)
	printf("key2.txt fail\n");
    else
	printf("key2.txt success\n");

    fprintf(kfd,"%s", newkey);

    fclose(kfd);
    system("./seal1.sh");

}

int main(){
    char string[1024],string1[1024];

    starttime = clock();
    fd = fopen("weatherdatapoint.txt", "r");

    if(fd<0)
	printf("weatherdatapoint fail\n");
    else 
	printf("weatherdatapoint success\n");

    hmac = fopen("hmac.txt", "a");

    if(hmac<0)
	printf("hmac open fail\n");
    else
	printf("hmac open success\n");

    makeRootkey();
    start();
    get_filelock();

//    while( fgets(string, 1024,fd)!= NULL){
   fgets(string, 1024,fd);
	system("./unseal.sh");
	system("rm temp1.txt");
	temp1 = fopen("temp1.txt", "w");

	if(temp1<0)
	    printf("temp1.txt openfail\n");
	else
	    printf("temp1 open success\n");
	printf("%s\n",string);
	fprintf(temp1, "%s", string);
	log_hmac(string);
//}
	 fgets(string1, 1024,fd);
	  temp2 = fopen("temp2.txt", "w");
	 
	         if(temp2<0)
	                  printf("temp2.txt openfail\n");
	         else
	                  printf("temp2 open success\n");
	         printf("%s\n",string1);
	          fprintf(temp2, "%s", string1);
	         log_hmac1(string1);

    get_fileunlock();
    generate_newkey();
    fclose(hmac);
    fclose(temp1);
    ////fclose(keyobj);
    endtime = clock();
    gap = (float) (endtime - starttime)/(CLOCKS_PER_SEC);
    printf("time: %f\n",gap);

    return 0;
}
