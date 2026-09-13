#include <winsock2.h>
#include <iostream>
#include <cmath>

#pragma comment(lib, "ws2_32.lib")

int modexp(int base, int exp, int mod) {
    int result = 1;
    for (int i = 0; i < exp; i++)
        result = (result * base) % mod;
    return result;
}

int main() {
    WSADATA wsa;
    SOCKET server, client;
    sockaddr_in serverAddr{}, clientAddr{};
    int clientSize = sizeof(clientAddr);

    int n = 9;   // Composite (VULNERABLE)
    int k = 7;   // Secret key

    WSAStartup(MAKEWORD(2, 2), &wsa);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(server, 1);

    std::cout << "Server listening...\n";
    client = accept(server, (sockaddr*)&clientAddr, &clientSize);

    int userID, token;
    recv(client, (char*)&userID, sizeof(userID), 0);
    recv(client, (char*)&token, sizeof(token), 0);

    int expected = modexp(userID, k, n);

    if (expected == token)
        std::cout << "Authentication SUCCESS\n";
    else
        std::cout << "Authentication FAILED\n";

    closesocket(client);
    closesocket(server);
    WSACleanup();
}
