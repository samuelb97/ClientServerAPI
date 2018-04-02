#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/errno.h>

extern int errno;

int cmpfunc(const void * a, const void *b){
	const char * ia = *(const char **)a;
	const char * ib = *(const char **)b;
	return strcmp(ia, ib);
}

int main(int argc, char * argv[]){
	if(argc != 2){
		printf("Error: Not Enough Arguments\n");
		exit(-1);
	}
	struct servent * pse;
	struct sockaddr_in sin;
	struct sockaddr_in fsin;
	int s, ssock, cc, i;
	char buf[121];
	unsigned int alen;
	int p = atoi(argv[1]);
	
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;

	printf("AF_INET\n");
	
//SET LOCAL SERVICE ADDRESS
	sin.sin_port = htons((unsigned short)p);

//CREATE SOCKET
	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s < 0){
		printf("Sort ERR: Cannot Get Socket\n");
		exit(1);
	}	
//BIND TO LOCAL SERVICE ADDRESS AND PORT
	printf("Boutta Bind\n");
	if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		printf("Sort ERR: Cannot Bind\n");
		exit(1);
	}	
//MAKE SOCKET PASSIVE
	printf("Boutta Listen\n");
	if(listen(s, 2) < 0){
		printf("Sort ERR: Cannot Listen\n");
		exit(1);
	}
	printf("Boutta Loop 1\n");
//LOOP
	while(1){
		printf("Listening on %s... \n", argv[1]);
		
		alen = sizeof(fsin);
			
	//WAIT FOR CLIENT TO CONNECT
		printf("Boutta accept\n");
		ssock = accept(s, (struct sockaddr *)&fsin, &alen);
		printf("Accepted\n");
		if(ssock < 0){
			printf("Sort ERR: problem accepting\n");
			exit(1);
		}
		int size;
	//APPLICATION PROTOCOL
		cc = recv(ssock, buf, sizeof(buf), 0);
			if(cc < 0){
				printf("Sort ERR: error on recv\n");
				exit(1);
			}
			size = atoi(buf);
				
			printf("Number Recieved: %s\n", buf);
			printf("Sort: %d strings\n", size);
	
			memset(buf, 0, sizeof(buf));
			char * str[size+1];
			int i = 0;
			while(i < size){
				str[i] = (char *)malloc(sizeof(char)*120);
				i++;
			}
			i = 0;
			FILE * fp = fdopen(ssock, "w+");
			printf("Finna loop recieve\n");
			while(i < size){
				fscanf(fp, "%s", buf);
				fflush(fp);
				strcpy(str[i], buf);
			printf("String Recieved: %s, Length: %d\n", buf, cc); 
				memset(buf, 0, sizeof(buf));
				i++;	
			}	
			
			i = 0;
			while(i < size){
				printf("Check - %s\n", str[i]);
				i++;
			}
			qsort(&str, size, sizeof(char*), cmpfunc);

			i = 0;
			char length[10];
			while(i < size){
				strcpy(buf, str[i]);
				buf[120] = '\0';
				fprintf(fp, "%s\n", buf);
			printf("Sort: Sent: %s: %d\n", buf, strlen(str[i])); 
				fflush(fp);
				printf("fp flushed\n");
				memset(buf, 0, sizeof(buf));
				i++;
			}
			cc = 0;
			fclose(fp);
		close(ssock);		
	}
	printf("Finished input\n");
	close(s);
	return 0;
}
