// client.cpp

#include <winsock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

struct Share
{
    long long x;
    long long y;
};

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2),&wsa);

    SOCKET s=socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(9000);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    connect(s,(sockaddr*)&server,sizeof(server));

    Share share;
    long long prime;

    recv(s,(char*)&share,sizeof(Share),0);
    recv(s,(char*)&prime,sizeof(long long),0);

    cout<<"Received share: "<<share.x<<" "<<share.y<<endl;
    cout<<"Prime: "<<prime<<endl;

    closesocket(s);

    cout<<"Send share back? (1=yes)\n";
    int choice;
    cin>>choice;

    if(choice==1)
    {
        SOCKET s2=socket(AF_INET,SOCK_STREAM,0);
        connect(s2,(sockaddr*)&server,sizeof(server));

        send(s2,(char*)&share,sizeof(Share),0);

        closesocket(s2);
    }

    WSACleanup();
}