#include <winsock2.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

string encrypt(string text, int shift) {
    for (char &c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift) % 26 + base;
        }
    }
    return text;
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (sockaddr*)&server, sizeof(server));

    string message;
    int shift;

    cout << "Enter message: ";
    getline(cin, message);

    cout << "Enter shift: ";
    cin >> shift;

    string encrypted = encrypt(message, shift);

    cout << "Encrypted message: " << encrypted << endl;

    send(clientSocket, encrypted.c_str(), encrypted.size(), 0);

    closesocket(clientSocket);
    WSACleanup();
}
