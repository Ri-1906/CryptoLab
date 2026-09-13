#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

#define PORT 8080

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock;
    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));

    string mode, text;

    cout << "Enter mode (ECB/CBC/CFB/OFB/CTR): ";
    cin >> mode;
    cin.ignore();

    cout << "Enter plaintext: ";
    getline(cin, text);

    string message = mode + " " + text;

    send(sock, message.c_str(), message.size(), 0);

    recv(sock, buffer, 1024, 0);

    cout << "Ciphertext: " << buffer << endl;

    closesocket(sock);
    WSACleanup();
}