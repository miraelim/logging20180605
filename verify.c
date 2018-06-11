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
#include <time.h>

#define BUFF_SIZE 2048

unsigned char digest[SHA256_DIGEST_LENGTH];
//const char* id = "MIRAE LIM";
//char rootkey[SHA256_DIGEST_LENGTH*2+1];
char rootkey[BUFF_SIZE];
char keyobject[BUFF_SIZE];
FILE  *fd, *kfd, *hmac, *temp1, *temporary, *keyobj;
int fd1, fd2;
char    temp[BUFF_SIZE];
char	hmac_log[BUFF_SIZE];
struct log{
    char* log;
    char* hmac_log;
};
float gap;
time_t starttime=0, endtime=0;


/*void verify(char *string){
  system("hmac256 \"key.txt\" \"temp1.txt\" >> temporary.txt");
  temporary = fopen("temporary.txt", "r");
  if(temporary<0)
  printf("temporary.txt fail\n");
  else
  printf("temporary.txt success\n");

  fscanf(temporary, "%s", hmac_log);
  printf("log hmac : %s\n",hmac_log);
  fprintf(hmac,"%s\t%s\n", string, hmac_log);
  fclose(temporary);
  system("rm temporary.txt");

  }
 */
int main(){
    char string1[1024]="test 1";
    char string2[1024]="test 2" ;

    temp1 = fopen("temp1.txt", "w");

    if(temp1<0)
	printf("temp1.txt openfail\n");
    else
	printf("temp1 open success\n");

    fprintf(temp1, "%s", string1);
    fprintf(temp1, "%s", string2);

    fclose(temp1);

    return 0;
}
