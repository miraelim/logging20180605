#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char string[1024], string1[1024];
    FILE *fd, *temp1, *temp2;
    fd = fopen("weatherdatapoint.txt", "r");
    temp1 = fopen("temp1.txt", "w");
    temp2 = fopen("temp2.txt", "w");
    if (fd<0)
	printf("weatherdatapoint fail\n");
    else
	printf("weatherdatapoint success\n");


    fgets(string, 1024,fd);
    fgets(string1, 1024, fd);
    printf("%s\n",string);
    printf("%s\n",string1);

    if(temp1<0)
	printf("temp1.txt openfail\n");
    else
	printf("temp1 open success\n");
    if(temp2<0)
	printf("temp2.txt openfail\n");
    else
	printf("temp2 open success\n");
    fprintf(temp1, "%s", string);
    fprintf(temp2, "%s", string1);

    system("./hmac.sh");
    system("hmac256 key.txt temp2.txt");
    return 0;
}

