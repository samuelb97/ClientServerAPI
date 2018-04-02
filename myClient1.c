#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]){
	char * sort1_h = (char *)malloc(sizeof(char)*50);
	char * sort2_h = (char *)malloc(sizeof(char)*50);
	char * merge_h = (char *)malloc(sizeof(char)*50);
	char * sIn = (char *)malloc(sizeof(char)*10);

	int sort1_p;
	int sort2_p;
	int merge_p;

	int nStr;
	int clevel = 2;

	printf("Hostname for mySort Service 1:\n");
	scanf("%s", sort1_h);
//	printf("%s\n", sort1_h); 
	printf("Port Number for mySort Service 1:\n");
	scanf("%s", sIn);
	sort1_p = atoi(sIn);

	printf("Hostname for mySort Service 2:\n");
	scanf("%s", sort2_h);
	printf("Port Number for mySort Service 2:\n");
	scanf("%s", sIn);
	sort2_p = atoi(sIn);

//	printf("Post Fault\n");
	printf("Hostname for myMerge Service:\n");
	scanf("%s", merge_h);
	printf("Port Number for myMerge Service:\n");
	scanf("%s", sIn);
	merge_p = atoi(sIn);
	
	printf("Number of Strings:\n");
	scanf("%s", sIn);
	nStr = atoi(sIn);	
	int nstr1 = nStr/2;
	int nstr2 = nStr - nstr1;

	printf("C-level (Always 2):\n");
	scanf("%s", sIn);
	clevel = atoi(sIn);

	char * Size1 = (char *)malloc(sizeof(char)*10);
	char * Size2 = (char *)malloc(sizeof(char)*10);

	sprintf(Size1, "%d", nstr1);
	sprintf(Size2, "%d", nstr2);
	
	struct hostent * phe;

	struct sockaddr_in saddr1;
	struct sockaddr_in saddr2;
	struct sockaddr_in saddr3;

	int s1, s2, s3;
	char buf[121];
	char buf2[121];
	int outchars, inchars, n;

	memset(&saddr1, 0, sizeof(saddr1));
	memset(&saddr2, 0, sizeof(saddr2));
	memset(&saddr3, 0, sizeof(saddr3));

	saddr1.sin_family = AF_INET;
	saddr2.sin_family = AF_INET;
	saddr3.sin_family = AF_INET;

	printf("Let Me Get Uhhhhhh... Hello\n");
//SEARCH FOR THE ADDRESS GIVEN THE HOSTNAME OF THE GIVEN SYSTEM
	if( phe = gethostbyname(sort1_h) ){
		printf("Let Me Get Uhhhhh...  host %s\n", phe->h_name);
		memcpy(&saddr1.sin_addr, phe->h_addr, phe->h_length);
	}	
	else{
		printf("Can't get host\n");
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
//SET PORT NUMBER OF REMOTE SYSTEM
	saddr1.sin_port = htons((unsigned short)sort1_p);
	printf("Sock1 Port: %d\n", saddr1.sin_port);
	
//CREATE SOCKET 1
	s1 = socket(PF_INET, SOCK_STREAM, 0);
	if(s1 < 0){
		printf("Cannot get socket\n");
		exit(1);
	}		
//CONNECT TO THE REMOTE SYSTEM
//SERVER 1=================================
	if(connect(s1, (struct sockaddr *)&saddr1, sizeof(saddr1)) < 0){
		printf("Cannot Connect\n");
		exit(1);
	}
	outchars = strlen(Size1);			
	int i = 0;
	
	char * arr1[nstr1];
	while(i < nstr1){
		arr1[i] = (char *)malloc(sizeof(char));
		i++;
	}
	i = 0;		
	
	send(s1, Size1, outchars, 0);

	FILE * fp = fdopen(s1, "w+");		

	i = 0;
	while(i < nstr1){
		scanf("%s\0", arr1[i]);
		strcpy(buf, arr1[i]);
		buf[120] = '\0';
//		printf("Sending: %s\n", buf);		
		fprintf(fp, "%s\n", buf);
		fflush(fp);
		memset(buf, 0, sizeof(buf));
		i++;
	}
	printf("Let Me Get Uhhhhhhh... Recieve Loop\n");
	i = 0;
	int dd;
//	printf("Seg Test\n");
	char buffer[121];
	buffer[120] = '\0';

	char * sArr[nstr1];
	while(i < nstr1){
		sArr[i] = (char *)malloc(sizeof(char));
		i++;
	}	
	i = 0;
	while(i < nstr1){
//		printf("looping\n");
		fscanf(fp, "%s", sArr[i]);	
//		dd = recv(s1, sArr[i], 120, 0);
//		printf("Recieved: %s, Size: %d\n", sArr[i], strlen(sArr[i]));
		fflush(fp);
		strcpy(arr1[i], sArr[i]);
//		printf("Flushed\n");
		memset(buf, 0, sizeof(buf));
		i++;
	} 	
	i = 0;	
	while(i < nstr1){
		printf("Arr: %d: %s\n", i, arr1[i]);
		i++;
	}
	close(s1);
//SERVER 2================
	if( phe = gethostbyname(sort2_h) ){
		memcpy(&saddr2.sin_addr, phe->h_addr, phe->h_length);
	}
	else{
		printf("Cant get host 2\n");
		exit(1);
	}
	saddr2.sin_port = htons((unsigned short)sort2_p);
	//CREATE SOCKET=====================
	s2 = socket(PF_INET, SOCK_STREAM, 0);
	if(s2 < 0){
		printf("Cannot get socket\n");
		exit(1);
	}
	printf("Socket 2 Made\n");
	printf("nstr2 : %d\n", nstr2);
	//CONNECT========================
	if(connect(s2, (struct sockaddr *)&saddr2, sizeof(saddr2)) < 0){
		printf("Cannot Connect\n");
		exit(1);
	}
	char * arr2[nstr2];
	i = 0;
	while(i < nstr2){
		arr2[i] = (char *)malloc(sizeof(char));
		i++;
	}
	outchars = strlen(Size2);
	i = 0;
	send(s2, Size2, outchars, 0);
	fp = fdopen(s2, "w+");
	while(i < nstr2){
		scanf("%s\0", arr2[i]);
		strcpy(buf, arr2[i]);
		buf[120] = '\0';
		fprintf(fp, "%s\n", buf);
		fflush(fp);
		memset(buf, 0, sizeof(buf));
		i++;
	}
	i = 0;
	while(i < nstr2){
		fscanf(fp, "%s", arr2[i]);
		fflush(fp);
		i++;	
	}
	i = 0;
	while(i < nstr2){
		printf("Arr: %d: %s\n", i, arr2[i]);
		i++;
	}
	close(s2);
//SERVER 3==========================
	if( phe = gethostbyname(merge_h) ){
		memcpy(&saddr3.sin_addr, phe->h_addr, phe->h_length);
	}
	else{
		printf("Cant get host 3\n");
		exit(1);
	}
	saddr3.sin_port = htons((unsigned short)merge_p);
	//CREATE SOCKET====================================
	s3 = socket(PF_INET, SOCK_STREAM, 0);
	if(s3 < 0){
		printf("Cannot get socket 3\n");
		exit(1);
	}
	printf("Socket 3 Made\n");
	//CONNECT============================
	if( connect(s3, (struct sockaddr *)&saddr3, sizeof(saddr3)) < 0){
		printf("Cannot Connect\n");
		exit(1);
	}
	i = 0;
//	printf("KKKKKKKKKKKKKKKKKKKKKK\n");
	fp = fdopen(s3, "w+");
	fprintf(fp, "%s\n", Size1);
	fflush(fp);
	fprintf(fp, "%s\n", Size2);
	fflush(fp);
//	outchars = strlen(Size1);
//	send(s3, Size1, outchars, 0);
//	outchars = strlen(Size2);
//	send(s3, Size2, outchars, 0);
//	printf("===========================\n");
	while(i < nstr1){
		strcpy(buf, arr1[i]);
		fprintf(fp, "%s\n", buf);
		fflush(fp);
		memset(buf, 0, sizeof(buf));
		i++;
	}
	i = 0;
	printf("+++++++++++++++++++++++++\n");
	while(i < nstr2){
		strcpy(buf, arr2[i]);
		fprintf(fp, "%s\n", buf);
		fflush(fp);
		memset(buf, 0, sizeof(buf));
		i++;
	}
	i = 0;
	char * arrF[nstr1 + nstr2];
	int nsize = nstr1 + nstr2;
	printf("nsize = %d\n", nsize);
	while(i < nsize){
		arrF[i] = (char *)malloc(sizeof(char));
		i++;
	}
	i = 0;	
	while(i < nsize){
		fscanf(fp, "%s", arrF[i]);
		printf("Recieved: %s\n", arrF[i]); 
		fflush(fp);
		i++;
	}
	i = 0;
	fclose(fp);
	while(i < nsize){
		printf("Final Arr: %s\n", arrF[i]);
		i++;
	}
	close(s3);
}
