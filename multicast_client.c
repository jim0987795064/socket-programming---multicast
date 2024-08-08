/* Receiver/client multicast Datagram example. */
#include <sys/types.h>
#include <sys/socket.h>
#include<sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #define BUFFER_SIZE 1024

struct sockaddr_in localSock;
struct ip_mreq group;

int sock;
int datalen, file_write_length = 0, file_size = 0;
char recvbuffer[BUFFER_SIZE] = {0};
char *file_name;
 
int main(int argc, char *argv[])
{
	struct stat st;
/* Create a datagram socket on which to receive. */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		perror("Opening datagram socket error");
		exit(1);
	}
	else
	printf("Opening datagram socket....OK.\n");
		 
	/* Enable SO_REUSEADDR to allow multiple instances of this */
	/* application to receive copies of the multicast datagrams. */
	{
		int reuse = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
	{
		perror("Setting SO_REUSEADDR error");
		close(sock);
		exit(1);
	}
	else
		printf("Setting SO_REUSEADDR...OK.\n");
	}
	 
	/* Bind to the proper port number with the IP address */
	/* specified as INADDR_ANY. */
	memset((char *) &localSock, 0, sizeof(localSock));
	localSock.sin_family = AF_INET;
	localSock.sin_port = htons(8888);
	localSock.sin_addr.s_addr = INADDR_ANY;
	if(bind(sock, (struct sockaddr*)&localSock, sizeof(localSock)))
	{
		perror("Binding datagram socket error");
		close(sock);
		exit(1);
	}
	else
		printf("Binding datagram socket...OK.\n");
	 
	/* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
	/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
	/* called for each local interface over which the multicast */
	/* datagrams are to be received. */
	
	group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
	group.imr_interface.s_addr = inet_addr("192.168.172.121");
	if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
	{
		perror("Adding multicast group error");
		close(sock);
		exit(1);
	}
	else
		printf("Adding multicast group...OK.\n");
	 

	/* Read from the socket. */
	datalen = sizeof(recvbuffer);
	
	//receive file_name from server, resolve filename length problem
    read(sock, recvbuffer, datalen);

	FILE *fp = fopen(recvbuffer, "w+");
	//catch file_name from recvbuffer
	file_name = recvbuffer;

	memset(recvbuffer, 0, sizeof(recvbuffer));
	


    
	while(1){
	
        file_write_length = read(sock, recvbuffer, datalen);
		if(recvbuffer[0] == 'e' && recvbuffer[1] == 'n' && recvbuffer[2] == 'd' ){		  
		    break;
		}
		fwrite(recvbuffer, sizeof(char), file_write_length, fp);
        memset(recvbuffer, 0, sizeof(recvbuffer));

		
	}
	printf("Reading datagram message...OK");
    
	
	//read file_name from server
    read(sock, recvbuffer, datalen);

	//store file_size
	stat(file_name, &st);
	file_size = st.st_size;
	file_size /= 1024;
    printf("\nreceive file size: %dKB\n", file_size);


	fclose(fp);
    close(sock);

	return 0;
}
