#include<bits/stdc++.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int block_size = 8;

vector<int> padding(vector<int> data){
    int rem = data.size() % block_size;
    if(rem != 0){
        int pad = block_size - rem;
        data.insert(data.end(), pad, 0);
    }
    return data;
}

vector<int> encrypt_block(vector<int> data, vector<int> key){
    vector<int> result(block_size);
    for(int i = 0; i < block_size; i++)
        result[i] = data[i] ^ key[i];
    return result;
}

vector<int> increment_counter(vector<int> counter){
    for(int i = block_size - 1; i >= 0; i--){
        if(counter[i] == 0){
            counter[i] = 1;
            break;
        }
        counter[i] = 0;
    }
    return counter;
}

vector<int> CTR_encrypt(vector<int> pt,
                         vector<int> key,
                         vector<int> counter){
    vector<int> cipher;
    for(int i = 0; i < pt.size(); i += block_size){
        vector<int> block(pt.begin() + i,
                          pt.begin() + i + block_size);

        vector<int> keystream = encrypt_block(counter, key);

        for(int j = 0; j < block_size; j++)
            cipher.push_back(block[j] ^ keystream[j]);

        counter = increment_counter(counter);
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
    vector<int> counter = {0,0,0,0,0,0,0,1};

    vector<int> plaintext = {
        1,0,1,1,0,0,1,0,
        1,1,0,1,1
    };

    plaintext = padding(plaintext);
    vector<int> cipher = CTR_encrypt(plaintext, key, counter);

    int size = cipher.size();
    send(csoc, (char*)&size, sizeof(size), 0);
    send(csoc, (char*)cipher.data(), size * sizeof(int), 0);

    cout << "CTR Cipher bits sent: ";
    for(int b : cipher) cout << b;
    cout << endl;

    closesocket(csoc);
    WSACleanup();
    return 0;
}
