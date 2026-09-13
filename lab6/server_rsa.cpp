#include <winsock2.h>
#include <iostream>
#include <ctime>
#include "rsa_utils.cpp"
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2,2), &wsa);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(7777);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    cout << "Server waiting...\n";
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

    srand(time(0));

    // RSA key generation
    long long p = generatePrime();
    long long q = generatePrime();
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    long long e;
    do {
        e = rand() % phi + 2;
    } while (gcd(e, phi) != 1);

    long long d = modInverse(e, phi);

    cout << "p = " << p << " q = " << q << endl;
    cout << "Public Key (e,n): (" << e << "," << n << ")\n";
    cout << "Private Key (d,n): (" << d << "," << n << ")\n";

    // Send public key
    long long pubkey[2] = {e, n};
    send(clientSocket, (char*)pubkey, sizeof(pubkey), 0);

    // Receive cipher
    long long cipher;
    recv(clientSocket, (char*)&cipher, sizeof(cipher), 0);

    cout << "Received Cipher: " << cipher << endl;

    // Decrypt
    long long message = power(cipher, d, n);
    cout << "Decrypted Message: " << message << endl;

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}
