#include<bits/stdc++.h>
#include<winsock2.h>
using namespace std;

int block_size = 8;

/* Zero Padding */
vector<int> padding(vector<int> data){
    int rem = data.size() % block_size;
    if(rem != 0){
        int pad = block_size - rem;
        data.insert(data.end(), pad, 0);
    }
    return data;
}

/* XOR Block Cipher */
vector<int> encrypt_block(vector<int> data, vector<int> key){
    vector<int> res(block_size);
    for(int i = 0; i < block_size; i++)
        res[i] = data[i] ^ key[i];
    return res;
}

/* CFB Encryption */
vector<int> encrypt_CFB(vector<int> pt, vector<int> key, vector<int> iv){
    vector<int> cipher;
    vector<int> feedback = iv;

    for(int i = 0; i < pt.size(); i += block_size){
        vector<int> block(pt.begin() + i, pt.begin() + i + block_size);

        vector<int> enc = encrypt_block(feedback, key);

        vector<int> c(block_size);
        for(int j = 0; j < block_size; j++)
            c[j] = block[j] ^ enc[j];

        cipher.insert(cipher.end(), c.begin(), c.end());
        feedback = c;
    }
    return cipher;
}

int main(){
    WSADATA wsa;
    SOCKET csoc;
    sockaddr_in saddr;

    WSAStartup(MAKEWORD(2,2), &wsa);
    csoc = socket(AF_INET, SOCK_STREAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(8989);

    connect(csoc, (sockaddr*)&saddr, sizeof(saddr));

    vector<int> key = {1,1,0,0,1,0,1,1};
    vector<int> iv  = {0,1,1,0,0,1,0,1};

    vector<int> plaintext = {
        1,0,1,1,0,0,1,0,
        1,1,0,1,1
    };

    plaintext = padding(plaintext);
    vector<int> cipher = encrypt_CFB(plaintext, key, iv);

    int size = cipher.size();
    send(csoc, (char*)&size, sizeof(size), 0);
    send(csoc, (char*)cipher.data(), size * sizeof(int), 0);

    closesocket(csoc);
    WSACleanup();
    return 0;
}
