#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/errno.h>

int main(int argc, char * argv[]){
	if(argc != 2){
		printf("Merge Err: Not enough Arguments\n");
		exit(1);
	}
	
	struct servent * pse;	
	struct sockaddr_in sin;
	struct sockaddr_in fsin;
	int s, ssock, cc, i;
	
	char buf[121];
	unsigned int alen;
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
//SET LOCAL ADDRESS SERVER
	sin.sin_addr.s_addr = INADDR_ANY;
	printf("Merge: Hello\n");
//SET THE LOCAL SERVICE PORT
	sin.sin_port = htons((unsigned short)atoi(argv[1]));
//CREATE SOCKET
	s = socket(PF_INET, SOCK_STREAM, 0);
	if(s < 0){
		printf("Merge Err: Cannot get socket\n");
		exit(1);		
	}
//BIND TO LOCAL SERVICE ADDRESS AND PORT
	if(bind(s, (struct sockaddr*)&sin, sizeof(sin)) < 0){
		printf("Merge Err: Cannot Bind\n");
		exit(1);
	}
//MAKE SOCKET PASSIVE
	if(listen(s,2) < 0){
		printf("Cannot Listen\n");
		exit(1);
	}
	printf("Let Me Get Uhhhhh... Loop\n");
	while(1){
		printf("Listening on %s...\n", argv[1]);
		alen = sizeof(fsin);
		
//WAIT FOR CLIENT TO CONNECT
		ssock = accept(s,(struct sockaddr *)&fsin, &alen);  

		if(ssock < 0){
			printf("Problem accepting\n");
			exit(1);
		}		
//APPLICATION PROTOCOL SECTION START===
		cc = recv(ssock, buf, sizeof(buf), 0);
			if(cc < 0){
				printf("Merge: Error on read 1\n");
				exit(1);
			}	
			buf[120] = '\0';
			printf("Merge: Recieved: %s\n", buf);
			
			int nStr1 = atoi(buf);
			memset(buf, 0, sizeof(buf));

			FILE * fp = fdopen(ssock, "w+");
			fscanf(fp, "%s", buf);

			printf("Merge: Recieved: %s\n", buf);
			buf[120] = '\0';
			int nStr2 = atoi(buf);
			memset(buf, 0, sizeof(buf));

			int nsize = nStr1 + nStr2;
			
			char * arr1[nStr1+1];
			char * arr2[nStr2+1];
			char * final[nsize+1];

			int i = 0;
			while(i <= nStr1){
				arr1[i] = (char *)malloc(sizeof(char)*121);
				i++;
			}	
			i = 0;
			while(i <= nStr2){
				arr2[i] = (char *)malloc(sizeof(char)*121);
				i++;
			}
			i = 0;
			while(i <= nsize){
				final[i] = (char *)malloc(sizeof(char)*121);
				i++;
			}
			arr1[nStr1] = "+++";
			arr2[nStr2] = "+++";
			i = 0;
			printf("Finna Loop Recieve\n");
			while(i < nStr1){
				fscanf(fp, "%s", buf);
				fflush(fp);
				strcpy(arr1[i], buf);
				printf("Recieved: Arr1: %d: %s\n",i, arr1[i]);
				memset(buf, 0, sizeof(buf));
				i++;
			}
			i = 0;
			while(i < nStr2){
				fscanf(fp, "%s", buf);
				fflush(fp);
				memset(arr2[i], 0, sizeof(arr2[i]));
				strcpy(arr2[i], buf);
				printf("Recieved: Arr2: %d: %s\n", i, arr2[i]);
				memset(buf, 0, sizeof(buf));
				i++;
			}
			i = 0;
			int j = 0;
			int k = 0;
//Merge Data to Final Array
			nsize = (nStr1 + nStr2);
			while(i < nsize){
				if(arr1[k] == "+++" && arr2[j] == "+++"){
				//	printf("Both At End\n");
					break;
				}
				else if(arr1[k] == "+++"){
				//	printf("Arr1 End\n");
					final[i] = arr2[j];
					j++;
					i++;
				}
				else if(arr2[j] == "+++"){
				//	printf("Arr2 End: %d, %s\n", j, arr2[j]);
					final[i] = arr1[k];
					k++;
					i++;
				}
				else if(strcmp(arr1[k], arr2[j]) < 0){
				//	printf("1 < 2\n");
					final[i] = arr1[k];
					k++;
					i++;
				}
				else{
				//	printf("2 < 1\n");
					final[i] = arr2[j];
					j++;
					i++;
				}
				printf("FinalArr: %d: %s\n", i, final[i]);
			}
			i = 0;
			while(i < nsize){
				fprintf(fp, "%s\n", final[i]);
				fflush(fp);
				printf("Sent: %s\n", final[i]);
				i++;
			}
		fclose(fp);
		close(ssock);	
	}
}
