#include <iostream>
#include <vector>
 
int find_answer(const std::vector<int>& dp) {
    int answer = dp.size() - 1;
    while(dp[answer] != 1) {
        --answer;
    }
    return answer;
}
void dynamic_prog (std::vector<int>& dp,const std::vector<int>& weight) {
    std::vector<int> dp_copy = dp;
    for (int i = 1; i < weight.size(); ++i){
        for (int j = 0; j < dp.size(); ++j) {
            if (dp[j] == 1 && j + weight[i] < dp.size()) {
                dp_copy[j + weight[i]] = 1;
            }
        }
        dp = dp_copy;
    }
}
int main() {
    int capacity, numbers;
    std::cin >> capacity >> numbers;
    std::vector <int> weight(numbers + 1);
    for (int i = 1; i <= numbers; ++i) {
        std::cin >> weight[i];
    }
    std::vector<int> dp(capacity + 1, 0);
    dp[0] = 1;
    dynamic_prog(dp, weight);
    std::cout << find_answer(dp);
    return 0;
}
