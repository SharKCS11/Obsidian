#ifndef MESHCHAT_H //include guards
#define MESHACHAT_H
// Includes for Sockets
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// Includes from STL
#include <vector>



//Socket-specific definitions
/*
struct sockaddr_in { //STRUCTURE OF A SOCKET ADDRESS
    uint8_t sin_len;
    sa_family_t sin_family; //AF_INET
    in_port_t sin_port; // PORT NUMBER
    struct in_addr sin_addr; // MACHINE'S IP ADDRESS
    char sin_zero[8];
};
*/
#define SOCKET_CONN_TYPE AF_INET
#define LOCIP INADDR_ANY
#define MSGSIZE 4096


typedef struct Node{

} Node;

/* MeshClient will be used as a namespace for all
global network-related functions and data */
class MeshClient
{
public:
//Class variables (globals)
    static std::vector<Node> neighbours;
//Class functions (globals)
    static void node_init();

};



#endif // MESHCHAT_H
