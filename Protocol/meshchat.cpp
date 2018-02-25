#include <iostream>
#include "meshchat.h"

using namespace std;

/***** MESHCLIENT GLOBAL VARIABLES ******/
vector<Node> MeshClient::neighbours;

/***** MESHCLIENT STATIC FUNCTIONS ******/
void MeshClient::node_init()
{
    int udsock;
    if( !(udsock=socket(SOCKET_CONN_TYPE, SOCK_DGRAM, 0)) )
    {
        perror("Error creating socket.");
        return;
    }
    struct sockaddr_in loc_addr;
    memset((char *)&loc_addr, 0, sizeof(loc_addr));
    loc_addr.sin_family = AF_INET;
    loc_addr.sin_addr.s_addr = htonl(LOCIP);
    loc_addr.sin_port = htons(0); //OS assigns port.
    if(bind(udsock, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) != 0)
    {
        perror("bind failed");
        return;
    }

    char rec_buffer[MSGSIZE];
    struct sockaddr_in remote_addr;
    uint32_t loclen = sizeof(loc_addr), addrlen;
    if(getsockname(udsock, (struct sockaddr*)&loc_addr, &loclen) != 0)
    {
        perror("Error getting socket name bad.");
        return;
    }
    cout << "Listening on port " << ntohs(loc_addr.sin_port) <<endl;
    //receive packets.
    while(1){
        int recv_status = recvfrom(udsock, rec_buffer, MSGSIZE, 0, (struct sockaddr *)&remote_addr, &addrlen);
        if(recv_status < 0)
        {
            perror("Receive failed.");
            return;
        }
        printf("Received message: %s\n", rec_buffer);

    }
}


/****** MAIN ******/
int main()
{
    MeshClient::node_init();
}
