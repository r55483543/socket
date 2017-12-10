#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<sys/stat.h> 
#include<stdlib.h>
#include<time.h>
 
#define FOLDER_PATH "/home/root/client/"
int connectServer(int* fdSocket);
int receiveFileName(int socket_desc, char* fileName);
int receiveFile(int socket_desc, FILE* fp);

static int socket_desc;
static char server_IPaddress[128];

int main(int argc , char *argv[])
{
    //int command = 0;
	//int event = 0;
	int socket_desc_x;
	char server_fileName[128];
	//char folder_path[30];
	int numbytes;
	char buf[1000000];
	FILE *fp;
	struct stat filestat;
	int start_time = 0;
	int end_time = 0;

		printf("Input server IP address to connect\n");
		memset(server_IPaddress, '\0', sizeof(server_IPaddress));
		scanf("%s",server_IPaddress);
		printf("Input a file name for store\n");
		memset(server_fileName, '\0', sizeof(server_fileName));
		
		scanf("%s",server_fileName);
		
		connectServer(&socket_desc_x);
		
		
		//event = htonl(command);

		if ( (fp = fopen(server_fileName, "wb")) == NULL){
			printf("open file fail\n");
			perror("fopen");
			exit(1);
		}
		start_time = time((time_t*)NULL);
		while(1){
			numbytes = read(socket_desc, buf, sizeof(buf));
			printf("read %d bytes, \n", numbytes);
			if(numbytes == 0)
			{
				printf("Client receice success\n");
				end_time = time((time_t*)NULL);
				break;
			}
			numbytes = fwrite(buf, sizeof(char), numbytes, fp);
			printf("fwrite %d bytesn\n", numbytes);
		}	
		fclose(fp);	
		if ( lstat(server_fileName, &filestat) < 0){
			exit(1);
		}
		printf("Total time of transfer file is %d seconds\n", (end_time - start_time));
		printf("The file size of client receive is %lu byte\n", filestat.st_size);
	
    return 0;
}

int connectServer(int* fdSocket)
{
	//int socket_desc;
    struct sockaddr_in server;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
         
    server.sin_addr.s_addr = inet_addr(server_IPaddress);
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
	
		//Connect to remote server
		if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
		{
			puts("connect error");
			return -1;
		}
		 
		puts("Connected\n");
		//*fdSocket = socket_desc;
		return 0;
}


int receiveFileName(int socket_desc, char* fileName)
{
	char server_reply[128];
	memset(server_reply, '\0', sizeof(server_reply));
	while(1)
	{
		
		if( recv(socket_desc, server_reply , 128 , 0) < 0)
		{
			puts("recv failed");
		}
		else
		{
			printf("File name is %s",server_reply);
			break;
		}
	}
	puts(server_reply);
	//strncpy(fileName, server_reply, sizeof(server_reply));
	//printf("File name is %s",fileName);
	return 0;
}

int receiveFile(int socket_desc, FILE* fp)
{
	int numbytes;
	char buf[1000000];
	while(1){
		numbytes = read(socket_desc, buf, sizeof(buf));
		printf("read %d bytes, ", numbytes);
		if(numbytes == 0){
			break;
		}
		numbytes = fwrite(buf, sizeof(char), numbytes, fp);
		printf("fwrite %d bytesn", numbytes);
	}
	return 0;
}