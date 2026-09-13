#include <winsock2.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s,(sockaddr*)&addr,sizeof(addr));
    listen(s,3);

    cout<<"[AS] Waiting...\n";
    SOCKET client = accept(s,NULL,NULL);

    char buffer[1024]={0};
    recv(client, buffer, sizeof(buffer), 0);

    cout<<"[AS] Request: "<<buffer<<endl;

    string tgt="TGT_ABC123";
    send(client, tgt.c_str(), tgt.size(), 0);

    cout<<"[AS] Sent TGT\n";
    closesocket(client);
    WSACleanup();
}
