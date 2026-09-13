#include <winsock2.h>
#include <iostream>
#include "miller_rabin2.cpp"
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    SOCKET sock;
    sockaddr_in serverAddr;

    WSAStartup(MAKEWORD(2,2), &wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(7777);

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    long long pubkey[2];
    recv(sock, (char*)pubkey, sizeof(pubkey), 0);
  
    long long e = pubkey[0];
    long long n = pubkey[1];

    cout << "Received Public Key (e,n): " << e << "," << n << endl;

    long long msg;
    cout << "Enter message (number < n): ";
    cin >> msg;

    long long cipher = power(msg, e, n);
    cout << "Encrypted Cipher: " << cipher << endl;

    send(sock, (char*)&cipher, sizeof(cipher), 0);

    closesocket(sock);
    WSACleanup();
}
