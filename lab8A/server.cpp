// server.cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <ctime>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using namespace std;
typedef long long ll;

/* ---------------- Modular Exponentiation ---------------- */

ll modExp(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp & 1)
            result = (__int128)result * base % mod;

        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------------- Miller Rabin ---------------- */

bool millerTest(ll d, ll n) {
    ll a = 2 + rand() % (n - 4);
    ll x = modExp(a, d, n);

    if (x == 1 || x == n - 1)
        return true;

    while (d != n - 1) {
        x = (__int128)x * x % n;
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
    while ((d & 1) == 0)
        d >>= 1;

    for (int i = 0; i < k; i++)
        if (!millerTest(d, n))
            return false;

    return true;
}

ll generatePrime() {
    while (true) {
        ll num = 100000 + rand() % 900000; // 6-digit prime
        if (isPrime(num))
            return num;
    }
}

/* ---------------- Primitive Root ---------------- */

vector<ll> findPrimeFactors(ll n) {
    vector<ll> factors;

    if (n % 2 == 0) {
        factors.push_back(2);
        while (n % 2 == 0)
            n /= 2;
    }

    for (ll i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0)
                n /= i;
        }
    }

    if (n > 2)
        factors.push_back(n);

    return factors;
}

ll generatePrimitiveRoot(ll p) {
    ll phi = p - 1;
    vector<ll> factors = findPrimeFactors(phi);

    while (true) {
        ll g = 2 + rand() % (p - 3);

        bool isPrimitive = true;

        for (ll factor : factors) {
            if (modExp(g, phi / factor, p) == 1) {
                isPrimitive = false;
                break;
            }
        }

        if (isPrimitive)
            return g;
    }
}

/* ---------------- Main ---------------- */

int main() {
    srand(time(0));

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8989);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 3);

    cout << "Waiting for client...\n";

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);

    /* -------- ElGamal Key Generation -------- */

    ll p = generatePrime();
    ll g = generatePrimitiveRoot(p);

    ll x;  // private key
    cout << "Enter private key (less than p): ";
    cin >> x;

    ll y = modExp(g, x, p);

    cout << "\nGenerated Parameters:\n";
    cout << "Prime (p): " << p << endl;
    cout << "Primitive Root (g): " << g << endl;
    cout << "Public key (y): " << y << endl;

    /* Send p g y */
    string data = to_string(p) + " " + to_string(g) + " " + to_string(y);
    send(clientSocket, data.c_str(), data.size(), 0);

    /* Receive Cipher */
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);

    ll c1, c2;
    stringstream ss(buffer);
    ss >> c1 >> c2;

    cout << "\nReceived Cipher:\n";
    cout << "c1: " << c1 << " c2: " << c2 << endl;

    /* -------- Decryption -------- */

    ll s = modExp(c1, x, p);
    ll s_inv = modExp(s, p - 2, p);  // Fermat inverse
    ll message = (c2 * s_inv) % p;

    cout << "\nDecrypted Message: " << message << endl;

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}