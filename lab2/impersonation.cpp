// attacker_impersonation.cpp
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sock;
    sockaddr_in serverAddr{};

    // ---- Attacker chosen fake identity ----
    int fakeUserID = 4;   // Attacker impersonates User 4
    int fakeToken  = 1;   // Valid token due to cycle

    /*
      Explanation:
      4^1 mod 9 = 4
      4^2 mod 9 = 7
      4^3 mod 9 = 1  <-- cycle hit
      4^4 mod 9 = 4 (repeats)
    */

    WSAStartup(MAKEWORD(2, 2), &wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // ---- Send forged credentials ----
    send(sock, (char*)&fakeUserID, sizeof(fakeUserID), 0);
    send(sock, (char*)&fakeToken, sizeof(fakeToken), 0);

    std::cout << "[ATTACK] Impersonation successful!\n";
    std::cout << "Sent User_ID = " << fakeUserID
              << " , Token = " << fakeToken << "\n";

    closesocket(sock);
    WSACleanup();
}
