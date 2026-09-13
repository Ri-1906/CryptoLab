// ===== SERVER.cpp =====
#include <winsock2.h>
#include <iostream>
#include <iomanip>
#include <limits>
#include "common_crypto.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void printDivider() {
    cout << "\n==================================================\n";
}

int main() {
    srand(time(0));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        cout << "WSAStartup failed\n";
        return 1;
    }

    SOCKET server_fd, client_socket;
    sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8989);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "[SERVER] Waiting for connection on port 8989...\n";
    client_socket = accept(server_fd, (sockaddr*)&address, &addrlen);

    printDivider();
    cout << "[SERVER] 🔐 KEY GENERATION\n";

    ll p = generatePrime();
    ll g = 2 + rand() % (p - 3);

    ll x = 2 + rand() % (p - 2);   // private key
    ll y = modexp(g, x, p);        // public key

    cout << "Prime (p)        = " << p << endl;
    cout << "Generator (g)    = " << g << endl;
    cout << "Private key (x)  = " << x << endl;
    cout << "Public key (y)   = " << y << endl;

    printDivider();
    cout << "[SERVER] 📝 MESSAGE INPUT\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string message;
    cout << "Enter message: ";
    getline(cin, message);

    // Hash of message (used for signature)
    ll m = 0;
    for (char c : message) m += c;

    cout << "\n[SERVER] 🧾 HASH COMPUTED\n";
    cout << "Message = " << message << endl;
    cout << "Hash (m) = " << m << endl;

    printDivider();
    cout << "[SERVER] ✍️ SIGNATURE GENERATION\n";

    ll k;
    do {
        k = 2 + rand() % (p - 2);
    } while (gcd(k, p - 1) != 1);

    ll r = modexp(g, k, p);
    ll k_inv = modInverse(k, p - 1);

    ll s = (k_inv * (m - x * r)) % (p - 1);
    if (s < 0) s += (p - 1);

    cout << "Random k         = " << k << endl;
    cout << "k inverse        = " << k_inv << endl;
    cout << "Signature r      = " << r << endl;
    cout << "Signature s      = " << s << endl;

    printDivider();
    cout << "[SERVER] 📦 DATA BLOCK SENT\n";

    ll data[6] = {p, g, y, m, r, s};

    cout << "[0] p = " << data[0] << endl;
    cout << "[1] g = " << data[1] << endl;
    cout << "[2] y = " << data[2] << endl;
    cout << "[3] m = " << data[3] << endl;
    cout << "[4] r = " << data[4] << endl;
    cout << "[5] s = " << data[5] << endl;

    send(client_socket, (char*)data, sizeof(data), 0);

    cout << "\n[SERVER] ✅ Data sent successfully\n";

    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}