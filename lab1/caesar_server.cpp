#include <winsock2.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

string decrypt(string text, int shift) {
    for (char &c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base - shift + 26) % 26 + base;
        }
    }
    return text;
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&server, sizeof(server));
    listen(serverSocket, 3);

    cout << "Server waiting...\n";

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);

    cout << "Encrypted message received: " << buffer << endl;

    int shift;
    cout << "Enter shift for decryption: ";
    cin >> shift;

    string decrypted = decrypt(buffer, shift);

    cout << "Decrypted message: " << decrypted << endl;

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}
