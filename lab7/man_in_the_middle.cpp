 #include <iostream>
#include <winsock2.h>

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

    // ----------------------------
    // CONNECT TO ALICE (port 8888)
    // ----------------------------
    SOCKET alice_socket;
    sockaddr_in alice;

    alice_socket = socket(AF_INET, SOCK_STREAM, 0);

    alice.sin_family = AF_INET;
    alice.sin_addr.s_addr = inet_addr("127.0.0.1");
    alice.sin_port = htons(8888);

    connect(alice_socket, (sockaddr*)&alice, sizeof(alice));
    cout << "Connected to Alice\n";

    // --------------------------------
    // ACT AS SERVER FOR BOB (9999)
    // --------------------------------
    SOCKET server_socket, bob_socket;
    sockaddr_in server, bob;
    int c = sizeof(sockaddr_in);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9999);

    bind(server_socket, (sockaddr*)&server, sizeof(server));
    listen(server_socket, 1);

    cout << "Waiting for Bob...\n";
    bob_socket = accept(server_socket, (sockaddr*)&bob, &c);
    cout << "Bob connected\n";

    // ----------------------------
    // RECEIVE p, g, A FROM ALICE
    // ----------------------------
    ll data[3];
    recv(alice_socket, (char*)data, sizeof(data), 0);

    ll p = data[0];
    ll g = data[1];
    ll A = data[2];

    cout << "\nIntercepted from Alice:\n";
    cout << "p = " << p << endl;
    cout << "g = " << g << endl;
    cout << "A = " << A << endl;

    // Attacker private key
    ll m;
    cout << "\nEnter Attacker Private Key (m): ";
    cin >> m;

    ll M = power_mod(g, m, p);

    cout << "Attacker Public Key (M): " << M << endl;

    // SEND M TO BOB (instead of A)
    ll fake_data[3] = {p, g, M};
    send(bob_socket, (char*)fake_data, sizeof(fake_data), 0);

    // RECEIVE B FROM BOB
    ll B;
    recv(bob_socket, (char*)&B, sizeof(B), 0);

    cout << "\nIntercepted Bob Public Key (B): " << B << endl;

    // SEND M TO ALICE (instead of B)
    send(alice_socket, (char*)&M, sizeof(M), 0);

    // ----------------------------
    // COMPUTE BOTH SECRETS
    // ----------------------------
    ll secret_with_alice = power_mod(A, m, p);
    ll secret_with_bob   = power_mod(B, m, p);

    cout << "\nSecret with Alice: " << secret_with_alice << endl;
    cout << "Secret with Bob:   " << secret_with_bob << endl;

    closesocket(server_socket);
    closesocket(alice_socket);
    WSACleanup();

    return 0;
}