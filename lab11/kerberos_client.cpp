#include <winsock2.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

SOCKET connectTo(int port){
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(s,(sockaddr*)&addr,sizeof(addr));
    return s;
}

int main(){
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET as = connectTo(8000);
    string req="LOGIN";
    send(as, req.c_str(), req.size(), 0);

    char tgt[1024]={0};
    recv(as, tgt, sizeof(tgt), 0);
    cout<<"TGT: "<<tgt<<endl;
    closesocket(as);

    SOCKET tgs = connectTo(8001);
    send(tgs, tgt, sizeof(tgt), 0);

    char ticket[1024]={0};
    recv(tgs, ticket, sizeof(ticket), 0);
    cout<<"Service Ticket: "<<ticket<<endl;
    closesocket(tgs);

    SOCKET srv = connectTo(8002);
    send(srv, ticket, sizeof(ticket), 0);

    char res[1024]={0};
    recv(srv, res, sizeof(res), 0);
    cout<<"Server Response: "<<res<<endl;

    closesocket(srv);
    WSACleanup();
}
