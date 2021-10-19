#include <iostream>
#include <vector>
#include <map>
#include <queue>

int alphabet = 2;

struct node {
    std::vector<int> to;
    bool term;
    int link;
    int amount = 0;
    std::vector<int> numbers;

    node() {
        to.resize(alphabet, -1);
        term = false;
        link = -1;
    }
};

void add(std::vector<node> &trie, const std::string &string, int number) {
    int vertex = 0;
    for (int i = 0; i < string.size(); ++i) {
        if (trie[vertex].to[string[i] - '0'] == -1) {
            trie.emplace_back();
            trie[vertex].to[string[i] - '0'] = int(trie.size()) - 1;
        }
        vertex = trie[vertex].to[string[i] - '0'];
    }
    trie[vertex].term = true;
    trie[vertex].numbers.push_back(number);
    trie[vertex].amount++;
}

void Aho_Corasick(std::vector<node> &trie) {
    trie[0].link = 0;
    for (int character = 0; character < alphabet; ++character) {
        if (trie[0].to[character] != -1) {
            continue;
        }
        trie[0].to[character] = 0;
    }
    std::queue<int> queue;
    queue.push(0);
    while (!queue.empty()) {
        int vertex = queue.front();
        queue.pop();
        for (int character = 0; character < alphabet; ++character) {
            int next_vertex = trie[vertex].to[character];
            if (trie[next_vertex].link != -1) {
                continue;
            }
            trie[next_vertex].link = (vertex == 0 ? 0 : trie[trie[vertex].link].to[character]);
            for (int d = 0; d < alphabet; ++d) {
                if (trie[next_vertex].to[d] != -1) {
                    continue;
                }
                trie[next_vertex].to[d] = trie[trie[next_vertex].link].to[d];
            }
            queue.push(next_vertex);
        }
    }
}

std::vector<int> bfs(std::vector<node> &trie) {
    std::vector<int> exit;
    std::queue<int> queue;
    queue.push(0);
    exit.resize(trie.size(), -1);
    exit[0] = 0;

    while (!queue.empty()) {
        int vertex = queue.front();
        queue.pop();
        for (int character = 0; character < alphabet; ++character) {
            int u = trie[vertex].to[character];
            if (exit[u] != -1) continue;
            exit[u] = trie[trie[u].link].term ? trie[u].link : exit[trie[u].link];
            queue.push(u);

        }
    }
    return exit;
}

void dfs(std::vector<int> &exit, bool &res, std::vector<node> &t, std::vector<std::string> &color, int v) {
    color[v] = "GRAY";
    for (auto to: t[v].to) {
        if (color[to] == "GRAY") {
            res = true;
        }
        if (color[to] != "WHITE" || t[to].term || exit[to] != 0) continue;
        dfs(exit, res, t, color, to);
    }
    color[v] = "BLACK";
}

bool
get_answer(std::vector<node> &trie) {
    Aho_Corasick(trie);
    auto exit = bfs(trie);
    std::vector<std::string> visited(trie.size(), "WHITE");
    bool res = false;
    dfs(exit, res, trie, visited, 0);
    return res;
}

std::vector<node> read() {
    std::vector<node> trie(1);
    trie.reserve(50000);
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        add(trie, word, i);
    }
    return trie;
}

int main() {
    auto trie = read();
    std::cout << (get_answer(trie) ? "TAK" : "NIE");
}
