#include <iostream>
#include <vector>
#include <algorithm>

void dfs(int& timer,std::vector<bool>& bridges, std::vector<int>& ret, std::vector<int>& tin, std::vector<int>& tout,
         std::vector<std::vector<int>>& table, std::vector <bool>& used, int vertex, int p = -1) {
    tin[vertex] = timer++;
    ret[vertex] = tin[vertex];
    used[vertex] = true;
    int cnt = 0;
    for (auto next : table[vertex]) {
        if(next == p) continue;
        if (used[next]) {
            ret[vertex]  = std::min(ret[vertex], tin[next]);
        }
        else {
            dfs(timer, bridges,ret, tin, tout, table, used, next, vertex);
            ++ cnt;
            ret[vertex] = std::min(ret[vertex], ret[next]);
            if (p!= -1) {
                if (ret[next] >= tin[vertex]) {
                    bridges[vertex] = true;
                }
            }
            else if (cnt >= 2){
                bridges[vertex] = true;
            }

        }
    }
    tout[vertex] = timer++;
}
std::vector<bool> get_bridges(std::vector <std::vector<int>>& table, int vertex) {
    std::vector <int> tin(vertex + 1), tout(vertex + 1), ret(vertex + 1);
    std::vector <bool> used (vertex + 1, false);
    std::vector<bool> bridges(vertex + 1, false);
    int timer = 0;
    for (int i = 1; i  <= vertex; ++i) {
        if (used[i]) continue;
        dfs(timer,bridges, ret, tin, tout, table, used, i);
    }
    return bridges;
}

int get_bridges_amount(std::vector<bool>& bridges) {
    int ans = 0;
    for (auto cur : bridges) {
        ans+= cur;
    }
    return ans;
}

int main() {
    int edge, vertex;
    std::cin >> vertex >> edge;
    std::vector <std::vector<int>> table(vertex + 1);
    for ( int i = 1; i <= edge; ++i) {
        int x, y;
        std::cin >> x >> y;
        table[x].push_back(y);
        table[y].push_back(x);
    }
    auto bridges = get_bridges(table, vertex);
    auto ans = get_bridges_amount(bridges);
    std::cout << ans << std::endl;
    for (int i = 1; i <= vertex; ++i ) {
        if(bridges[i]) {
            std::cout << i << " ";
        }
    }
    return 0;
}
