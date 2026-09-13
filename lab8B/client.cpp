// client.cpp  (Correct Homomorphic ElGamal)

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <vector>
#include <ctime>

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
        ll num = 100000 + rand() % 900000;
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
        bool ok = true;

        for (ll factor : factors) {
            if (modExp(g, phi / factor, p) == 1) {
                ok = false;
                break;
            }
        }

        if (ok)
            return g;
    }
}

/* ---------------- MAIN ---------------- */

int main() {

    srand(time(0));

    /* -------- Key Generation -------- */

    ll p = generatePrime();
    ll g = generatePrimitiveRoot(p);

    ll x;
    cout << "Enter private key (< p): ";
    cin >> x;

    ll y = modExp(g, x, p);

    cout << "\nGenerated Keys:";
    cout << "\np: " << p;
    cout << "\ng (primitive root): " << g;
    cout << "\ny: " << y << endl;

    /* -------- Connect to Server -------- */

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    /* Send public key */
    string params = to_string(p) + " " + to_string(g) + " " + to_string(y);
    send(sock, params.c_str(), params.size(), 0);

    /* -------- Encrypt -------- */

    ll m1, m2;
    cout << "\nEnter message1 (< p): ";
    cin >> m1;

    cout << "Enter message2 (< p): ";
    cin >> m2;

    ll k1 = 2 + rand() % (p - 2);
    ll k2 = 2 + rand() % (p - 2);

    ll c1 = modExp(g, k1, p);
    ll c2 = (m1 * modExp(y, k1, p)) % p;

    ll d1 = modExp(g, k2, p);
    ll d2 = (m2 * modExp(y, k2, p)) % p;

    string cipher = to_string(c1) + " " + to_string(c2) + " "
                  + to_string(d1) + " " + to_string(d2);

    send(sock, cipher.c_str(), cipher.size(), 0);

    /* -------- Receive Result -------- */

    char buffer[1024] = {0};
    recv(sock, buffer, sizeof(buffer), 0);

    ll new_c1, new_c2;
    stringstream ss(buffer);
    ss >> new_c1 >> new_c2;

    /* -------- Decrypt -------- */

    ll s = modExp(new_c1, x, p);
    ll s_inv = modExp(s, p - 2, p);
    ll result = (new_c2 * s_inv) % p;

    cout << "\nDecrypted Result (m1*m2 mod p): "
         << result << endl;

    closesocket(sock);
    WSACleanup();
}