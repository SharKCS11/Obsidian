#include <iostream>
#include <fstream>
#include "meshchat.h"

using namespace std;

/***** MESHCLIENT GLOBAL VARIABLES ******/
string MeshClient::name;
int MeshClient::udsock;
struct sockaddr_in MeshClient::loc_addr;
struct sockaddr_in MeshClient::outbound_addr;
priority_queue<Message*, vector<Message*>, MsgComp > MeshClient::msgque;
mutex MeshClient::quelock;
ofstream MeshClient::handle_log;

/***** MESHCLIENT STATIC FUNCTIONS ******/
void MeshClient::node_init(string name_in)
{
    handle_log.open("hlog.txt");
    name = name_in;
    cout << "Setting up node for " << MeshClient::name <<endl;
    setup_socket();

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
    /*if( !(outgoing=socket(SOCKET_CONN_TYPE, SOCK_DGRAM, 0)) )
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
    }*/
    outbound_addr.sin_family = AF_INET;
    //outbound_addr.sin_addr.s_addr = htonl(BROADCAST_IP);
    outbound_addr.sin_addr.s_addr = inet_addr("192.168.1.255");
    outbound_addr.sin_port = htons(MAGICPORT); //OS assigns port.
    int on=1;
    setsockopt(udsock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

}

//THREAD MAIN: GETTING USER INPUT
void MeshClient::getUserInput()
{ //runs as the main thread.
    Message entry_msg;
    sprintf(entry_msg.text, "%d %s has joined chat.", 1, MeshClient::name.c_str());
    entry_msg.timestamp = getTime();
    if( sendto(udsock, &entry_msg, sizeof(entry_msg), 0,
               (struct sockaddr *)&outbound_addr, sizeof(outbound_addr)) < 0 )
	{
		perror("Sendto first failed.");
		return;
	}
    string line;
    while(getline(cin, line)){
        if(line.size() + 2 + name.size() >= MSGBUFFSIZE)
        {
            perror("Message too long!");
            continue;
        }
        sprintf(entry_msg.text, "%d %s %s", 1, name.c_str(), line.c_str());
        entry_msg.timestamp = getTime();
        if( sendto(udsock, &entry_msg, sizeof(entry_msg), 0,
               (struct sockaddr *)&outbound_addr, sizeof(outbound_addr)) < 0 )
        {
            perror("Sendto failed.");
            return;
        }
    }
}

// THREAD 2: LISTENING FOR UDP
void MeshClient::listen()
{
    ofstream llog("llog.txt");
    Message rec;
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
        int recv_status = recvfrom(udsock, &rec, sizeof(rec), 0, (struct sockaddr *)&remote_addr, &addrlen);
        if(recv_status < 0)
        {
            perror("Receive failed.");
            return;
        }
        else
        {
            quelock.lock();
            msgque.push(new Message(rec));
            quelock.unlock();
            thread req_t(MeshClient::request_handler);
            req_t.detach();
        }
    }
}

//THREAD 3: Handling requests.
void MeshClient::request_handler()
{
    //cout << "Called request handler" <<endl;
    handle_log << "Called request handler" <<endl;
    quelock.lock();
    while(!msgque.empty()){
        Message* head = msgque.top();
        //cout << "stuff" <<endl;
        //cout << head->text << endl;
        handle_log << head->text << endl;
        msgque.pop();
        char tsbuf[50];
        getFormattedTimeStamp(tsbuf, head->timestamp);
        if(head->text[0] == '1') //message
        {
            int opcode;
            char sender[25];
            char body[MSGBUFFSIZE-25];
            sscanf(head->text,"%d %s %2040[^\n]%s",&opcode, sender, body);
            handle_log << tsbuf << " " << sender << ": " << body <<endl;
        }
        delete head;

    }
    quelock.unlock();
}


unsigned long long MeshClient::getTime()
{
    unsigned long long ms = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
    ).count();
    return ms;
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
int main(int argc, char** argv)
{
    MeshClient::node_init(string(argv[1]));
}
