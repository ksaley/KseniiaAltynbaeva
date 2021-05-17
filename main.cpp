#include <iostream>
#include<vector>
#include <set>

struct edge {
    int from;
    int to;
    long long  weight;
    edge(int from, int  to, long long weight): from(from), to(to), weight(weight) {}
    bool operator<(const edge& e) const {
        if (weight == e.weight) {
            if( from == e.from) {
                if (to < e.to) return true;
            }
            if (from < e.from) return true;
        }
        if (weight < e.weight) return true;

        return false;
    }
};


long long prims_algorithm(std::vector<std::vector<edge>>& edges) {
    std::vector<bool> used(edges.size(), false);
    used[1] = true;
    std::set<edge> q;
    long long ans = 0;
    for (auto& e : edges[1]) {
        q.insert(e);
    }
    while (!q.empty()) {
        auto front = *q.begin();
        q.erase(*q.begin());
        auto vertex = front.to;
        if (used[vertex]) continue;
        used[vertex] = true;
        ans += front.weight;
        for (auto& e : edges[vertex]) {
            if (used[e.to]) continue;
            q.insert(e);
        }
   }
    return ans;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<edge>> edges(n + 1);
    for (int i = 1; i <= m; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        edges[from].push_back(edge(from, to, weight));
        edges[to].push_back(edge(to, from, weight));
    }
    std::cout << prims_algorithm(edges);
    return 0;
}