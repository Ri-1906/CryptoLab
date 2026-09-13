#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>
#include "sdes.cpp"
#pragma comment(lib,"ws2_32.lib")

using namespace std;

/* Split received string */
vector<string> split(string data, char delim) {
    vector<string> tokens;
    string temp = "";

    for (char c : data) {
        if (c == delim) {
            if (!temp.empty())
                tokens.push_back(temp);
            temp = "";
        } else {
            temp += c;
        }
    }
    return tokens;
}

int main() {
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[4096] = {0};

    WSAStartup(MAKEWORD(2,2), &wsa);
    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    bind(s, (struct sockaddr*)&server, sizeof(server));
    listen(s, 3);

    cout << "Waiting for client...\n";
    c = sizeof(struct sockaddr_in);
    new_socket = accept(s, (struct sockaddr*)&client, &c);
    cout << "Client connected.\n";

    recv(new_socket, buffer, 4096, 0);
    string received(buffer);

    string key10;
    cout << "\nEnter 10-bit key for decryption: ";
    cin >> key10;

    vector<string> cipherBlocks = split(received, '|');

    cout << "\nServer Decryption:\n";
    for (auto &cipher : cipherBlocks) {
        if (cipher.length() == 8) {
            string plain = decryptSDES(cipher, key10);
            cout << "Cipher: " << cipher
                 << "  -> Plain: " << plain << endl;
        }
    }

    closesocket(new_socket);
    closesocket(s);
    WSACleanup();
    return 0;
}
