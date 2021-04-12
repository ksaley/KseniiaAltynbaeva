#include <iostream>
#include <vector>
 
std::vector <std::vector<bool>> build_ok(int n) {
    std::vector <std::vector<bool>> ok(1 << n, std::vector<bool>(1<< n, false));
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
int implement_dp(int n, int m) {
    auto ok = build_ok(n);
    std::vector<std::vector<long long>> dp(m + 1, std::vector<long long> (1<< n,0));
    for (int i = 0; i < (1 << n); ++i) {
        dp[1][i] = 1;
    }
    for(int j = 1; j < m ; ++j) {
        for (int mask = 0; mask < (1 << n); ++mask) {
            for (int mask1 = 0; mask1 <  (1<< n); ++mask1) {
                if (ok[mask][mask1]) {
                    dp[j+1][mask1]+=dp[j][mask];
                }
            }
        }
    }
    long long ans = 0;
    for (int  i = 0; i < (1 << n); ++i) {
        ans += dp[m][i];
    }
    return ans;
}
 
int main() {
    int n, m;
    std::cin >> n >> m;
    if (m < n) std::swap(m,n);
    auto ans = implement_dp(n, m);
    std::cout << ans;
    return 0;
}
