#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

#define PORT 8080
#define BLOCK_SIZE 8

string key = "mysecret";

// XOR
string xor_op(string a, string b) {
    string res = "";
    for (int i = 0; i < a.size(); i++)
        res += a[i] ^ b[i % b.size()];
    return res;
}

// ECB
string ecb(string text) {
    string out = "";
    for (int i = 0; i < text.size(); i += BLOCK_SIZE)
        out += xor_op(text.substr(i, BLOCK_SIZE), key);
    return out;
}

// CBC
string cbc(string text) {
    string out = "", iv = "initvect", prev = iv;
    for (int i = 0; i < text.size(); i += BLOCK_SIZE) {
        string block = xor_op(text.substr(i, BLOCK_SIZE), prev);
        string enc = xor_op(block, key);
        out += enc;
        prev = enc;
    }
    return out;
}

// CFB
string cfb(string text) {
    string out = "", iv = "initvect", prev = iv;
    for (int i = 0; i < text.size(); i += BLOCK_SIZE) {
        string enc = xor_op(prev, key);
        string block = text.substr(i, BLOCK_SIZE);
        string cipher = xor_op(block, enc);
        out += cipher;
        prev = cipher;
    }
    return out;
}

// OFB
string ofb(string text) {
    string out = "", iv = "initvect", prev = iv;
    for (int i = 0; i < text.size(); i += BLOCK_SIZE) {
        prev = xor_op(prev, key);
        out += xor_op(text.substr(i, BLOCK_SIZE), prev);
    }
    return out;
}

// CTR
string ctr(string text) {
    string out = "";
    int counter = 1;

    for (int i = 0; i < text.size(); i += BLOCK_SIZE) {
        string ctr_block = to_string(counter++);
        string keystream = xor_op(ctr_block, key);
        out += xor_op(text.substr(i, BLOCK_SIZE), keystream);
    }
    return out;
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (SOCKADDR*)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "Server waiting...\n";

    client_socket = accept(server_fd, (SOCKADDR*)&address, &addrlen);

    recv(client_socket, buffer, 1024, 0);

    string input(buffer);
    string mode = input.substr(0, 3);
    string text = input.substr(4);

    string result;

    if (mode == "ECB") result = ecb(text);
    else if (mode == "CBC") result = cbc(text);
    else if (mode == "CFB") result = cfb(text);
    else if (mode == "OFB") result = ofb(text);
    else if (mode == "CTR") result = ctr(text);

    send(client_socket, result.c_str(), result.size(), 0);

    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();
}