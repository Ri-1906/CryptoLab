#include<bits/stdc++.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int block_size = 8;

// -------- XOR BLOCK CIPHER --------
vector<int> encrypt_block(vector<int> data, vector<int> key){
    vector<int> result(block_size);
    for(int i = 0; i < block_size; i++)
        result[i] = data[i] ^ key[i];
    return result;
}

// -------- INCREMENT COUNTER --------
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

// -------- CTR DECRYPT --------
vector<int> CTR_decrypt(vector<int> cipher,
                         vector<int> key,
                         vector<int> counter){
    vector<int> plaintext;
    for(int i = 0; i < cipher.size(); i += block_size){
        vector<int> block(cipher.begin() + i,
                          cipher.begin() + i + block_size);

        vector<int> keystream = encrypt_block(counter, key);

        for(int j = 0; j < block_size; j++)
            plaintext.push_back(block[j] ^ keystream[j]);

        counter = increment_counter(counter);
    }
    return plaintext;
}

int main(){
    WSADATA wsa;
    SOCKET ssoc, csoc;
    sockaddr_in saddr, caddr;
    int clientSize = sizeof(caddr);

    WSAStartup(MAKEWORD(2,2), &wsa);

    ssoc = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(8989);

    bind(ssoc, (sockaddr*)&saddr, sizeof(saddr));
    listen(ssoc, 3);

    cout << "Server listening...\n";
    csoc = accept(ssoc, (sockaddr*)&caddr, &clientSize);
    cout << "Client connected!\n";

    int size;
    recv(csoc, (char*)&size, sizeof(size), 0);

    vector<int> cipher(size);
    recv(csoc, (char*)cipher.data(), size * sizeof(int), 0);

    vector<int> key = {1,1,0,0,1,0,1,1};
    vector<int> counter = {0,0,0,0,0,0,0,1};

    vector<int> plaintext = CTR_decrypt(cipher, key, counter);

    cout << "Received CTR Cipher bits: ";
    for(int b : cipher) cout << b;
    cout << "\nDecrypted Plaintext bits: ";
    for(int b : plaintext) cout << b;
    cout << endl;

    closesocket(csoc);
    closesocket(ssoc);
    WSACleanup();
    return 0;
}
