#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>

using base = std::complex<long double>;
const long double PI = acos(-1);

void fourier_transform(std::vector<base>& coefficients, bool invert,
                       const std::vector<base>& ws, size_t log = 0) {
    size_t half = coefficients.size() / 2;
    if (half < 1) return;

    std::vector<base> coefficients1(half), coefficients2(half);
    for (size_t i = 0; i < half; ++i) {
        coefficients1[i] = coefficients[2 * i];
        coefficients2[i] = coefficients[2 * i + 1];
    }

    fourier_transform(coefficients1, invert, ws, log + 1);
    fourier_transform(coefficients2, invert, ws, log + 1);

    base w1 = ws[log];
    if (invert) w1 = base(w1.real(), -w1.imag());
    base wi = 1;

    for (size_t i = 0; i < half; ++i) {
        coefficients[i] = coefficients1[i] + wi * coefficients2[i];
        coefficients[i + half] = coefficients1[i] - wi * coefficients2[i];

        wi *= w1;
    }
}

std::vector<long long> multiply(std::vector<long long>& a_coefs, std::vector<long long>& b_coefs) {
    size_t n = 1;
    std::vector<base> ws(1, 1);
    while (n < 2 * a_coefs.size() || n < 2 * b_coefs.size()) {
        n *= 2;
        long double angel = 2 * PI / n;
        ws.push_back(base(cos(angel), sin(angel)));
    }
    for (size_t i = 0; i < ws.size() / 2; ++i) {
        std::swap(ws[i], ws[ws.size() - 1 - i]);
    }
    std::vector<base> c1(n, 0), c2(n, 0);
    for (size_t i = 0; i < a_coefs.size(); ++i) {
        c1[i] = a_coefs[i];
    }
    for (size_t i = 0; i < b_coefs.size(); ++i) {
        c2[i] = b_coefs[i];
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
    std::vector<long long> a_coefs(n + 1);
    for ( auto& i : a_coefs)  {
        std::cin >> i;
    }
    std::cin >> m;
    std::vector<long long> b_coefs(m + 1);
    for ( auto& i : b_coefs)  {
        std::cin >> i;
    }
    auto result = multiply(a_coefs, b_coefs);
    result.resize(n + m + 1);
    std::cout << n + m << " ";
    for (auto& i : result) {
        std::cout << i << " ";
    }
}
