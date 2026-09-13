// client.cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

using namespace std;
typedef long long ll;

/* Modular Exponentiation */
ll modExp(ll base, ll exp, ll mod) {
    ll res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            res = (__int128)res * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

int main() {
    srand(time(0));

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    /* Receive p g y */
    char buffer[1024] = {0};
    recv(sock, buffer, sizeof(buffer), 0);

    ll p, g, y;
    stringstream ss(buffer);
    ss >> p >> g >> y;

    cout << "Received Parameters:\n";
    cout << "Prime (p): " << p << endl;
    cout << "Generator (g): " << g << endl;
    cout << "Public key (y): " << y << endl;

    /* Input Message */
    ll message;
    cout << "\nEnter numeric message (< p): ";
    cin >> message;

    /* Select random k */
    ll k = 2 + rand() % (p - 2);

    /* Encryption */
    ll c1 = modExp(g, k, p);
    ll c2 = (message * modExp(y, k, p)) % p;

    cout << "\nGenerated Cipher:\n";
    cout << "c1: " << c1 << " c2: " << c2 << endl;

    /* Send Cipher */
    string cipher = to_string(c1) + " " + to_string(c2);
    send(sock, cipher.c_str(), cipher.size(), 0);

    closesocket(sock);
    WSACleanup();

    return 0;
}