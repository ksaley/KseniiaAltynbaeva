#include <iostream>
#include <vector>

std::pair<int, std::vector<int>> implement_dp(int n, std::vector<std::vector<int>>& matrix) {
    std::vector<std::vector<std::pair<long long, int>>> dp(n, std::vector<std::pair<long long, int>>
                                                                (1<<n,{1000000000, -1}));
    for (int i =0; i < n; ++i) {
        dp[i][1<<i].first = 0;
    }
    for(int mask = 0; mask < (1<<n) ; ++mask) {
        for(int v = 0; v < n; ++v) {
            for (int u = 0; u < n; ++u) {
                if ((mask>>u) & 1) continue;
                int newmask = mask | (1<<u);
                dp[u][newmask].first = std::min(dp[u][newmask].first, dp[v][mask].first + matrix[v][u]);
                if (dp[u][newmask].first == dp[v][mask].first + matrix[v][u]) dp[u][newmask].second = v;
            }
        }
    }

    int ans = 0;
    for(int i = 0; i < n; ++i) {
        if (dp[ans][(1 << n) - 1].first > dp[i][(1 << n) - 1].first) {
            ans = i;
        }
    }
    int count =  dp[ans][(1<<n) -1].first;
    std::vector<int> ret;
    long long mask = (1<< n) - 1;
    while (true) {
        ret.push_back(ans+1);
        int bit = (1<<ans);
        ans = dp[ans][mask].second;
        mask = (mask | bit) ^ bit; ;
        if (ans == -1) break;
    }
    return {count,ret};
}

int main() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> matrix(n, std::vector<int> (n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> matrix[i][j];
        }
    }
    auto ans = implement_dp(n, matrix);
    std::cout << ans.first << "\n";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }
    return 0;
}
