 #include <iostream>
#include <winsock2.h>
#include <ctime>
#include <cstdlib>

#pragma comment(lib,"ws2_32.lib")

using namespace std;
using ll = long long;

ll power_mod(ll a, ll d, ll n) {
    ll result = 1;
    a %= n;
    while (d > 0) {
        if (d & 1)
            result = (result * a) % n;
        a = (a * a) % n;
        d >>= 1;
    }
    return result;
}

int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET s;
    sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(9999);

    connect(s, (sockaddr*)&server, sizeof(server));

    ll data[3];
    recv(s, (char*)data, sizeof(data), 0);

    ll p = data[0];
    ll g = data[1];
    ll A = data[2];

    cout << "Received Prime (p): " << p << endl;
    cout << "Received Primitive Root (g): " << g << endl;
    cout << "Received Alice Public Key (A): " << A << endl;

    // USER INPUT PRIVATE KEY
    ll b;
    cout << "Enter Bob Private Key (b < p): ";
    cin >> b;

    if (b <= 1 || b >= p) {
        cout << "Invalid private key.\n";
        return 0;
    }

    ll B = power_mod(g, b, p);
    cout << "Bob Public Key (B): " << B << endl;

    send(s, (char*)&B, sizeof(B), 0);

    ll secret = power_mod(A, b, p);
    cout << "Shared Secret Key: " << secret << endl;

    closesocket(s);
    WSACleanup();

    return 0;
}