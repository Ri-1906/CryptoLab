#include <iostream>
#include <winsock2.h>
#include <cmath>
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

// Miller-Rabin Test
bool millerTest(ll d, ll n) {
    ll a = 2 + rand() % (n - 4);
    ll x = power_mod(a, d, n);

    if (x == 1 || x == n - 1)
        return true;

    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;

        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

bool isPrime(ll n, int k = 5) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    ll d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < k; i++)
        if (!millerTest(d, n))
            return false;

    return true;
}

ll generatePrime() {
    while (true) {
        ll num = 1000 + rand() % 9000;
        if (isPrime(num))
            return num;
    }
}

// Primitive root finder
ll findPrimitiveRoot(ll p) {
    ll phi = p - 1;

    for (ll g = 2; g < p; g++) {
        bool flag = false;
        for (ll i = 2; i <= sqrt(phi); i++) {
            if (phi % i == 0) {
                if (power_mod(g, phi / i, p) == 1 ||
                    power_mod(g, i, p) == 1) {
                    flag = true;
                    break;
                }
            }
        }
        if (!flag)
            return g;
    }
    return -1;
}

int main() {

    srand(time(0));

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_socket, client_socket;
    sockaddr_in server, client;
    int c = sizeof(sockaddr_in);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    bind(server_socket, (sockaddr*)&server, sizeof(server));
    listen(server_socket, 3);

    cout << "Waiting for connection...\n";
    client_socket = accept(server_socket, (sockaddr*)&client, &c);
    cout << "Client connected.\n";

    // ll p = generatePrime();
    ll p = 5099;
    // ll g = findPrimitiveRoot(p);
    ll g = 2;

    cout << "Generated Prime (p): " << p << endl;
    cout << "Primitive Root (g): " << g << endl;

    // USER INPUT PRIVATE KEY
    ll a;
    cout << "Enter Alice Private Key (a < p): ";
    cin >> a;

    if (a <= 1 || a >= p) {
        cout << "Invalid private key.\n";
        return 0;
    }

    ll A = power_mod(g, a, p);
    cout << "Alice Public Key (A): " << A << endl;

    ll data[3] = {p, g, A};
    send(client_socket, (char*)data, sizeof(data), 0);

    ll B;
    recv(client_socket, (char*)&B, sizeof(B), 0);

    cout << "Received Bob Public Key (B): " << B << endl;

    ll secret = power_mod(B, a, p);
    cout << "Shared Secret Key: " << secret << endl;

    closesocket(server_socket);
    WSACleanup();

    return 0;
}