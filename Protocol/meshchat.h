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
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <functional>

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
#define MSGBUFFSIZE 2040
#define MSGSIZE 2048
#define MAGICPORT 50001

constexpr uint32_t BROADCAST_IP = SUBNET_MASK | 0xFF;


typedef std::chrono::milliseconds ms_t;


typedef struct Message{
    char text[MSGBUFFSIZE];
    unsigned long long timestamp;
    //Default ctor
    Message(){}
    //Copy ctor
    Message(Message &oth)
    :timestamp(oth.timestamp)
    {
        memcpy(this->text, oth.text, MSGBUFFSIZE);
    }
} Message;

/* MeshClient will be used as a namespace for all
global network-related functions and data */

class MsgComp
{
public:
    bool operator()(Message* lhs, Message* rhs)
    {
        return lhs->timestamp > rhs->timestamp;
    }
};

class MeshClient
{
public:
//Class variables (globals)
    static std::string name;
    static int udsock; //listening socket
    static int outgoing; //sending socket
    static sockaddr_in loc_addr; //listening address
    static sockaddr_in outbound_addr; //sending address
    static std::priority_queue<Message*, std::vector<Message*>, MsgComp > msgque;
    static std::mutex quelock;
    static std::ofstream handle_log;
//Class functions (globals)
    static void node_init(std::string name_in);
private:
    //threaded/blocking functions
    static void listen();
    static void getUserInput();
    static void request_handler();
    //other
    static void setup_socket();
    static unsigned long long getTime();
    static void getFormattedTimeStamp(char* buf, unsigned long long timecount);

};

/** UDP Protocol Description:
Every message will be preceded by a numeric opcode.
1 - Message
2 - Leave
3 - Shutdown whole network

Message format will be: "<op> <name> <msg>" with a total of up to 2048B.
**/

#endif // MESHCHAT_H
