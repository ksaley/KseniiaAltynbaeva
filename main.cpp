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

int get(int v, std::vector<int >& prev) {
    if (prev[v] == -1) {
        return v;
    }
    else {
        return prev[v] = get(prev[v], prev);
    }
}

void unite(int u, int v, std::vector<int>& size, std::vector<int >& prev) {
    u = get(u, prev);
    v= get(v, prev);
    if (u == v) return;
    if (size[v] < size[v]) std::swap(u, v);
    prev[v] = u;
    size[u]+=size[v];
}

long long kruskal_algorithm(int n, std::set <edge>& edges) {
    std::vector<int> prev(n+1,-1);
    std::vector<int> size(n+1, 1);
    long long ans = 0;
    while(!edges.empty()) {
        auto front = *(edges.begin());
        edges.erase(edges.begin());
        if (get(front.to, prev)!= get(front.from, prev)) {
            ans += front.weight;
            unite(front.to, front.from,size, prev);
        }
    }
    return ans;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::set<edge> edges;
    for (int i = 1; i <= m; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        edges.insert(edge(from, to, weight));
    }
    std::cout << kruskal_algorithm(n, edges);
    return 0;
}