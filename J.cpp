#include <iostream>
#include <vector>
#include <cmath>
#define mod 999999937ll
std::vector<std::vector<unsigned long long>> multiply(std::vector<std::vector<unsigned long long>>& matrix1,std::vector<std::vector<unsigned long long>>& matrix2) {
    std::vector<std::vector<unsigned long long>> ans(5, std::vector<unsigned long long> (5,0));
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 5; ++k) {
                ans[i][j] += ((matrix1[i][k] % mod) * (matrix2[k][j] % mod)) % mod;
            }
        }
    }
    return ans;
}
std::vector<std::vector<unsigned long long>> binpow(std::vector<std::vector<unsigned long long>>& matrix, unsigned long long n) {
    std::vector<std::vector<unsigned long long>> ed(5, std::vector<unsigned long long>(5, 0));
    for (int i = 0; i < 5; ++i) {
    ed[i][i] = 1;
}
    if  (n == 0) return ed;
 
    std::vector<std::vector<unsigned long long>> ans = ed;
    std::vector<std::vector<unsigned long long>> p = matrix;
    while(n) {
        if (n&1) ans = multiply(ans, p);
        p = multiply(p,p);
        n >>= 1;
    }
    return ans;
}
int main() {
 unsigned long long n;
 std::cin >> n;
 std::vector<std::vector<unsigned long long>> matrix(5, std::vector<unsigned long long> (5,1));
 matrix[2][3] = 0;
 matrix[2][4] = 0;
 matrix[4][3] = 0;
 matrix[4][4] = 0;
 while (n) {
     auto ans = binpow(matrix, n);
     unsigned long long res = 0;
     for (int i = 0; i < 5; ++i) {
         res += ans[0][i] % mod;
     }
    std::cout << res % mod << std::endl;
    std::cin >> n;
 }
    return 0;
}
