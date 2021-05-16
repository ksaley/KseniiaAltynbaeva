#include <iostream>
#include <vector>
#include <unordered_map>
#define inf 9'000'000'000'000'000'001ll

struct edge {
    int from;
    int to;
    long long  weight;
    edge(int from, int  to, long long weight): from(from), to(to), weight(weight) {}
    edge(int from, int  to): from(from), to(to) {}
    bool operator==(const edge& e) const {
        return from == e.from && to == e.to ;
    }
};

template <>
class std::hash <edge>{
public:
    size_t operator()(const edge& e) const {
        int x = 98'798'431;
        return e.to * x + e.from * x * x;
    }
};

void dfs(std::vector<std::vector<int>>& table,std::vector <bool>& used, long long vertex) {
    used[vertex] = true;
    for (auto next : table[vertex]) {
        if (!used[next]) dfs(table, used, next);
    }
}

std::pair<std::vector<long long>, std::vector <bool>>
                implement_dp(int s, int n, std::unordered_map<edge, long long>& edges) {
    long long check;
    std::vector <edge> clean_edges;
    std::vector <std::vector<int>> table(n + 1);
    clean_edges.reserve(edges.size());
    for (auto& [key, value] : edges) {
        table[key.from].push_back(key.to);
        clean_edges.emplace_back(key.from, key.to, value);
    }
    std::vector<long long>  dp(n + 1, inf);
    dp[s] = 0;
    for (int i = 1; i <= n + 1; ++i) {
        check  = 0;
        for (int k = 0; k < clean_edges.size(); ++k) {
            if (dp[clean_edges[k].from] < inf) {
                if (dp[clean_edges[k].from] + clean_edges[k].weight < dp[clean_edges[k].to]) {
                    dp[clean_edges[k].to] = std::min(dp[clean_edges[k].to], dp[clean_edges[k].from] + clean_edges[k].weight);
                    if (dp[clean_edges[k].to] < -inf) dp[clean_edges[k].to] = -inf;
                    check = clean_edges[k].to;
                }
            }
        }
    }
    std::vector<bool> visited (n+1, false);
    if (check) {
        dfs(table, visited, check);
    }
    return std::make_pair(dp, visited);
}
int main() {
    int n, m, s;
    std::cin >> n >> m >> s;
    std::unordered_map<edge, long long> edges;
    for (int i = 0; i < m; ++i) {
        long long from, to, weight;
        std::cin >> from >> to >> weight;
        if (from == to) continue;
        if(edges.count(edge(from, to))) {
            if (edges[edge(from, to)] > weight) {
                edges[edge(from, to)] = weight;
            }
        } else {
            edges[edge(from, to)] = weight;
        }
    }

    auto res = implement_dp(s, n, edges);
    auto dp = res.first;
    auto visited = res.second;
    for (int i = 1; i <= n; ++i) {
        if (dp[i] == inf) std::cout << "*";
        else if (visited[i]) std::cout << "-";
        else std::cout << dp[i];
        std::cout << '\n';
    }
    return 0;
}