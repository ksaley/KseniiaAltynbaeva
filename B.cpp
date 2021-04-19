#include <iostream>
#include <vector>
#include <string>
int timer = 0;
void dfs(std::vector<int>& tin, std::vector<int>& tout, std::vector<std::vector<int>>& vertex,
         std::vector<std::string>& color,   std::vector<int>& par, int v, int p = -1) {
    par[v] = p;
    tin[v] = timer++;
    color[v] = "GRAY";
    for (auto to : vertex[v]) {
        if (color[to] != "WHITE") continue;
        dfs(tin, tout, vertex, color, par, to, v);
    }
    tout[v] = timer++;
    color[v] = "BLACK";
}
int is_parent( std::vector<int>& tin,  std::vector<int>& tout,int x, int y){
    return (tin[x] < tin[y] && tout[x]> tout[y])?1:0;
}
int main() {
    int vertex;
    std::cin >> vertex;
    int place = -1;
    std::vector <std::vector<int>> table(vertex + 1);
    for (int i = 1; i  <= vertex; ++i) {
        int x;
        std::cin >> x;
        if (!x) place = i;
        table[x].push_back(i);
    }

    std::vector<int> tin(vertex + 1);
    std::vector<int> tout(vertex + 1);
    std::vector<std::string> color(vertex + 1, "WHITE");
    std::vector<int> par(vertex + 1, -1);
    dfs(tin, tout,table, color, par, place);

    int request;
    std::cin >> request;
    for ( int i = 0; i < request; ++i) {
        int x, y;
        std::cin >> x >> y;
        std::cout << is_parent(tin, tout, x, y) << std::endl;
    }
    return 0;
}
