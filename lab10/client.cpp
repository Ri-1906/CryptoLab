// ===== CLIENT.cpp =====
#include <winsock2.h>
#include <iostream>
#include "common_crypto.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void printDivider() {
    cout << "\n==================================================\n";
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8989);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    ll data[6];
    recv(sock, (char*)data, sizeof(data), 0);

    printDivider();
    cout << "[CLIENT] 📥 RAW DATA RECEIVED\n";

    for (int i = 0; i < 6; i++) {
        cout << "data[" << i << "] = " << data[i] << endl;
    }

    ll p = data[0];
    ll g = data[1];
    ll y = data[2];
    ll m = data[3];
    ll r = data[4];
    ll s = data[5];

    printDivider();
    cout << "[CLIENT] 🔍 INTERPRETED PARAMETERS\n";

    cout << "Prime (p)        = " << p << endl;
    cout << "Generator (g)    = " << g << endl;
    cout << "Public key (y)   = " << y << endl;
    cout << "Message hash (m) = " << m << endl;
    cout << "Signature r      = " << r << endl;
    cout << "Signature s      = " << s << endl;

    printDivider();
    cout << "[CLIENT] 🧮 VERIFICATION STEPS\n";

    ll v1 = modexp(g, m, p);
    ll v2_part1 = modexp(y, r, p);
    ll v2_part2 = modexp(r, s, p);
    ll v2 = (v2_part1 * v2_part2) % p;

    cout << "Step 1: g^m mod p           = " << v1 << endl;
    cout << "Step 2: y^r mod p           = " << v2_part1 << endl;
    cout << "Step 3: r^s mod p           = " << v2_part2 << endl;
    cout << "Step 4: (y^r * r^s) mod p   = " << v2 << endl;

    printDivider();
    cout << "[CLIENT] ✅ FINAL RESULT\n";

    if (v1 == v2)
        cout << "Signature VALID ✔️\n";
    else
        cout << "Signature INVALID ❌\n";

    closesocket(sock);
    WSACleanup();

    return 0;
}