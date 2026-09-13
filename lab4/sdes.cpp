#include <bits/stdc++.h>
using namespace std;

// Permutations
int P10[] = {3,5,2,7,4,10,1,9,8,6};
int P8[]  = {6,3,7,4,8,5,10,9};
int IP[]  = {2,6,3,1,4,8,5,7};
int IP_INV[] = {4,1,3,5,7,2,8,6};
int EP[]  = {4,1,2,3,2,3,4,1};
int P4[]  = {2,4,3,1};

// S-boxes
int S0[4][4] = {
    {1,0,3,2},
    {3,2,1,0},
    {0,2,1,3},
    {3,1,3,2}
};

int S1[4][4] = {
    {0,1,2,3},
    {2,0,1,3},
    {3,0,1,0},
    {2,1,0,3}
};

// Helper functions
string permute(string bits, int table[], int n) {
    string res = "";
    for(int i=0;i<n;i++) res += bits[table[i]-1];
    return res;
}

string leftShift(string bits, int n) {
    return bits.substr(n) + bits.substr(0,n);
}

string xorBits(string a, string b) {
    string res="";
    for(int i=0;i<a.size();i++) res += ((a[i]-'0')^(b[i]-'0'))+'0';
    return res;
}

string sbox(string bits, int sbox[4][4]) {
    int row = (bits[0]-'0')*2 + (bits[3]-'0');
    int col = (bits[1]-'0')*2 + (bits[2]-'0');
    int val = sbox[row][col];
    string res = "";
    res += (val & 2 ? '1' : '0');
    res += (val & 1 ? '1' : '0');
    return res;
}

// Key generation
pair<string,string> generateKeys(string key10) {
    key10 = permute(key10,P10,10);
    string left = key10.substr(0,5), right = key10.substr(5,5);

    // LS-1
    left = leftShift(left,1);
    right = leftShift(right,1);
    string key1 = permute(left+right,P8,8);

    // LS-2
    left = leftShift(left,2);
    right = leftShift(right,2);
    string key2 = permute(left+right,P8,8);

    return {key1,key2};
}

string fk(string bits, string key) {
    string left = bits.substr(0,4);
    string right = bits.substr(4,4);

    string right_exp = permute(right,EP,8);
    string temp = xorBits(right_exp,key);

    string left4 = sbox(temp.substr(0,4),S0);
    string right4 = sbox(temp.substr(4,4),S1);
    string fk_out = permute(left4+right4,P4,4);

    return xorBits(left,fk_out) + right;
}

string encryptSDES(string plaintext, string key10) {
    pair<string,string> keys = generateKeys(key10);
    string key1 = keys.first;
    string key2 = keys.second;

    string ip = permute(plaintext,IP,8);
    string temp = fk(ip,key1);
    temp = temp.substr(4,4)+temp.substr(0,4); // Swap
    temp = fk(temp,key2);
    string cipher = permute(temp,IP_INV,8);
    return cipher;
}

string decryptSDES(string cipher, string key10) {
    pair<string,string> keys = generateKeys(key10);
    string key1 = keys.first;
    string key2 = keys.second;

    string ip = permute(cipher,IP,8);
    string temp = fk(ip,key2);
    temp = temp.substr(4,4)+temp.substr(0,4); // Swap
    temp = fk(temp,key1);
    string plain = permute(temp,IP_INV,8);
    return plain;
}
