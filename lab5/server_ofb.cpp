#include<bits/stdc++.h>
#include<winsock2.h>
using namespace std;

int block_size = 8;

/* XOR Block Cipher */
vector<int> encrypt_block(vector<int> data, vector<int> key){
    vector<int> res(block_size);
    for(int i = 0; i < block_size; i++)
        res[i] = data[i] ^ key[i];
    return res;
}

/* OFB Decryption */
vector<int> decrypt_OFB(vector<int> cipher, vector<int> key, vector<int> iv){
    vector<int> pt;
    vector<int> feedback = iv;

    for(int i = 0; i < cipher.size(); i += block_size){
        vector<int> block(cipher.begin() + i, cipher.begin() + i + block_size);

        feedback = encrypt_block(feedback, key);   // same keystream

        vector<int> p(block_size);
        for(int j = 0; j < block_size; j++)
            p[j] = block[j] ^ feedback[j];

        pt.insert(pt.end(), p.begin(), p.end());
    }
    return pt;
}

int main(){
    WSADATA wsa;
    SOCKET ssoc, csoc;
    sockaddr_in saddr, caddr;
    int clen = sizeof(caddr);

    WSAStartup(MAKEWORD(2,2), &wsa);
    ssoc = socket(AF_INET, SOCK_STREAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(8989);

    bind(ssoc, (sockaddr*)&saddr, sizeof(saddr));
    listen(ssoc, 3);

    cout << "Server listening...\n";
    csoc = accept(ssoc, (sockaddr*)&caddr, &clen);

    int size;
    recv(csoc, (char*)&size, sizeof(size), 0);

    vector<int> cipher(size);
    recv(csoc, (char*)cipher.data(), size * sizeof(int), 0);

    vector<int> key = {1,1,0,0,1,0,1,1};
    vector<int> iv  = {0,1,1,0,0,1,0,1};

    vector<int> pt = decrypt_OFB(cipher, key, iv);

    cout << "Decrypted bits:\n";
    for(int b : pt) cout << b;
    cout << endl;

    closesocket(csoc);
    closesocket(ssoc);
    WSACleanup();
    return 0;
}
