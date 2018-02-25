#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	int clisock=socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in cli_addr;
    memset((char *)&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    cli_addr.sin_port = htons(0);
    if(bind(clisock, (struct sockaddr *)&cli_addr, sizeof(cli_addr)) != 0)
    {
        perror("bind failed");
        return -1;
    }
    struct sockaddr_in serv_addr;
	memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
	char msg_buffer[4096];
	strcpy(msg_buffer, "Message!");
	if( sendto(clisock, msg_buffer, strlen(msg_buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
	{
		perror("Sendto failed.");
		return -1;
	}
	
}