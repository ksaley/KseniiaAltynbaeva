#include <iostream>
#include <vector>
#include <string>
#include <complex>
using std::vector;
 
class BigInteger {
public:
    BigInteger();
    BigInteger(int x);
    BigInteger operator-() const;
 
    BigInteger& operator+=(const BigInteger& x);
    BigInteger& operator-=(const BigInteger& x);
    BigInteger& operator*=(const BigInteger& x);
    BigInteger& operator/=(const BigInteger& x);
    BigInteger& operator%=(const BigInteger& x);
 
    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);
 
    std::string toString() const;
    explicit operator bool();
 
private:
    inline static const size_t DIGIT_LOG = 4;
    inline static const unsigned int MAX_DIGIT = 10'000;
 
    vector<unsigned int> value;
    bool negative;
 
    using base = std::complex<double>;
    inline static const double PI = acos(-1);
 
    inline static const size_t MAX_SMALL_NUMBER_LOG = 10;
 
    BigInteger& positive_add(const BigInteger& x);
    BigInteger& positive_subtract(const BigInteger& x);
    bool positive_less_or_equal(const BigInteger& x) const;
    void normalize();
    void fourier_transform(vector<base>& c, bool invert, const vector<base>& ws, size_t log);
 
    friend bool operator==(const BigInteger& a, const BigInteger& b);
    friend bool operator<=(const BigInteger& a, const BigInteger& b);
    friend std::istream& operator>>(std::istream& in, BigInteger& s);
};
 
BigInteger operator+(const BigInteger& a, const BigInteger& b);
BigInteger operator-(const BigInteger& a, const BigInteger& b);
BigInteger operator*(const BigInteger& a, const BigInteger& b);
BigInteger operator/(const BigInteger& a, const BigInteger& b);
BigInteger operator%(const BigInteger& a, const BigInteger& b);
 
bool operator==(const BigInteger& a, const BigInteger& b);
bool operator!=(const BigInteger& a, const BigInteger& b);
bool operator<=(const BigInteger& a, const BigInteger& b);
bool operator>=(const BigInteger& a, const BigInteger& b);
bool operator<(const BigInteger& a, const BigInteger& b);
bool operator>(const BigInteger& a, const BigInteger& b);
 
std::ostream& operator<<(std::ostream& out, const BigInteger& x);
std::istream& operator>>(std::istream& in, BigInteger& x);
 
void BigInteger::normalize() {
    while (value.size() > 1 && value.back() == 0) {
        value.pop_back();
    }
    if (value.empty()) value.push_back(0);
    if (value.size() == 1 && value[0] == 0) negative = false;
}
 
BigInteger::BigInteger(): value(vector<unsigned int>(1, 0)), negative(false) {};
 
BigInteger::BigInteger(int x): value(vector<unsigned int>(1, 0)),
                               negative(x < 0) {
    if (x < 0) x = -x;
    for (size_t i = 0; x > 0; ++i) {
        if (value.size() <= i) value.push_back(0);
        value[i] = static_cast<unsigned int>(x % MAX_DIGIT);
        x /= MAX_DIGIT;
    }
    normalize();
}
 
BigInteger& BigInteger::positive_add(const BigInteger& x) {
    long long sum = 0;
    for (size_t i = 0; ; ++i) {
        if (i < x.value.size())
            sum += x.value[i];
        if (i < value.size())
            sum += value[i];
        if (sum == 0 && i >= value.size() && i >= x.value.size()) break;
        if (value.size() <= i) value.push_back(0);
        value[i] = static_cast<unsigned int>(sum % MAX_DIGIT);
        sum /= MAX_DIGIT;
    }
    normalize();
    return *this;
}
 
BigInteger& BigInteger::positive_subtract(const BigInteger& x) {
    bool bigger = !positive_less_or_equal(x);
 
    const BigInteger* a = this;
    const BigInteger* b = &x;
 
    if (!bigger) {
        negative ^= true;
        std::swap(a, b);
    }
 
    long long sum = 0;
    for (size_t i = 0; i < a->value.size(); ++i) {
        sum += a->value[i];
        if (i < b->value.size()) {
            sum -= b->value[i];
        }
        if (sum == value[i] && a == this && i >= b->value.size()) {
            break;
        }
        long long next_sum = 0;
        if (sum < 0) {
            next_sum = (sum - MAX_DIGIT + 1) / MAX_DIGIT;
            sum += MAX_DIGIT;
        }
        if (value.size() <= i) value.push_back(0);
        value[i] = static_cast<unsigned int>(sum % MAX_DIGIT);
        sum = next_sum;
    }
 
    normalize();
    return *this;
}
 
BigInteger BigInteger::operator-() const {
    BigInteger negative_x = *this;
    negative_x.negative ^= true;
    negative_x.normalize();
    return negative_x;
}
 
BigInteger& BigInteger::operator+=(const BigInteger& x) {
    return (negative == x.negative) ? positive_add(x) : positive_subtract(x);
}
 
BigInteger& BigInteger::operator-=(const BigInteger& x) {
    return (negative == x.negative) ? positive_subtract(x) : positive_add(x);
}
 
void BigInteger::fourier_transform(vector<base>& c, bool invert,
                                   const vector<base>& ws, size_t log = 0) {
    size_t half_n = c.size() / 2;
    if (half_n < 1) return;
 
    vector<base> c1(half_n), c2(half_n);
    for (size_t i = 0; i < half_n; ++i) {
        c1[i] = c[2 * i];
        c2[i] = c[2 * i + 1];
    }
 
    fourier_transform(c1, invert, ws, log + 1);
    fourier_transform(c2, invert, ws, log + 1);
 
    base w1 = ws[log];
    if (invert) w1 = base(w1.real(), -w1.imag());
    base wi = 1;
 
    for (size_t i = 0; i < half_n; ++i) {
        c[i] = c1[i] + wi * c2[i];
        c[i + half_n] = c1[i] - wi * c2[i];
 
        wi *= w1;
    }
}
 
BigInteger& BigInteger::operator*=(const BigInteger& x) {
    if (std::min(value.size(), x.value.size()) <= MAX_SMALL_NUMBER_LOG) {
        const BigInteger* a = this;
        const BigInteger* b = &x;
 
        if (*this < x) std::swap(a, b);
        BigInteger result = 0;
 
        for (size_t i = 0; i < b->value.size(); ++i) {
            if (b->value[i] == 0) continue;
            BigInteger temp = 0;
            temp.value.resize(i, 0);
            long long sum = 0;
            for (size_t j = 0; ; ++j) {
                if (j < a->value.size()) {
                    sum += 1ll * a->value[j] * b->value[i];
                }
                if (sum == 0 && j >= a->value.size()) break;
                temp.value.push_back(static_cast<unsigned int>(sum % MAX_DIGIT));
                sum /= MAX_DIGIT;
            }
            temp.normalize();
            result += temp;
        }
 
        result.negative = a->negative ^ b->negative;
        result.normalize();
        *this = result;
        return *this;
    }
 
    size_t n = 1;
    vector<base> ws(1, 1);
    while (n < 2 * value.size() || n < 2 * x.value.size()) {
        n *= 2;
        double angel = 2 * PI / n;
        ws.push_back(base(cos(angel), sin(angel)));
    }
    for (size_t i = 0; i < ws.size() / 2; ++i) {
        std::swap(ws[i], ws[ws.size() - 1 - i]);
    }
    vector<base> c1(n, 0), c2(n, 0);
    for (size_t i = 0; i < value.size(); ++i) {
        c1[i] = value[i];
    }
    for (size_t i = 0; i < x.value.size(); ++i) {
        c2[i] = x.value[i];
    }
 
    fourier_transform(c1, false, ws);
    fourier_transform(c2, false, ws);
 
    for (size_t i = 0; i < n; ++i) {
        c1[i] *= c2[i];
    }
 
    fourier_transform(c1, true, ws);
 
    long long sum = 0;
    value.resize(n, 0);
    for (size_t i = 0; i < n; ++i) {
        c1[i] /= n;
        sum += static_cast<long long>(c1[i].real() + 0.5);
        value[i] = static_cast<unsigned int>(sum % MAX_DIGIT);
        sum /= MAX_DIGIT;
    }
 
    negative ^= x.negative;
    normalize();
    return *this;
}
 
BigInteger& BigInteger::operator/=(const BigInteger& x) {
    vector<unsigned int> result;
    BigInteger temp = 0;
    for (size_t i = value.size(); i > 0; ) {
        --i;
        temp = temp * MAX_DIGIT + value[i];
        unsigned int left = 0, right = MAX_DIGIT + 1;
        while (right - left > 1) {
            unsigned int middle = (left + right) / 2;
            if ((x * middle).positive_less_or_equal(temp)) {
                left = middle;
            } else {
                right = middle;
            }
        }
        result.push_back(left);
        temp.positive_subtract(left * x);
    }
 
    for (size_t i = 0; i < result.size() / 2; ++i) {
        std::swap(result[i], result[result.size() - 1 - i]);
    }
    value = result;
    negative ^= x.negative;
    normalize();
    return *this;
}
 
BigInteger& BigInteger::operator%=(const BigInteger& x) {
    return *this -= *this / x * x;
}
 
BigInteger operator+(const BigInteger& a, const BigInteger& b) {
    BigInteger result = a;
    result += b;
    return result;
}
 
BigInteger operator-(const  BigInteger& a, const BigInteger& b) {
    BigInteger result = a;
    result -= b;
    return result;
}
 
BigInteger operator*(const BigInteger& a, const BigInteger& b) {
    BigInteger result = a;
    result *= b;
    return result;
}
 
BigInteger operator/(const BigInteger& a, const BigInteger& b) {
    BigInteger result = a;
    result /= b;
    return result;
}
 
BigInteger operator%(const BigInteger& a, const BigInteger& b) {
    BigInteger result = a;
    result %= b;
    return result;
}
 
BigInteger& BigInteger::operator++() {
    return *this += 1;
}
 
BigInteger BigInteger::operator++(int) {
    BigInteger copy = *this;
    ++*this;
    return copy;
}
 
BigInteger& BigInteger::operator--() {
    return *this -= 1;
}
 
BigInteger BigInteger::operator--(int) {
    BigInteger copy = *this;
    --*this;
    return copy;
}
 
std::string BigInteger::toString() const {
    std::string number = "";
    for (size_t i = 0; i < value.size(); ++i) {
        unsigned int digit = value[i];
        for (size_t j = 0; j < DIGIT_LOG; ++j) {
            number += static_cast<char>(digit % 10 + 48);
            digit /= 10;
            if (i + 1 == value.size() && digit == 0) break;
        }
    }
    if (negative) number += '-';
    for (size_t i = 0; i < number.size() / 2; ++i) {
        std::swap(number[i], number[number.size() - i - 1]);
    }
    return number;
}
 
bool BigInteger::positive_less_or_equal(const BigInteger& x) const {
    if (value.size() != x.value.size()) {
        return value.size() < x.value.size();
    }
    for (size_t i = value.size(); i > 0; ) {
        --i;
        if (value[i] < x.value[i]) return true;
        if (value[i] > x.value[i]) return false;
    }
    return true;
}
 
bool operator==(const BigInteger& a, const BigInteger& b) {
    if (a.value.size() != b.value.size() || a.negative != b.negative)
        return false;
    for (size_t i = 0; i < a.value.size(); ++i) {
        if (a.value[i] != b.value[i]) return false;
    }
    return true;
}
 
bool operator!=(const BigInteger& a, const BigInteger& b) {
    return !(a == b);
}
 
bool operator<=(const BigInteger& a, const BigInteger& b) {
    if (a.negative != b.negative) return a.negative;
    return a.negative ? b.positive_less_or_equal(a) : a.positive_less_or_equal(b);
}
 
bool operator>=(const BigInteger& a, const BigInteger& b) {
    return b <= a;
}
 
bool operator<(const BigInteger& a, const BigInteger& b) {
    return a <= b && a != b;
}
 
bool operator>(const BigInteger& a, const BigInteger& b) {
    return b < a;
}
 
BigInteger::operator bool() {
    return *this != BigInteger(0);
}
 
std::ostream& operator<<(std::ostream& out, const BigInteger& x) {
    out << x.toString();
    return out;
}
 
std::istream& operator>>(std::istream& in, BigInteger& x) {
    std::string s;
    in >> s;
    x = 0;
    x.negative = s.size() > 0 && s[0] == '-';
    for (size_t i = 0; i < s.size() / 2; ++i) {
        std::swap(s[i], s[s.size() - 1 - i]);
    }
    while (s.size() > 1 && s.back() == '0') {
        s.pop_back();
    }
    x.value = vector<unsigned int>(0);
    unsigned int power = 1;
    for (size_t i = 0; i < s.size() && s[i] != '-'; ++i) {
        if (i % BigInteger::DIGIT_LOG == 0) {
            power = 1;
            x.value.push_back(0);
        } else {
            power *= 10;
        }
        x.value.back() += power * (static_cast<int>(s[i]) - 48);
    }
    x.normalize();
    return in;
}
 
std::vector<std::vector<unsigned long long>> multiply(std::vector<std::vector<unsigned long long>>& matrix1,
                                                      std::vector<std::vector<unsigned long long>>& matrix2,
                                                      int mod) {
    std::vector<std::vector<unsigned long long>> ans(matrix1.size(), std::vector<unsigned long long> (matrix2[0].size(),0));
    for (int i = 0; i < matrix1.size(); ++i) {
        for (int j = 0; j < matrix2[0].size(); ++j) {
            for (int k = 0; k < matrix2.size(); ++k) {
                ans[i][j] += ((matrix1[i][k] % mod) * (matrix2[k][j]  % mod)) ;
            }
        }
    }
    return ans;
}
std::vector<std::vector<unsigned long long>> binpow(std::vector<std::vector<unsigned long long>>& matrix, BigInteger n, int mod) {
    std::vector<std::vector<unsigned long long>> ed(matrix.size(), std::vector<unsigned long long>(matrix.size(), 0));
    for (int i = 0; i < matrix.size(); ++i) {
        ed[i][i] = 1;
    }
    if  (n == 0) return ed;
 
    std::vector<std::vector<unsigned long long>> ans = ed;
    std::vector<std::vector<unsigned long long>> p = matrix;
    while(n) {
        if (int(n.toString().back())&1) ans = multiply(ans, p, mod);
        p = multiply(p,p, mod);
        n /= 2;
    }
    return ans;
}
std::vector <std::vector<unsigned long long>> build_ok(int n) {
    std::vector <std::vector<unsigned long long>> ok(1 << n, std::vector<unsigned long long>(1<< n, false));
    for (int i = 0; i < 1 << n; ++i) {
        for(int j = i; j < 1 << n; ++j) {
            bool flag = true;
            for (int k = 1; k < n; ++k) {
                bool black_check = ((i >> k) & 1) && ((i >> (k - 1)) & 1) && ((j >> k) & 1) && ((j >> (k-1)) & 1);
                bool white_check = ((i >> k) & 1) || ((i >> (k - 1)) & 1) || ((j >> k) & 1) || ((j >> (k-1)) & 1);
                if (black_check or !white_check) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                ok[i][j] = true;
                ok[j][i] = true;
            }
        }
    }
    return ok;
}
 
int implement_dp(int n, int mod, BigInteger& m) {
    auto ok = build_ok(n);
    ok = binpow(ok, m - 1, mod);
    std::vector<std::vector<unsigned long long>> dp(1<< n, std::vector<unsigned long long> (1));
    for (int i = 0; i < (1 << n); ++i) {
        dp[i][0] = 1;
    }
    auto res = multiply(ok,dp, mod);
    long long ans = 0;
    for (int i = 0; i < 1<<n; ++i) {
        ans +=res[i][0];
    }
    return ans % mod;
}
int main() {
    int n, mod;
    BigInteger m;
    std::cin >> m >> n >> mod;
    auto ans = implement_dp(n, mod, m);
    std::cout << ans % mod;
    return 0;
}
