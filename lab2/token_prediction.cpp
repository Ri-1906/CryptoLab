// attacker_predict.cpp
#include <iostream>

int modexp(int base, int exp, int mod) {
    int r = 1;
    for (int i = 0; i < exp; i++)
        r = (r * base) % mod;
    return r;
}

int main() {
    int userID = 2;
    int n = 9;

    for (int k = 1; k <= 15; k++) {
        std::cout << "k=" << k
                  << " Token=" << modexp(userID, k, n) << "\n";
    }
}
