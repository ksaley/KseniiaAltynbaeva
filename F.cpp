#include <iostream>
#include <vector>

int build_dp(int n, int m, std::vector <int>& first, std::vector <int>& second) {
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m+1,0));
    for (int i = 1; i <= n; ++i) {
        for(int j = 1; j <= m; ++j) {
            if (first[i] == second[j]) {
                dp[i][j] = std::max(std::max(1+dp[i-1][j-1],dp[i-1][j]),dp[i][j-1]);
            }
            else {
                dp[i][j] = std::max(dp[i-1][j],dp[i][j-1]);
            }
        }
    }
    return dp[n][m];
}

int main() {
    int n;
    std::cin >> n;
    std::vector <int> first(n + 1);
    for ( int i = 1; i <= n; ++i) {
        std::cin >> first[i];
    }
    
    int m;
    std::cin >> m;
    std::vector <int> second(m+1);
    for ( int i =1; i <= m; ++i) {
        std::cin >> second[i];
    }
    std::cout << build_dp(n, m, first, second);
    return 0;
}
