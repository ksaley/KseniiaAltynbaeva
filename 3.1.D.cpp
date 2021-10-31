#include <iostream>
#include <vector>
#include <map>

struct node {
    std::map<char, int> to;
    bool term;
    int amount = 0;
};

void add(std::vector<node> &trie, const std::string &string) {
    int vertex = 0;
    for (int i = 0; i < string.size(); ++i) {
        if (!trie[vertex].to.count(string[i])) {
            trie.emplace_back();
            trie[vertex].to[string[i]] = int(trie.size()) - 1;
        }
        vertex = trie[vertex].to[string[i]];
    }
    trie[vertex].term = true;
    trie[vertex].amount++;
}

void dfs(std::vector<int> &points, std::vector<node> &trie,
         int v, std::string *p, int &pos) {
    for (auto to: trie[v].to) {
        if (trie[to.second].term) {
            for (int i = 0; i < trie[to.second].amount; ++i) {
                for (int j = 0; j < points[pos]; ++j) {
                    std::cout << '.';
                }
                ++pos;
                std::cout << *p + to.first;
            }
        }
        *p += to.first;
        dfs(points, trie, to.second, p, pos);
        (*p).pop_back();
    }
}

std::pair<std::vector<int>, std::vector<node>> read() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::vector<node> trie(1);
    trie.reserve(50000);
    std::vector<int> vec;
    char c;
    int count = 0;
    std::string cur;
    bool word = false, point = true;
    //int f = 0;
    while (std::cin >> c) {
        if (c == '.') {
            if (count == 0 && !word) {
                add(trie, cur);
            }
            cur.clear();
            ++count;
            word = true;
            point = true;
        }
        if (c != '.') {
            if (point) {
                vec.push_back(count);
                //std::cout << count << " ";
            }
            cur += c;
            count = 0;
            point = false;
            word = false;
        }
    }
    if (count != 0) vec.push_back(count);
    if (!cur.empty()) add(trie, cur);
    return {vec, trie};
}

int main() {
    auto data = read();
    auto vec = data.first;
    auto trie = data.second;
    int t = 0;
    auto *s = new std::string(); // можно просто std::string s создать вроде, зачем так сложно?
    dfs(vec, trie, 0, s, t);
    if (t == int(vec.size()) - 1) { // std::size_t - это вроде unsigned int, приводя его к int можно получить другое значение. Можно t объявить как std::size_t t = 0
        for (int j = 0; j < vec[t]; ++j) {
            std::cout << '.';
        }
    }
    delete s;
}
