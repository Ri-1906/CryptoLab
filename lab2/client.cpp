// client.cpp
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int modexp(int base, int exp, int mod) {
    int result = 1;
    for (int i = 0; i < exp; i++)
        result = (result * base) % mod;
    return result;
}

int main() {
    WSADATA wsa;
    SOCKET sock;
    sockaddr_in serverAddr{};

    int n = 9;
    int k = 7;
    int userID = 2;
    int token = modexp(userID, k, n);

    WSAStartup(MAKEWORD(2, 2), &wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    send(sock, (char*)&userID, sizeof(userID), 0);
    send(sock, (char*)&token, sizeof(token), 0);

    std::cout << "Sent User_ID & Token\n";

    closesocket(sock);
    WSACleanup();
}
