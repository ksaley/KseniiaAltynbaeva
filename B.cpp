#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

void print(std::vector<long long> n) {
    for (int i = 0; i < n.size(); ++i) {
        std::cout << n[i] << " ";
    }
    std::cout << std::endl;
}

std::vector <long long>  build_dp(int amount, std::vector<long long>& input) {
    long long inf = std::pow(2, 31) - 1;
    std::reverse(input.rbegin(), input.rend());
    std::vector <long long> dp(amount + 1, inf), pos(amount + 1, -1), prev(amount + 1, -1);
    dp[0] = -inf;
    for (int i = 0; i < amount; i++) {
        int j = int (std::upper_bound(dp.begin(), dp.end(), input[i]) - dp.begin());
        if (dp[j-1] <= input[i] && (input[i] <= dp[j])) {
            dp[j] = input[i];
            pos[j] = i;
            prev[i] = pos[j-1];
        }
    }

    int ans_pos = amount;
    while (dp[ans_pos] == inf) {
        --ans_pos;
    }

    std::vector <long long> ans;
    int cur_position = pos[ans_pos];

    while (cur_position != -1) {
        ans.push_back(amount - cur_position);
        cur_position = prev[cur_position];
    }
    return ans;
}

int main() {
    int amount;
    std::cin >> amount;
    std::vector <long long> input (amount);
    for (int i = 0; i < amount; ++i) {
        std::cin >> input[i];
    }
   auto ans = build_dp(amount, input);
    std::cout << ans.size() << std::endl;
    for (int i = 0; i  < ans.size(); ++i) {
        std::cout << ans[i] << " ";
    }
    return 0;
}
