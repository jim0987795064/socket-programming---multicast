/* Send Multicast Datagram code example. */
#include <sys/types.h>
#include <sys/socket.h>
#include<sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024


struct in_addr localInterface;
struct sockaddr_in groupSock;
int sock, file_read_length = 0, file_size = 0;
char sendbuffer[1024] = {0};
int datalen = sizeof(sendbuffer);
 
int main (int argc, char *argv[ ])
{
	struct stat st;
	char *file_name = argv[1];
	char *transfer_end = "";

/* Create a datagram socket on which to send. */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
	  perror("Opening datagram socket error");
	  exit(1);
	}
	else
	  printf("Opening the datagram socket...OK.\n");
	 
	/* Initialize the group sockaddr structure with a */
	/* group address of 225.1.1.1 and port 5555. */
	memset((char *) &groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
	groupSock.sin_port = htons(8888);
	 
	 
	/* Set local interface for outbound multicast datagrams. */
	/* The IP address specified must be associated with a local, */
	/* multicast capable interface. */
	localInterface.s_addr = inet_addr("192.168.172.121");
	if(setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
	{
	  perror("Setting local interface error");
	  exit(1);
	}
	else
	  printf("Setting the local interface...OK\n");
	/* Send a message to the multicast group specified by the*/
	/* groupSock sockaddr structure. */

	FILE *fp = fopen(file_name, "r");
    
	//send file_name to client
	datalen = sizeof(sendbuffer);
	sendto(sock, file_name, datalen, 0, (struct sockaddr*)&groupSock, sizeof(groupSock));

	while( (file_read_length = fread(sendbuffer, sizeof(char), BUFFER_SIZE, fp)) > 0){  
             if (sendto(sock, sendbuffer, file_read_length, 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0){  
                printf("Send File:\t%s Failed!\n", file_name);  
                break;  
            }
			
	}
	//send ending message to client
    transfer_end = "end19970709";
    sendto(sock, transfer_end, datalen, 0, (struct sockaddr*)&groupSock, sizeof(groupSock));
    
	//send file_name to client
    sendto(sock, file_name, datalen, 0, (struct sockaddr*)&groupSock, sizeof(groupSock));

	printf("Sending datagram messaage...OK\n");
    
	//store file_size
	stat(file_name, &st);
	file_size = st.st_size;
	file_size /= 1024;
    printf("file size: %dKB\n", file_size);

    fclose(fp);
    close(sock);
	return 0;
}