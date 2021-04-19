#include <iostream>
#include <vector>
#include <string>

void dfs(std::pair<int, int>& res, std::vector<std::vector<int>>& vertex,  std::vector<std::string>& color,   std::vector<int>& par, int v, int p = -1) {
    par[v] = p;
    color[v] = "GRAY";
    for (auto to : vertex[v]) {
        if(color[to] == "GRAY")  {
            res = {v, to};
        }
        if (color[to] != "WHITE") continue;
        dfs(res,vertex, color, par, to, v);
    }
    color[v] = "BLACK";
}

std::vector<int> solve(int vertex, std::vector<std::vector<int>>& table) {
    std::pair<int, int> res = {-1, -1};
    std::vector<std::string> color(vertex + 1, "WHITE");
    std::vector<int> par(vertex + 1);
    for(int i = 1; i <= vertex; ++i) {
        if (color[i] == "WHITE") dfs(res, table, color, par, i);
        if (res.first != -1) break;
    }
    std::vector<int> answer;
    if (res.first == -1) return answer;
    
    while(res.first != res.second) {
        answer.push_back(res.first);
        res.first = par[res.first];
    }
    answer.push_back(res.second);
    return answer;
}

int main() {
    int vertex, edge;
    std::cin >> vertex >> edge;
    std::vector<std::vector<int>> table(vertex + 1);
    for (int i = 0; i < edge; ++i) {
        int first, second;
        std::cin >> first >> second;
        table[first].push_back(second);
    }
    auto answer = solve(vertex, table);
    if (answer.empty()) {
        std::cout << "NO" << std::endl;
    }
    else {
        std::cout << "YES" <<std::endl;
        for (int i = answer.size() -1; i >= 0 ; --i){
            std::cout << answer[i] << " ";
        }
    }
    return 0;
}
