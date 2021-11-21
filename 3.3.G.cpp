#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
 
using base = std::complex<long double>;
const long double PI = acos(-1);
 
void fourier_transform(std::vector<base>& c, bool invert,
                       const std::vector<base>& ws, size_t log = 0) {
    size_t half = c.size() / 2;
    if (half < 1) return;
 
    std::vector<base> c1(half), c2(half);
    for (size_t i = 0; i < half; ++i) {
        c1[i] = c[2 * i];
        c2[i] = c[2 * i + 1];
    }
 
    fourier_transform(c1, invert, ws, log + 1);
    fourier_transform(c2, invert, ws, log + 1);
 
    base w1 = ws[log];
    if (invert) w1 = base(w1.real(), -w1.imag());
    base wi = 1;
 
    for (size_t i = 0; i < half; ++i) {
        c[i] = c1[i] + wi * c2[i];
        c[i + half] = c1[i] - wi * c2[i];
 
        wi *= w1;
    }
}
 
std::vector<long long> multiply(std::vector<long long>& a, std::vector<long long>& b) {
    size_t n = 1;
    std::vector<base> ws(1, 1);
    while (n < 2 * a.size() || n < 2 * b.size()) {
        n *= 2;
        long double angel = 2 * PI / n;
        ws.push_back(base(cos(angel), sin(angel)));
    }
    for (size_t i = 0; i < ws.size() / 2; ++i) {
        std::swap(ws[i], ws[ws.size() - 1 - i]);
    }
    std::vector<base> c1(n, 0), c2(n, 0);
    for (size_t i = 0; i < a.size(); ++i) {
        c1[i] = a[i];
    }
    for (size_t i = 0; i < b.size(); ++i) {
        c2[i] = b[i];
    }
 
    fourier_transform(c1, false, ws);
    fourier_transform(c2, false, ws);
 
    for (size_t i = 0; i < n; ++i) {
        c1[i] *= c2[i];
    }
 
    fourier_transform(c1, true, ws);
 
    std::vector<long long> result(n, 0);
    for (size_t i = 0; i < n; ++i) {
        c1[i] /= n;
        result[i] = static_cast<long long>(c1[i].real() + (c1[i].real()>=0?0.5:-0.5));
    }
 
    return result;
}
 
int main() {
    long long n, m;
    std::cin >> n;
    std::vector<long long> a(n + 1);
    for ( auto& i : a)  {
        std::cin >> i;
    }
    std::cin >> m;
    std::vector<long long> b(m + 1);
    for ( auto& i : b)  {
        std::cin >> i;
    }
    auto result = multiply(a,b);
    result.resize(n + m + 1);
    std::cout << n + m << " ";
    for (auto& i : result) {
        std::cout << i << " ";
    }
}
