#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>
#include "sdes.cpp"
#pragma comment(lib,"ws2_32.lib")

using namespace std;

/* Split plaintext into 8-bit blocks with padding */
vector<string> splitBlocks(string plaintext) {
    vector<string> blocks;

    for (int i = 0; i < plaintext.length(); i += 8) {
        string block = plaintext.substr(i, 8);
        while (block.length() < 8)
            block += '0';   // padding
        blocks.push_back(block);
    }
    return blocks;
}

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    WSAStartup(MAKEWORD(2,2), &wsa);
    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    connect(s, (struct sockaddr*)&server, sizeof(server));

    string plaintext, key10;
    cout << "Enter plaintext bits: ";
    cin >> plaintext;

    cout << "Enter 10-bit key: ";
    cin >> key10;

    vector<string> blocks = splitBlocks(plaintext);

    string sendData = "";

    cout << "\nClient Encryption:\n";
    for (auto &block : blocks) {
        string cipher = encryptSDES(block, key10);
        cout << "Plain: " << block
             << "  -> Cipher: " << cipher << endl;

        sendData += cipher + "|";
    }

    send(s, sendData.c_str(), sendData.length(), 0);

    closesocket(s);
    WSACleanup();
    return 0;
}
