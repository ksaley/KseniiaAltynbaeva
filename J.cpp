#include <iostream>
#include <vector>
#include <map>
 
void euler(std::vector<std::pair<int, int>>& answer, std::map<std::pair<int, int>, bool>& check, std::vector<std::vector<int>> &table,
           std::vector<int> &ptr, int vertex, int prev = -1) {
    while (ptr[vertex] != table[vertex].size()) {
        std::pair<int, int> e = {vertex,table[vertex][ptr[vertex]]};
        if (check[e]) {
            ++ptr[vertex];
            continue;
        }
        int u = e.second;
        check[e] = true;
        ++ptr[vertex];
        euler(answer, check,table, ptr, u, e.first);
    }
    if (prev != - 1) answer.push_back({vertex, prev});
}
 
std::vector<std::pair<int, int>> solve(std::vector<std::vector<int>>& table, int vertex, int cur_pos) {
    std::vector<int> ptr(vertex + 1, 0);
    std::map<std::pair<int, int>, bool> check;
    std::vector<std::pair<int, int>> answer;
    euler(answer, check,table,ptr, cur_pos);
    return answer;
}
 
int main() {
    int vertex, cur_pos;
    std::cin >> vertex >> cur_pos;
    std::vector<std::vector<int>> table(vertex + 1);
    for (int i = 1; i <= vertex; ++i) {
        for (int j = 1; j <= vertex; ++j) {
            bool x;
            std::cin >> x;
            if (j == i) continue;
            if(!x) table[i].push_back(j);
        }
    }
    
    auto answer = solve(table, vertex, cur_pos);
    for (int  i = answer.size() -1; i >=0; --i) {
        std::cout << answer[i].second << " " << answer[i].first << std::endl;
    }
    return 0;
}
