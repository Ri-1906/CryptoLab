#include<bits/stdc++.h>
#include<winsock2.h>
using namespace std;
int block_size = 8;
vector<int> decrypt_block(vector<int> data,vector<int> key){
    vector<int> result(block_size);
    for(int i = 0 ; i < block_size ; i++){
        result[i] = data[i] ^ key[i];
    }
    return result;
}

vector<int> decrypt(vector<int> cipher,vector<int> key){
    vector<int> pt;
    for(int i = 0 ; i < cipher.size() ; i+= block_size){
        vector<int> block(cipher.begin() + i,cipher.begin() + i + block_size);
        vector<int> enc = decrypt_block(block,key);
        for(int i : enc) cout<<i;
        cout<<" ";
        pt.insert(pt.end(),enc.begin(),enc.end());
    }
    return pt; 
}

int main(){
    WSADATA wsa;
    SOCKET ssoc,csoc;
    sockaddr_in saddr,caddr;
    int clientSize = sizeof(caddr);

    WSAStartup(MAKEWORD(2,2), &wsa);
    ssoc = socket(AF_INET,SOCK_STREAM,0);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(8989);

    bind(ssoc,(sockaddr*) &saddr, sizeof(saddr));
    listen(ssoc,3);
    cout<<"Server is listeing..."<<endl;
    csoc = accept(ssoc,(sockaddr*) &caddr,&clientSize);
    cout<<"Connected!!!"<<endl;
    
    int size;
    recv(csoc,(char*) &size,sizeof(size), 0);
    vector<int> cipher(size);
    recv(csoc,(char *) cipher.data(),size * sizeof(int), 0);
    vector<int> key = {1,1,0,0,1,0,1,1};
    for(int i: cipher) cout<<i;
    cout<<endl;
    vector<int> pt = decrypt(cipher,key);

    closesocket(ssoc);
    closesocket(csoc);
    WSACleanup();
    return 0;
}