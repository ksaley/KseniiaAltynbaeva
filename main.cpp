#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
#define inf 9'000'000'000ll
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

std::vector<long long> dijkstra(int start, std::vector<std::vector<edge>>& edges) {
    std::vector<long long> dist(edges.size(), inf);
    std::vector<bool> used(edges.size(), false);
    dist[start] = 0;
    std::set<std::pair<int, int>> q;
    q.insert({0, start});
    while (!q.empty()) {
        auto front = *q.begin();
        q.erase(*q.begin());
        if (dist[front.second] < front.first) continue;
        //used[front.second] = true;
        for (auto& e : edges[front.second]) {
            if (dist[e.to] > dist[e.from] + e.weight) {
                if (q.count({dist[e.to], e.to})) {
                    q.erase({dist[e.to], e.to});
                }
                dist[e.to] = dist[e.from] + e.weight;
                if (!used[e.to]) q.insert({dist[e.to], e.to});
            }
        }
        //if (dist[front.to] < dist[front.from] + front.weight) continue;
    }
    return dist;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    int start, finish;
    std::cin >> start >> finish;
    std::vector<std::vector<edge>> edges(n+1);
    for ( int i = 1; i <= m; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        edges[from].push_back(edge(from, to, weight));
        edges[to].push_back(edge(to, from, weight));
    }
    auto dist = dijkstra(start, edges);
    std::cout << (dist[finish] == inf? -1: dist[finish]);
    return 0;
}