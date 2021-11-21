#include <iostream>
#include <vector>
#include <numeric>

long long sieveOfEratosthenes(long long n) {
    std::vector<long long> primes;
    std::vector<long long> mind(n + 1);
    std::iota(mind.begin(), mind.end(), 0);
    long long ans = 0;
    for ( long long i = 2; i <= n; ++i) {
        if (mind[i] == i) {
            primes.push_back(i);
        }
        for (int p = 0; p < primes.size() && primes[p] * i <= n && primes[p] <= mind[i]; ++p) {
            mind[primes[p] * i] = primes[p];
        }
    }
    for ( int i = 3; i <= n; ++i) {
        if (mind[i] == i) continue;
        ans += mind[i];
    }
    return ans;
}

int main() {
    long long n;
    std::cin >> n;
    std::cout << sieveOfEratosthenes(n);
}
