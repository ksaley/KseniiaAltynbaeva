#include <iostream>
#include <vector>
#include <cmath>

long long mod = 1e9 + 7;

long long bpow(long long a, long long n) {
    if (!n) return 1;
    if (n % 2) return bpow(a, n - 1) * a % mod;
    auto t = bpow(a, n / 2) % mod;
    return t * t % mod;

}

void gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return;
    }
    gcd(b, a % b, x, y);
    long long x1 = y;
    long long y1 = x - (a / b) * (y % mod) % mod;
    x = x1;
    y = y1;
}

long long factorial(long long n, long long k = 0) {
    static std::vector<long long> facts(2 * 1e5, 0);
    facts[0] = 1;
    facts[1] = 1;
    if (facts[n]) return facts[n];
    facts[n] = (n * factorial(n - 1)) % mod;
    return facts[n];
}

long long stirling(long long n, long long k) {
    long long sum = 0;
    for (long long j = 0; j <= k; ++j) {

        long long x, y;
        gcd(mod, (factorial(j) % mod), x, y);
        long long invertFactorial1 = (y + mod) % mod;

        gcd(mod, (factorial(k - j) % mod), x, y);
        long long invertFactorial2 = (y + mod) % mod;

        long long one = (((long long) std::pow(-1, j + k) + mod) % mod);

        sum += (one * (factorial(k) % mod) % mod
                * invertFactorial1 % mod
                * invertFactorial2 % mod
                * (bpow(j, n) % mod)) % mod;
        sum %= mod;
    }
    long long x, y;

    gcd(mod, (factorial(k) % mod), x, y);

    long long invertFactorial = (y + mod) % mod;
    sum *= invertFactorial;
    sum %= mod;
    return sum;
}

inline long long get_coefficient(long long n, long long k) {
    return (stirling(n, k) + (n - 1) * ((stirling(n - 1, k) % mod)) % mod) % mod;

}

long long implement_solution(long long n, long long k, long long sum) {
    sum %= mod;
    return sum * get_coefficient(n, k) % mod;
}

int main() {
    long long n, k;
    std::cin >> n >> k;
    std::vector<long long> a(n);
    long long sum = 0;
    for (auto &i : a) {
        std::cin >> i;
        sum += i;
    }
    std::cout << implement_solution(n, k, sum);
}
