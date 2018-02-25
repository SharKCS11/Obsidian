#include <iostream>
#include <fstream>
#include "meshchat.h"

using namespace std;

/***** MESHCLIENT GLOBAL VARIABLES ******/
vector<Node> MeshClient::neighbours;
int MeshClient::udsock;
struct sockaddr_in MeshClient::loc_addr;

/***** MESHCLIENT STATIC FUNCTIONS ******/
void MeshClient::node_init()
{
    setup_socket();
    unsigned long long ms = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
    ).count();
    char tsstr[250];
    getFormattedTimeStamp(tsstr,ms);
    cout << tsstr << endl;
    thread t_listen(MeshClient::listen);
    t_listen.detach();
    cout << "Thread t is listening." <<endl;
    getUserInput();
}



void MeshClient::setup_socket()
{
    //UDSOCK
    if( !(udsock=socket(SOCKET_CONN_TYPE, SOCK_DGRAM, 0)) )
    {
        perror("Error creating socket.");
        return;
    }
    memset((char *)&loc_addr, 0, sizeof(loc_addr));
    loc_addr.sin_family = AF_INET;
    loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    loc_addr.sin_port = htons(MAGICPORT); //OS assigns port.
    if(bind(udsock, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) != 0)
    {
        perror("bind failed");
        return;
    }
    //SENDING SOCKADDR
    //UDSOCK
    if( !(outgoing=socket(SOCKET_CONN_TYPE, SOCK_DGRAM, 0)) )
    {
        perror("Error creating socket.");
        return;
    }
    memset((char *)&loc_addr, 0, sizeof(loc_addr));
    loc_addr.sin_family = AF_INET;
    loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    loc_addr.sin_port = htons(MAGICPORT); //OS assigns port.
    if(bind(udsock, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) != 0)
    {
        perror("bind failed");
        return;
    }
    outbound_addr.sin_family = AF_INET;
    outbound_addr.sin_addr.s_addr = htonl(BROADCAST_IP);
    outbound_addr.sin_port = htons(MAGICPORT); //OS assigns port.

}

//THREAD MAIN: GETTING USER INPUT
void MeshClient::getUserInput()
{ //runs as the main thread.
    string line;
    while(getline(cin, line)){
        cout << "Got line: " << line << endl;
    }
}

// THREAD 2: LISTENING FOR UDP
void MeshClient::listen()
{
    ofstream llog("llog.txt");
    char rec_buffer[MSGSIZE];
    struct sockaddr_in remote_addr;
    uint32_t loclen = sizeof(loc_addr), addrlen;
    if(getsockname(udsock, (struct sockaddr*)&loc_addr, &loclen) != 0)
    {
        perror("Error getting socket name bad.");
        return;
    }
    llog << "Listening on port " << ntohs(loc_addr.sin_port) <<endl;
    //receive packets.
    while(1){
        llog << "asdasdads" <<endl;
        int recv_status = recvfrom(udsock, rec_buffer, MSGSIZE, 0, (struct sockaddr *)&remote_addr, &addrlen);
        if(recv_status < 0)
        {
            perror("Receive failed.");
            return;
        }
        printf("Received message: %s\n", rec_buffer);
    }
}

void MeshClient::getFormattedTimeStamp(char* buf, unsigned long long timecount)
{
    int mils = timecount % 1000;
    timecount /= 1000;
    auto date = gmtime((const time_t*)&timecount);
    char month_names[][4] = {"Jan","Feb","Mar","Apr","May","Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    buf += sprintf(buf, "%d-",1900+date->tm_year);
    buf += sprintf(buf, "%s-",month_names[date->tm_mon]);
    buf += sprintf(buf, "%d",date->tm_mday);
    buf += sprintf(buf, "@@%d:%d:%d.%d", date->tm_hour, date->tm_min, date->tm_sec, mils);
}


/****** MAIN ******/
int main()
{
    MeshClient::node_init();
}
