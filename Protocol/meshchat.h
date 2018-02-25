#ifndef MESHCHAT_H //include guards
#define MESHACHAT_H
//C includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Includes for Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// Includes from STL
#include <vector>
#include <thread>
#include <queue>
#include <chrono>

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
#define SUBNET_MASK 0xC0A80100
#define MSGSIZE 4096

constexpr uint32_t BROADCAST_IP = SUBNET_MASK | 0xFF;

//Pass nodes by value everywhere.
typedef struct{
    sockaddr_in address;
} Node;

typedef std::chrono::milliseconds ms_t;

typedef struct{
    unsigned long long timestamp;
    std::string text;
} Message;

/* MeshClient will be used as a namespace for all
global network-related functions and data */
class MeshClient
{
public:
//Class variables (globals)
    static std::vector<Node> neighbours;
    static int udsock;
    static sockaddr_in loc_addr;
    static std::chrono::steady_clock CLK;
//Class functions (globals)
    static void node_init();
    static void node_init(Node connection);
    static void listen();
private:
    static void setup_socket();
    static void getUserInput();

};



#endif // MESHCHAT_H
