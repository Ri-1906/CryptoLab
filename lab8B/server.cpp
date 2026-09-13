// server.cpp  (Homomorphic - Operation Only)

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;
typedef long long ll;

int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8989);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 3);

    cout << "Waiting for client...\n";
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);

    /* Receive public key */
    char buffer[2048] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);

    ll p, g, y;
    stringstream ss(buffer);
    ss >> p >> g >> y;

    cout << "\nReceived Public Key:";
    cout << "\np: " << p;
    cout << "\ng: " << g;
    cout << "\ny: " << y << endl;

    /* Receive ciphertexts */
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);

    ll c1, c2, d1, d2;
    stringstream ss2(buffer);
    ss2 >> c1 >> c2 >> d1 >> d2;

    cout << "\nCipher1: (" << c1 << ", " << c2 << ")";
    cout << "\nCipher2: (" << d1 << ", " << d2 << ")";

    /* Homomorphic multiplication */
    ll new_c1 = (c1 * d1) % p;
    ll new_c2 = (c2 * d2) % p;

    cout << "\n\nAfter Multiplication:";
    cout << "\nNew Cipher: (" << new_c1 << ", " << new_c2 << ")\n";

    string result = to_string(new_c1) + " " + to_string(new_c2);
    send(clientSocket, result.c_str(), result.size(), 0);

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}